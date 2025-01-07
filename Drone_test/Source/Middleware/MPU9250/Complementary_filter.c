#include <math.h>
#include "MPU9250.h"
#include "Complementary_filter.h"
#include "Digital_filters.h"

// Define the alpha constant (between 0 and 1)
#define ALPHA 0.98f
#define M_PI 3.14159265358979323846




// Gravity compensation: remove gravity from accelerometer data based on roll and pitch
void apply_gravity_compensation(MPU9250_Data *mpu_data)
{
    float accel_x = mpu_data->accel_x;
    float accel_y = mpu_data->accel_y;
    float accel_z = mpu_data->accel_z;

    // Use the roll and pitch angles to remove gravity
    float gravity_x = sin(mpu_data->angle_pitch * M_PI / 180.0f);
    float gravity_y = -sin(mpu_data->angle_roll * M_PI / 180.0f);
    float gravity_z = cos(mpu_data->angle_roll * M_PI / 180.0f) * cos(mpu_data->angle_pitch * M_PI / 180.0f);

    // Remove gravity effect from accelerometer readings
    mpu_data->accel_x -= gravity_x;
    mpu_data->accel_y -= gravity_y;
    mpu_data->accel_z -= gravity_z;
}



// Example function that uses the filter to smooth sensor data before processing
void Complementary_Filter(MPU9250_Data *mpu_data, float dt)
{
    // Apply low-pass filter to accelerometer data
    apply_low_pass_filter(mpu_data, dt);

    // Apply high-pass filter to gyroscope data
    apply_high_pass_filter(mpu_data, dt);

    // Calculate roll and pitch using accelerometer data
    float accel_roll = 0.0f;
    float accel_pitch = 0.0f;

    // Avoid calculating roll when pitch is near ±90° (as accelerometer data becomes unreliable)
    if (fabs(mpu_data->accel_x) > 0.1f || fabs(mpu_data->accel_y) > 0.1f) {
        accel_roll = atan2(mpu_data->accel_y, mpu_data->accel_z) * 180.0f / M_PI;
    }
    accel_pitch = atan2(mpu_data->accel_x, sqrt(mpu_data->accel_y * mpu_data->accel_y + mpu_data->accel_z * mpu_data->accel_z)) * 180.0f / M_PI;

    // Integrate gyroscope data to get the angle (in degrees)
    mpu_data->gyro_x += mpu_data->gyro_x * dt;  // Gyroscope is assumed to give degrees per second
    mpu_data->gyro_y += mpu_data->gyro_y * dt;

    // Combine accelerometer and gyroscope data using the complementary filter
    mpu_data->angle_roll = ALPHA * (mpu_data->angle_roll + mpu_data->gyro_x * dt) + (1 - ALPHA) * accel_roll;
    mpu_data->angle_pitch = ALPHA * (mpu_data->angle_pitch + mpu_data->gyro_y * dt) + (1 - ALPHA) * accel_pitch;

    // Ensure the angles remain within -180° to +180° range
    if (mpu_data->angle_roll > 180.0f) mpu_data->angle_roll -= 360.0f;
    if (mpu_data->angle_roll < -180.0f) mpu_data->angle_roll += 360.0f;

    if (mpu_data->angle_pitch > 180.0f) mpu_data->angle_pitch -= 360.0f;
    if (mpu_data->angle_pitch < -180.0f) mpu_data->angle_pitch += 360.0f;


}




