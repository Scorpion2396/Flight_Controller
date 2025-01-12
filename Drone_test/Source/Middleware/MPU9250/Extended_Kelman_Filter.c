/*
 * EKF_filter.c
 *
 *  Created on: 31-Dec-2024
 *      Author: Saksham Raj
 */

#include <math.h>
#include "MPU9250.h"
#include "Common_Datatype.h"
#include "MPU9250_Interface.h"
#include <stdlib.h>
#include <stdbool.h>

#define M_PI 3.14159265358979323846

// Define Extended Kalman filter structure
typedef struct {
    float angle;        // Estimated angle
    float bias;         // Gyroscope bias
    float P[2][2];      // Error covariance matrix
    float Q_angle;      // Process noise covariance for angle
    float Q_bias;       // Process noise covariance for bias
    float R_measure;    // Measurement noise covariance
} EKF_Filter;

// Initialize the EKF filter
void EKF_Init(EKF_Filter *ekf) {
    ekf->angle = 0.0f;
    ekf->bias = 0.0f;
    ekf->P[0][0] = 1.0f;  // Initial estimation error covariance
    ekf->P[0][1] = 0.0f;
    ekf->P[1][0] = 0.0f;
    ekf->P[1][1] = 1.0f;
    ekf->Q_angle = 0.001f;  // Process noise for angle
    ekf->Q_bias = 0.003f;   // Process noise for bias
    ekf->R_measure = 0.03f; // Measurement noise
}

// Extended Kalman filter update with Jacobian
float EKF_Update(EKF_Filter *ekf, float newAngle, float newRate, float dt, float accel_x, float accel_y, float accel_z) {
    // Prediction step (nonlinear model)
    ekf->angle += dt * (newRate - ekf->bias);  // Angle is predicted based on the gyroscope rate

    // Compute Jacobian for the nonlinear state model (gyroscope)
    float F[2][2] = {{1.0f, -dt}, {0.0f, 1.0f}};  // State transition matrix (Jacobian)

    // Error covariance prediction
    ekf->P[0][0] += dt * (dt * ekf->P[1][1] - ekf->P[0][1] - ekf->P[1][0] + ekf->Q_angle);
    ekf->P[0][1] -= dt * ekf->P[1][1];
    ekf->P[1][0] -= dt * ekf->P[1][1];
    ekf->P[1][1] += ekf->Q_bias * dt;

    // Compute Jacobian for the measurement model (accelerometer)
    float H[2][2] = {
        {accel_y / (accel_y * accel_y + accel_z * accel_z), 0},   // Roll Jacobian w.r.t. accel_y
        {-accel_x / (accel_x * accel_x + accel_y * accel_y + accel_z * accel_z), 0} // Pitch Jacobian w.r.t. accel_x
    };

    // Measurement update (using accelerometer data)
    float y = newAngle - ekf->angle;  // Difference between measured angle and predicted angle
    float S = ekf->P[0][0] + ekf->R_measure;  // Measurement uncertainty
    float K[2];  // Kalman gain

    // Compute Kalman gain
    K[0] = ekf->P[0][0] / S;
    K[1] = ekf->P[1][0] / S;

    // Update the angle estimate and the bias
    ekf->angle += K[0] * y;
    ekf->bias += K[1] * y;

    // Update the error covariance matrix
    float P00_temp = ekf->P[0][0];
    float P01_temp = ekf->P[0][1];
    ekf->P[0][0] -= K[0] * P00_temp;
    ekf->P[0][1] -= K[0] * P01_temp;
    ekf->P[1][0] -= K[1] * P00_temp;
    ekf->P[1][1] -= K[1] * P01_temp;

    return ekf->angle;
}

// Function to use Extended Kalman filter for pitch and roll
void EKF_Filter_Angles(MPU9250_Data *mpu_data, float dt) {
    static EKF_Filter ekf_roll;
    static EKF_Filter ekf_pitch;

    // Initialize the EKF filter if it's not done yet
    static bool is_initialized = 0;
    if (!is_initialized) {
        EKF_Init(&ekf_roll);
        EKF_Init(&ekf_pitch);
        is_initialized = 1;
    }

    // Apply low-pass filter to accelerometer data
    apply_low_pass_filter(mpu_data, dt);

    // Apply high-pass filter to gyroscope data
    apply_high_pass_filter(mpu_data, dt);

    // Calculate the roll and pitch from accelerometer data
    // Roll: rotation around Y-axis (tilting left or right)
    float accel_roll = atan2(mpu_data->accel_y, mpu_data->accel_z) * 180.0f / M_PI;

    // Pitch: rotation around X-axis (tilting toward or away from you)
    // Removed negative sign to ensure pitch increases when tilting toward you
    float accel_pitch = atan2(mpu_data->accel_x, sqrt(mpu_data->accel_y * mpu_data->accel_y + mpu_data->accel_z * mpu_data->accel_z)) * 180.0f / M_PI;

    // Use the gyroscope to calculate the rate of change of roll and pitch
    float gyro_roll_rate = mpu_data->gyro_x;  // Gyroscope rate in degrees per second
    float gyro_pitch_rate = mpu_data->gyro_y; // Gyroscope rate in degrees per second

    // Update the roll and pitch angles using the EKF
    mpu_data->angle_roll = EKF_Update(&ekf_roll, accel_roll, gyro_roll_rate, dt, mpu_data->accel_x, mpu_data->accel_y, mpu_data->accel_z);
    mpu_data->angle_pitch = EKF_Update(&ekf_pitch, accel_pitch, gyro_pitch_rate, dt, mpu_data->accel_x, mpu_data->accel_y, mpu_data->accel_z);

    // Ensure the angles stay within the range of -180� to +180�
    if (mpu_data->angle_roll > 180.0f) mpu_data->angle_roll -= 360.0f;
    if (mpu_data->angle_roll < -180.0f) mpu_data->angle_roll += 360.0f;
    if (mpu_data->angle_pitch > 180.0f) mpu_data->angle_pitch -= 360.0f;
    if (mpu_data->angle_pitch < -180.0f) mpu_data->angle_pitch += 360.0f;
}
