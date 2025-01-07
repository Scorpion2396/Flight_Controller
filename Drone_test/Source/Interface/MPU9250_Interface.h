/*
 * MPU9250_Interface.h
 *
 *  Created on: 28-Dec-2024
 *      Author: Saksham Raj
 */

#ifndef SOURCE_INTERFACE_MPU9250_INTERFACE_H_
#define SOURCE_INTERFACE_MPU9250_INTERFACE_H_

#include "Common_Datatype.h"

typedef struct {
    int16_t accel_x_raw;
    int16_t accel_y_raw;
    int16_t accel_z_raw;
    int16_t gyro_x_raw;
    int16_t gyro_y_raw;
    int16_t gyro_z_raw;
    int16_t mag_x_raw;
    int16_t mag_y_raw;
    int16_t mag_z_raw;
    int16_t temp_raw;

    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float mag_x;
    float mag_y;
    float mag_z;
    float temperature;

    float accel_scale;
    float gyro_scale;
    float mag_scale;

    // Calibration offsets
    int32_t accel_offset[3];
    int32_t gyro_offset[3];
    int32_t mag_offset[3];

    float angle_roll;
    float angle_pitch;

    float velocity_x;
    float velocity_y;
    float velocity_z;

    float quaternion[4];
    float rotation_matrix[3][3];  // 3x3 matrix to store the rotation matrix
    float axis_angle[4];
} MPU9250_Data;

// Function Prototypes
extern void MPU9250_Init(MPU9250_Data *mpu_data);
extern void MPU9250_Read_Accelerometer(MPU9250_Data *mpu_data);
extern void MPU9250_Read_Gyroscope(MPU9250_Data *mpu_data);
extern void MPU9250_Read_Magnetometer(MPU9250_Data *mpu_data);
extern void MPU9250_Read_Temperature(MPU9250_Data *mpu_data);
extern void Read_MPU9250(MPU9250_Data *mpu_data);
extern void MPU9250_Get_Offset(MPU9250_Data *mpu_data);



extern float Calculate_Compass_Heading();

extern void Complementary_Filter(MPU9250_Data *mpu_data, float dt);

extern void Kalman_Filter_Angles(MPU9250_Data *mpu_data, float dt);

extern void EKF_Filter_Angles(MPU9250_Data *mpu_data, float dt);

extern void EKF_Filter_Rotation(MPU9250_Data *mpu_data, float dt);

#endif /* SOURCE_INTERFACE_MPU9250_INTERFACE_H_ */
