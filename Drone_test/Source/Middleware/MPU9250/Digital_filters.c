/*
 * Digital_filters.c
 *
 *  Created on: 07-Jan-2025
 *      Author: Saksham Raj
 */

#include "Digital_filters.h"

// Define the alpha constant (between 0 and 1)
#define M_PI 3.14159265358979323846

// Define the alpha constants for blending LPF and HPF (range: 0 to 1)
#define LPF_ALPHA_BLEND 1.0  // Blend for LPF (0: no filter, 1: full LPF effect)
#define HPF_ALPHA_BLEND 1.0  // Blend for HPF (0: no filter, 1: full HPF effect)


// Define time constants for the filters (cutoff frequencies in Hz)
#define LPF_CUTOFF_FREQ 5.0f  // Example low-pass cutoff frequency (in Hz)
#define HPF_CUTOFF_FREQ 1.0f  // Example high-pass cutoff frequency (in Hz)

float compute_alpha_lpf(float dt)
{
    float tau = 1.0f / (2 * M_PI * LPF_CUTOFF_FREQ);  // Time constant for low-pass filter
    return dt / (tau + dt);
}

float compute_alpha_hpf(float dt)
{
    float tau = 1.0f / (2 * M_PI * HPF_CUTOFF_FREQ);  // Time constant for high-pass filter
    return dt / (tau + dt);
}

// Low-pass filter for smoothing sensor data (first-order filter)
float low_pass_filter(float previous_value, float current_value, float alpha)
{
    // Apply the low-pass filter formula, considering the time step
    return (alpha * current_value) + ((1.0f - alpha) * previous_value);
}

// High-pass filter for removing low-frequency drift (first-order filter)
float high_pass_filter(float current_value, float previous_value, float alpha)
{
    // Apply the high-pass filter formula, considering the time step
    return alpha * (current_value - previous_value);
}

// Function to apply a low-pass filter to the accelerometer readings
void apply_low_pass_filter(MPU9250_Data *mpu_data, float dt)
{
    // Static variables to hold the previous values of the accelerometer readings
    static float prev_accel_x = 0.0f;
    static float prev_accel_y = 0.0f;
    static float prev_accel_z = 0.0f;
    float filtered_accel_x = 0.0f;
    float filtered_accel_y = 0.0f;
    float filtered_accel_z = 0.0f;

    // Compute the alpha value based on dt and cutoff frequency
    float alpha = compute_alpha_lpf(dt);

    // Apply the low-pass filter to smooth the accelerometer readings
    filtered_accel_x = low_pass_filter(prev_accel_x, mpu_data->accel_x, alpha);
    filtered_accel_y = low_pass_filter(prev_accel_y, mpu_data->accel_y, alpha);
    filtered_accel_z = low_pass_filter(prev_accel_z, mpu_data->accel_z, alpha);

    // Blend the LPF and HPF data based on the blending factors
    mpu_data->accel_x = LPF_ALPHA_BLEND * filtered_accel_x + (1.0f - LPF_ALPHA_BLEND) * mpu_data->accel_x;
    mpu_data->accel_y = LPF_ALPHA_BLEND * filtered_accel_y + (1.0f - LPF_ALPHA_BLEND) * mpu_data->accel_y;
    mpu_data->accel_z = LPF_ALPHA_BLEND * filtered_accel_z + (1.0f - LPF_ALPHA_BLEND) * mpu_data->accel_z;

    // Update the static variables with the current values for the next iteration
    prev_accel_x = mpu_data->accel_x;
    prev_accel_y = mpu_data->accel_y;
    prev_accel_z = mpu_data->accel_z;
}


// Function to apply high-pass filter to the gyroscope readings
void apply_high_pass_filter(MPU9250_Data *mpu_data, float dt)
{
    // Static variables to hold the previous values of the gyroscope readings
    static float prev_gyro_x = 0.0f;
    static float prev_gyro_y = 0.0f;
    static float prev_gyro_z = 0.0f;
    float filtered_gyro_x = 0.0f;  // Added missing semicolon
    float filtered_gyro_y = 0.0f;  // Added missing semicolon
    float filtered_gyro_z = 0.0f;  // Added missing semicolon

    // Compute the alpha value based on dt and cutoff frequency
    float alpha = compute_alpha_hpf(dt);

    // Apply the high-pass filter to the gyroscope data to remove low-frequency drift
    filtered_gyro_x = high_pass_filter(mpu_data->gyro_x, prev_gyro_x, alpha);
    filtered_gyro_y = high_pass_filter(mpu_data->gyro_y, prev_gyro_y, alpha);
    filtered_gyro_z = high_pass_filter(mpu_data->gyro_z, prev_gyro_z, alpha);

    // Blend the HPF and LPF data based on the blending factors
    mpu_data->gyro_x = HPF_ALPHA_BLEND * filtered_gyro_x + (1.0f - HPF_ALPHA_BLEND) * mpu_data->gyro_x;
    mpu_data->gyro_y = HPF_ALPHA_BLEND * filtered_gyro_y + (1.0f - HPF_ALPHA_BLEND) * mpu_data->gyro_y;
    mpu_data->gyro_z = HPF_ALPHA_BLEND * filtered_gyro_z + (1.0f - HPF_ALPHA_BLEND) * mpu_data->gyro_z;

    // Store the current gyroscope values for the next cycle
    prev_gyro_x = filtered_gyro_x;
    prev_gyro_y = filtered_gyro_y;
    prev_gyro_z = filtered_gyro_z;
}

