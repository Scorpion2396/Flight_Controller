/*
 * MPU9250.h
 *
 *  Created on: 28-Dec-2024
 *      Author: Saksham Raj
 */

#ifndef SOURCE_MIDDLEWARE_MPU9250_MPU9250_H_
#define SOURCE_MIDDLEWARE_MPU9250_MPU9250_H_


#include "Common_Datatype.h"
#include "MPU9250_Interface.h"


//#define ACCEL_X_OFFSET -5422
//#define ACCEL_Y_OFFSET -8429
//#define ACCEL_Z_OFFSET 9225

#define ACCEL_X_OFFSET 400
#define ACCEL_Y_OFFSET 396
#define ACCEL_Z_OFFSET 11321

// Address for AK8963 magnetometer (if AD0 is low)
#define MPU9250_MAGNETOMETER_ADDRESS    0x0C
#define AK8963_WHO_AM_I_VALUE           0x48  // Expected value for the WHO_AM_I register of AK8963
#define AK8963_WHO_AM_I                 0x00  // AK8963 WHO_AM_I register address
#define AK8963_CNTL                     0x0A  // AK8963 Control Register address
#define AK8963_XOUT_L                   0x03  // X-axis Data (Low byte)
#define AK8963_XOUT_H                   0x04  // X-axis Data (High byte)
#define AK8963_YOUT_L                   0x05  // Y-axis Data (Low byte)
#define AK8963_YOUT_H                   0x06  // Y-axis Data (High byte)
#define AK8963_ZOUT_L                   0x07  // Z-axis Data (Low byte)
#define AK8963_ZOUT_H                   0x08  // Z-axis Data (High byte)
#define AK8963_ST1                      0x02  // Status 1 Register

// Define the mask for the Bandwidth bits (Bit 6) in AK8963_CNTL register
#define AK8963_BW_MASK                  0x40  // 01000000 (Bit 6 represents the bandwidth setting)
// Define the mask for the Mode bits (Bits 0 and 1) in AK8963_CNTL register
#define AK8963_MODE_MASK                0x03  // 00000011 (Bits 0 and 1 represent the mode setting)
// Magnetometer scale setting bits (AK8963_CNTL register)
#define AK8963_SCALE_MASK               0x30  // 00110000 (Bits 4 and 5 represent the scale setting)




#define MPU9250_ADDRESS                 0x68  // MPU9250 I2C address

// Mask for the ACCEL_FS_SEL bits (bits 1 and 0 in ACCEL_CONFIG register)
#define ACCEL_FS_SEL_MASK 0x18  // 0b00011000

// Mask for the GYRO_FS_SEL bits (bits 1 and 0 in GYRO_CONFIG register)
#define GYRO_FS_SEL_MASK  0x18  // 0b00011000


// Register Definitions
#define MPU9250_PWR_MGMT_1             0x6B    // Power Management 1 register
#define MPU9250_PWR_MGMT_2             0x6C    // Power Management 2 register
#define MPU9250_WHO_AM_I               0x75    // Who Am I register
#define MPU9250_WHO_AM_I_VALUE         0x70    // Expected value of WHO_AM_I register

// Accelerometer, Gyroscope and Magnetometer Config Registers
#define MPU9250_MAGNETOMETER_MODE      0x0A

#define MPU9250_GYRO_BW_CONFIG         0x1A
#define MPU9250_GYRO_SCALE_CONFIG      0x1B
#define MPU9250_ACCEL_SCALE_CONFIG     0x1C
#define MPU9250_ACCEL_BW_CONFIG        0x1D

#define MPU9250_SMPLRT_DIV             0x19

#define MPU9250_INT_PIN_CFG    0x37

// MPU9250 Register addresses
#define MPU9250_ACCEL_XOUT_H   0x3B
#define MPU9250_ACCEL_XOUT_L   0x3C
#define MPU9250_ACCEL_YOUT_H   0x3D
#define MPU9250_ACCEL_YOUT_L   0x3E
#define MPU9250_ACCEL_ZOUT_H   0x3F
#define MPU9250_ACCEL_ZOUT_L   0x40
#define MPU9250_TEMP_OUT_H     0x41
#define MPU9250_TEMP_OUT_L     0x42
#define MPU9250_GYRO_XOUT_H    0x43
#define MPU9250_GYRO_XOUT_L    0x44
#define MPU9250_GYRO_YOUT_H    0x45
#define MPU9250_GYRO_YOUT_L    0x46
#define MPU9250_GYRO_ZOUT_H    0x47
#define MPU9250_GYRO_ZOUT_L    0x48
#define MPU9250_MAGNETOMETER_X 0x49 // Use correct register if applicable
#define MPU9250_MAGNETOMETER_Y 0x4A // Use correct register if applicable
#define MPU9250_MAGNETOMETER_Z 0x4B // Use correct register if applicable



// Accelerometer scale settings (according to MPU9250 datasheet)
typedef enum {
    ACCEL_SCALE_2G = 0x00,  // ±2g
    ACCEL_SCALE_4G = 0x08,  // ±4g
    ACCEL_SCALE_8G = 0x10,  // ±8g
    ACCEL_SCALE_16G = 0x18  // ±16g
} Accel_Scale_t;

