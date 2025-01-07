/*
 * Kelman_filter.c
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

// Define Kalman filter structure
typedef struct {
    float angle;        // Estimated angle
    float bias;         // Gyroscope bias
    float P[2][2];      // Error covariance matrix
    float Q_angle;      // Process noise covariance for angle
    float Q_bias;       // Process noise covariance for bias
    float R_measure;    // Measurement noise covariance
} KalmanFilter;

// Initialize the Kalman filter
void Kalman_Init(KalmanFilter *kf) {
    kf->angle = 0.0f;
    kf->bias = 0.0f;
    kf->P[0][0] = 1.0f;  // Initial estimation error covariance
    kf->P[0][1] = 0.0f;
    kf->P[1][0] = 0.0f;
    kf->P[1][1] = 1.0f;
    kf->Q_angle = 0.001f;  // Process noise for angle
    kf->Q_bias = 0.003f;   // Process noise for bias
    kf->R_measure = 0.03f; // Measurement noise
}

// Update the Kalman filter with the new gyroscope and accelerometer data
float Kalman_Update(KalmanFilter *kf, float newAngle, float newRate, float dt) {
    // Prediction step
    kf->angle += dt * (newRate - kf->bias);  // Angle is predicted based on the gyroscope rate
    kf->P[0][0] += dt * (dt * kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + kf->Q_angle);
    kf->P[0][1] -= dt * kf->P[1][1];
    kf->P[1][0] -= dt * kf->P[1][1];
    kf->P[1][1] += kf->Q_bias * dt;

    // Measurement step (using accelerometer data)
    float y = newAngle - kf->angle;  // Difference between measured angle and predicted angle
    float S = kf->P[0][0] + kf->R_measure;  // Measurement uncertainty
    float K[2];  // Kalman gain
    K[0] = kf->P[0][0] / S;
    K[1] = kf->P[1][0] / S;

    // Update the angle estimate and the bias
    kf->angle += K[0] * y;
    kf->bias += K[1] * y;

    // Update the error covariance matrix
    float P00_temp = kf->P[0][0];
    float P01_temp = kf->P[0][1];
    kf->P[0][0] -= K[0] * P00_temp;
    kf->P[0][1] -= K[0] * P01_temp;
    kf->P[1][0] -= K[1] * P00_temp;
    kf->P[1][1] -= K[1] * P01_temp;

    return kf->angle;
}

// Function to use Kalman filter for pitch and roll
void Kalman_Filter_Angles(MPU9250_Data *mpu_data, float dt) {
    static KalmanFilter kf_roll;
    static KalmanFilter kf_pitch;

    // Initialize the Kalman filter if it's not done yet
    static bool is_initialized = 0;
    if (!is_initialized) {
        Kalman_Init(&kf_roll);
        Kalman_Init(&kf_pitch);
        is_initialized = 1;
    }

    // Calculate the roll and pitch from accelerometer data
    // Roll: rotation around Y-axis (tilting left or right)
    float accel_roll = atan2(mpu_data->accel_y, mpu_data->accel_z) * 180.0f / M_PI;

    // Pitch: rotation around X-axis (tilting toward or away from you)
    // Removed negative sign to ensure pitch increases when tilting toward you
    float accel_pitch = atan2(mpu_data->accel_x, sqrt(mpu_data->accel_y * mpu_data->accel_y + mpu_data->accel_z * mpu_data->accel_z)) * 180.0f / M_PI;

    // Use the gyroscope to calculate the rate of change of roll and pitch
    float gyro_roll_rate = mpu_data->gyro_x;  // Gyroscope rate in degrees per second
    float gyro_pitch_rate = mpu_data->gyro_y; // Gyroscope rate in degrees per second

    // Update the roll and pitch angles using the Kalman filter
    mpu_data->angle_roll = Kalman_Update(&kf_roll, accel_roll, gyro_roll_rate, dt);
    mpu_data->angle_pitch = Kalman_Update(&kf_pitch, accel_pitch, gyro_pitch_rate, dt);

    // Ensure the angles stay within the range of -180° to +180°
    if (mpu_data->angle_roll > 180.0f) mpu_data->angle_roll -= 360.0f;
    if (mpu_data->angle_roll < -180.0f) mpu_data->angle_roll += 360.0f;
    if (mpu_data->angle_pitch > 180.0f) mpu_data->angle_pitch -= 360.0f;
    if (mpu_data->angle_pitch < -180.0f) mpu_data->angle_pitch += 360.0f;
}