// Gyroscope scale settings (according to MPU9250 datasheet)
typedef enum {
    GYRO_SCALE_250DPS = 0x00,  // ±250 degrees per second
    GYRO_SCALE_500DPS = 0x08,  // ±500 degrees per second
    GYRO_SCALE_1000DPS = 0x10, // ±1000 degrees per second
    GYRO_SCALE_2000DPS = 0x18  // ±2000 degrees per second
} Gyro_Scale_t;

// Magnetometer scale settings (based on AK8963 datasheet, part of MPU9250)
typedef enum {
    MAG_SCALE_14BIT = 0x00,  // 14-bit resolution (default scale)
    MAG_SCALE_16BIT = 0x10   // 16-bit resolution
} Mag_Scale_t;

// Magnetometer mode settings (based on AK8963 datasheet)
typedef enum {
    MAG_MODE_POWERDOWN     = 0x00,  // Power down mode
    MAG_MODE_SINGLE        = 0x01,  // Single measurement mode
    MAG_MODE_CONTINUOUS    = 0x02,  // Continuous measurement mode
    MAG_MODE_EXTERNAL      = 0x04   // External magnetometer mode (optional mode)
} MAG_Mode_t;

// Accelerometer bandwidth settings (according to MPU9250 datasheet)
typedef enum {
    ACC_BW_460Hz = 0x00,  // 460Hz bandwidth (maximum)
    ACC_BW_184Hz = 0x01,  // 184Hz bandwidth
    ACC_BW_92Hz  = 0x02,  // 92Hz bandwidth
    ACC_BW_41Hz  = 0x03,  // 41Hz bandwidth
    ACC_BW_20Hz  = 0x04,  // 20Hz bandwidth
    ACC_BW_10Hz  = 0x05   // 10Hz bandwidth
} ACC_Bandwidth_t;

// Gyroscope bandwidth settings (according to MPU9250 datasheet)
typedef enum {
    GYRO_BW_250Hz = 0x00,  // 250Hz bandwidth (default)
    GYRO_BW_184Hz = 0x01,  // 184Hz bandwidth
    GYRO_BW_92Hz  = 0x02,  // 92Hz bandwidth
    GYRO_BW_41Hz  = 0x03,  // 41Hz bandwidth
    GYRO_BW_20Hz  = 0x04,  // 20Hz bandwidth
    GYRO_BW_10Hz  = 0x05   // 10Hz bandwidth
} GYRO_Bandwidth_t;

// Magnetometer bandwidth settings (based on AK8963 datasheet)
typedef enum {
    MAG_BW_8Hz  = 0x00,   // 8Hz bandwidth
    MAG_BW_10Hz = 0x01,  // 10Hz bandwidth
    MAG_BW_12Hz = 0x02,  // 12Hz bandwidth
    MAG_BW_15Hz = 0x03,  // 15Hz bandwidth
    MAG_BW_20Hz = 0x04,  // 20Hz bandwidth
    MAG_BW_25Hz = 0x05,  // 25Hz bandwidth
    MAG_BW_30Hz = 0x06,  // 30Hz bandwidth
    MAG_BW_35Hz = 0x07,  // 35Hz bandwidth
    MAG_BW_40Hz = 0x08   // 40Hz bandwidth
} MAG_Bandwidth_t;


// Function Prototypes
void MPU9250_Init(MPU9250_Data *mpu_data);
void MPU9250_Init_Magnetometer(MPU9250_Data *mpu_data);
void MPU9250_INT_PIN_CFG_EN_BYPASS(MPU9250_Data *mpu_data);
void Set_Accelerometer_Scale(MPU9250_Data *mpu_data, Accel_Scale_t scale);
void Set_Gyroscope_Scale(MPU9250_Data *mpu_data, Gyro_Scale_t scale);
void Set_Magnetometer_Scale(MPU9250_Data *mpu_data, uint8_t scale);
void Set_Accelerometer_Bandwidth(ACC_Bandwidth_t bandwidth);
void Set_Gyroscope_Bandwidth(GYRO_Bandwidth_t bandwidth);
void Set_Magnetometer_Bandwidth(uint8_t bandwidth);
void Set_Magnetometer_Mode(uint8_t mode);
uint8_t Calculate_SMPLRT_DIV(uint16_t sample_rate);
void Set_Sample_Rate(uint16_t sample_rate);
void MPU9250_Read_Accelerometer(MPU9250_Data *mpu_data);
void MPU9250_Read_Gyroscope(MPU9250_Data *mpu_data);
void MPU9250_Read_Magnetometer(MPU9250_Data *mpu_data);
void MPU9250_Read_Temperature(MPU9250_Data *mpu_data);
void Convert_Accelerometer_To_Physical(MPU9250_Data *mpu_data);
void Convert_Gyroscope_To_Physical(MPU9250_Data *mpu_data);
void Convert_Magnetometer_To_Physical(MPU9250_Data *mpu_data);
void Convert_Temperature_To_Physical(MPU9250_Data *mpu_data);
void Apply_Offsets_accel(MPU9250_Data *mpu_data) ;
void Apply_Offsets_gyro(MPU9250_Data *mpu_data);
void Apply_Offsets(MPU9250_Data *mpu_data);
void MPU9250_set_accel_offset(MPU9250_Data *mpu_data);
void MPU9250_set_gyro_offset(MPU9250_Data *mpu_data);
void Read_MPU9250(MPU9250_Data *mpu_data);
void MPU9250_Get_Offset(MPU9250_Data *mpu_data);


#endif /* SOURCE_MIDDLEWARE_MPU9250_MPU9250_H_ */
