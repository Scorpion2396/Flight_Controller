/*
 * MPU9250.c
 *
 *  Created on: 28-Dec-2024
 *      Author: Saksham Raj
 */
#include "MPU9250.h"
#include "I2C_Interface.h"    // Include your I2C driver

uint8_t is_mpu9250_init = 0;
I2C_h mpu9250_i2c_h;

// Function to initialize the MPU9250 for drone use
void MPU9250_Init(MPU9250_Data *mpu_data)
{
    uint8_t data;

    DigitalWrite(PB4, HIGH);
    DigitalWrite(PA5, LOW);

    Delay_ms(1000);

    I2C_Init(&mpu9250_i2c_h, I2C_1, 400);

    // Step 1: Wake up the MPU9250 by writing 0x00 to PWR_MGMT_1 register
    data = 0x00;
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_PWR_MGMT_1, data);

    // Step 2: Set PWR_MGMT_2 to 0x00 to enable all sensors
    data = 0x00;  // Enable all sensors (accelerometer, gyroscope, magnetometer)
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_PWR_MGMT_2, data);

    // Step 3: Verify the device ID (WHO_AM_I should return 0x71)
    data = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_WHO_AM_I);
    if (data != MPU9250_WHO_AM_I_VALUE)
    {
        // Handle error: device not found or I2C communication issue
        return;
    }

    // Step 3: Set accelerometer, gyroscope, and magnetometer scales
    Set_Accelerometer_Scale(mpu_data, ACCEL_SCALE_4G);  // Example: 2g scale
    Set_Gyroscope_Scale(mpu_data, GYRO_SCALE_250DPS);  // Example: 250 degrees per second scale

    // Step 4: Configure bandwidths for accelerometer, gyroscope, and magnetometer
    Set_Accelerometer_Bandwidth(ACC_BW_41Hz);  // Example: 44Hz bandwidth
    Set_Gyroscope_Bandwidth(GYRO_BW_20Hz);  // Example: 20Hz bandwidth

    // Step 5: Set sample rate (adjust based on application, e.g., 500Hz)
    Set_Sample_Rate(100);

    MPU9250_INT_PIN_CFG_EN_BYPASS(mpu_data);
    MPU9250_Init_Magnetometer(mpu_data);

    // Optional: Additional configurations based on specific drone requirements
    // For example, enabling interrupts, configuring DLPF settings, etc.

    is_mpu9250_init = 1;


    MPU9250_set_accel_offset(mpu_data);
    MPU9250_set_gyro_offset(mpu_data);

}


// Function to initialize the magnetometer
void MPU9250_Init_Magnetometer(MPU9250_Data *mpu_data)
{
    uint8_t data;

    // Step 1: Power up the magnetometer by writing 0x01 to AK8963_CNTL register
    data = 0x00;  // Power up the magnetometer
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_CNTL, data);

    Set_Magnetometer_Scale(mpu_data, MAG_SCALE_16BIT);  // Example: 16-bit mode for magnetometer
    Set_Magnetometer_Bandwidth(MAG_BW_10Hz);  // Example: 10Hz bandwidth

    // Step 2: Set the magnetometer to continuous measurement mode (0x02)
    Set_Magnetometer_Mode(MAG_MODE_CONTINUOUS);


    // Step 4: Verify magnetometer initialization by reading the WHO_AM_I register (expected value is 0x48)
    data = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_WHO_AM_I);
    if (data != AK8963_WHO_AM_I_VALUE)
    {
        // Handle error: magnetometer not found or communication issue
        return;
    }
}

void MPU9250_INT_PIN_CFG_EN_BYPASS(MPU9250_Data *mpu_data)
{
    uint8_t current_value = (uint8_t)0;

    // Read the current value from the ACCEL_CONFIG register
    current_value = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_INT_PIN_CFG);

    // Set the ACCEL_FS_SEL bits with the new scale value
    current_value |= (1 << 1);

    // Write the new value back to the ACCEL_CONFIG register
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_INT_PIN_CFG, current_value);
}



// Function to set the accelerometer scale factor using enum
void Set_Accelerometer_Scale(MPU9250_Data *mpu_data, Accel_Scale_t scale)
{
    uint8_t data = (uint8_t)scale;
    uint8_t current_value = (uint8_t)0;

    // Read the current value from the ACCEL_CONFIG register
    current_value = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_ACCEL_SCALE_CONFIG);

    // Clear the ACCEL_FS_SEL bits (bits 1-0) using the mask
    current_value &= ~ACCEL_FS_SEL_MASK;

    // Set the ACCEL_FS_SEL bits with the new scale value
    current_value |= (data & ACCEL_FS_SEL_MASK);

    // Write the new value back to the ACCEL_CONFIG register
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_ACCEL_SCALE_CONFIG, current_value);

    // Update the scale in mpu_data
    switch (scale)
    {
    case ACCEL_SCALE_2G:
        mpu_data->accel_scale = 16384.0f;  // LSB/g for 2g
        break;
    case ACCEL_SCALE_4G:
        mpu_data->accel_scale = 8192.0f;   // LSB/g for 4g
        break;
    case ACCEL_SCALE_8G:
        mpu_data->accel_scale = 4096.0f;   // LSB/g for 8g
        break;
    case ACCEL_SCALE_16G:
        mpu_data->accel_scale = 2048.0f;   // LSB/g for 16g
        break;
    default:
        mpu_data->accel_scale = 1.0f;      // Fallback default
        break;
    }
}


// Function to set the gyroscope scale factor using enum
void Set_Gyroscope_Scale(MPU9250_Data *mpu_data, Gyro_Scale_t scale)
{
    uint8_t data = (uint8_t)scale, current_value = 0;

    // Read the current value from the GYRO_CONFIG register
    current_value = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_GYRO_SCALE_CONFIG);

    // Clear the GYRO_FS_SEL bits (bits 1-0) using the mask
    current_value &= ~GYRO_FS_SEL_MASK;

    // Set the GYRO_FS_SEL bits with the new scale value
    current_value |= (data & GYRO_FS_SEL_MASK);

    // Write the new value back to the GYRO_CONFIG register
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_GYRO_SCALE_CONFIG, current_value);

    // Update the scale in mpu_data
    switch (scale)
    {
    case GYRO_SCALE_250DPS:
        mpu_data->gyro_scale = 131.0f;  // LSB/°/s for 250 DPS
        break;
    case GYRO_SCALE_500DPS:
        mpu_data->gyro_scale = 65.5f;   // LSB/°/s for 500 DPS
        break;
    case GYRO_SCALE_1000DPS:
        mpu_data->gyro_scale = 32.8f;   // LSB/°/s for 1000 DPS
        break;
    case GYRO_SCALE_2000DPS:
        mpu_data->gyro_scale = 16.4f;   // LSB/°/s for 2000 DPS
        break;
    default:
        mpu_data->gyro_scale = 1.0f;    // Fallback default
        break;
    }
}


// Function to set the magnetometer scale in AK8963_CNTL register
void Set_Magnetometer_Scale(MPU9250_Data *mpu_data, uint8_t scale)
{
    uint8_t current_value;

    // Read the current value from the AK8963_CNTL register
    current_value = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_CNTL);

    // Clear the scale bits (bits 4 and 5) using the mask
    current_value &= ~AK8963_SCALE_MASK;

    // Set the scale bits with the new scale value
    current_value |= (scale & AK8963_SCALE_MASK);

    // Write the new value back to the AK8963_CNTL register
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_CNTL, current_value);

    // Update the scale in mpu_data
    if (scale == MAG_SCALE_16BIT)
    {
        mpu_data->mag_scale = 0.15f;  // µT/LSB for 16-bit
    }
    else
    {
        mpu_data->mag_scale = 0.6f;   // µT/LSB for 14-bit
    }
}






// Function to set accelerometer bandwidth
void Set_Accelerometer_Bandwidth(ACC_Bandwidth_t bandwidth)
{
    uint8_t data = (uint8_t)bandwidth;
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_ACCEL_BW_CONFIG, data);  // Corrected register: ACCEL_CONFIG2
}

// Function to set gyroscope bandwidth
void Set_Gyroscope_Bandwidth(GYRO_Bandwidth_t bandwidth)
{
    uint8_t data = (uint8_t)bandwidth;
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_GYRO_BW_CONFIG, data);  // Correct register: CONFIG
}

// Function to set magnetometer bandwidth
void Set_Magnetometer_Bandwidth(uint8_t bandwidth)
{
    uint8_t current_value;

    // Read the current value of the AK8963_CNTL register
    current_value = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_CNTL);

    // Clear the Bandwidth bit (Bit 6) using bitwise AND with the inverse mask
    current_value &= ~AK8963_BW_MASK;

    // Set the new Bandwidth value by applying bitwise OR
    current_value |= (bandwidth << 6);  // Shift the bandwidth to the correct position

    // Write the modified value back to the register
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_CNTL, current_value);
}



// Function to set the magnetometer mode (e.g., continuous, single, or powerdown)
void Set_Magnetometer_Mode(uint8_t mode)
{
    uint8_t current_value;

    // Read the current value of the AK8963_CNTL register
    current_value = I2C_Read_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_CNTL);

    // Clear the Mode bits (Bits 4-3) using bitwise AND with the inverse mask (NOT operator)
    current_value &= ~AK8963_MODE_MASK;

    // Set the new Mode by applying the mask and using bitwise OR
    current_value |= (mode << 3);  // Shift the mode to the correct position

    // Write the modified value back to the register
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_CNTL, current_value);
}


// Function to calculate the SMPLRT_DIV based on desired sample rate
uint8_t Calculate_SMPLRT_DIV(uint16_t sample_rate)
{
    // Calculate SMPLRT_DIV
    return (uint8_t)((8000 / sample_rate) - 1);  // 8kHz clock rate
}

// Function to set the sample rate
void Set_Sample_Rate(uint16_t sample_rate)
{
    uint8_t smplrt_div = Calculate_SMPLRT_DIV(sample_rate);

    // Write the calculated value to the SMPLRT_DIV register
    I2C_Write_Memory(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_SMPLRT_DIV, smplrt_div);
}




// Read Accelerometer Data
void MPU9250_Read_Accelerometer(MPU9250_Data *mpu_data)
{
    if (is_mpu9250_init == 0)
        MPU9250_Init(mpu_data);

    uint8_t buffer[6];
    I2C_Read_Memory_Multiple(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_ACCEL_XOUT_H, buffer, 6);

    // Combine high and low bytes for each axis
    mpu_data->accel_x_raw = (int16_t)((buffer[0] << 8) | buffer[1]);
    mpu_data->accel_y_raw = (int16_t)((buffer[2] << 8) | buffer[3]);
    mpu_data->accel_z_raw = (int16_t)((buffer[4] << 8) | buffer[5]);
}

// Read Gyroscope Data
void MPU9250_Read_Gyroscope(MPU9250_Data *mpu_data)
{
    if (is_mpu9250_init == 0)
        MPU9250_Init(mpu_data);

    uint8_t buffer[6];
    I2C_Read_Memory_Multiple(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_GYRO_XOUT_H, buffer, 6);

    // Combine high and low bytes for each axis
    mpu_data->gyro_x_raw = (int16_t)((buffer[0] << 8) | buffer[1]);
    mpu_data->gyro_y_raw = (int16_t)((buffer[2] << 8) | buffer[3]);
    mpu_data->gyro_z_raw = (int16_t)((buffer[4] << 8) | buffer[5]);
}

// Function to read magnetometer data
void MPU9250_Read_Magnetometer(MPU9250_Data *mpu_data)
{
    if (is_mpu9250_init == 0)
        MPU9250_Init(mpu_data);

    uint8_t buffer[6];
    I2C_Read_Memory_Multiple(mpu9250_i2c_h, MPU9250_MAGNETOMETER_ADDRESS, AK8963_XOUT_L, buffer, 6);

    // Combine high and low bytes for each axis
    mpu_data->mag_x_raw = (int16_t)((buffer[0] << 8) | buffer[1]);
    mpu_data->mag_y_raw = (int16_t)((buffer[2] << 8) | buffer[3]);
    mpu_data->mag_z_raw = (int16_t)((buffer[4] << 8) | buffer[5]);
}

// Read Temperature from MPU
void MPU9250_Read_Temperature(MPU9250_Data *mpu_data)
{
    if (is_mpu9250_init == 0)
        MPU9250_Init(mpu_data);

    uint8_t buffer[2];
    I2C_Read_Memory_Multiple(mpu9250_i2c_h, MPU9250_ADDRESS, MPU9250_TEMP_OUT_H, buffer, 2);

    int16_t raw_temperature = (int16_t)((buffer[0] << 8) | buffer[1]);
    mpu_data->temp_raw = raw_temperature;
}

// Function to convert raw accelerometer data to physical units (m/s²)
void Convert_Accelerometer_To_Physical(MPU9250_Data *mpu_data)
{
    mpu_data->accel_x = (float)(((float)mpu_data->accel_x_raw * 9.81) / ((float)mpu_data->accel_scale)); // Convert to m/s²
    mpu_data->accel_y = (float)(((float)mpu_data->accel_y_raw * 9.81) / ((float)mpu_data->accel_scale));
    mpu_data->accel_z = (float)(((float)mpu_data->accel_z_raw * 9.81) / ((float)mpu_data->accel_scale));
}

// Function to convert raw gyroscope data to physical units (°/s)
void Convert_Gyroscope_To_Physical(MPU9250_Data *mpu_data)
{
    mpu_data->gyro_x = (float)(((float)mpu_data->gyro_x_raw) / (float)mpu_data->gyro_scale);  // Convert to °/s
    mpu_data->gyro_y = (float)(((float)mpu_data->gyro_y_raw) / (float)mpu_data->gyro_scale);
    mpu_data->gyro_z = (float)(((float)mpu_data->gyro_z_raw) / (float)mpu_data->gyro_scale);
}

// Function to convert raw magnetometer data to physical units (µT)
void Convert_Magnetometer_To_Physical(MPU9250_Data *mpu_data)
{
    mpu_data->mag_x = (float)(((float)mpu_data->mag_x_raw) / (float)mpu_data->mag_scale);  // Convert to µT
    mpu_data->mag_y = (float)(((float)mpu_data->mag_y_raw) / (float)mpu_data->mag_scale);
    mpu_data->mag_z = (float)(((float)mpu_data->mag_z_raw) / (float)mpu_data->mag_scale);
}


// Function to convert raw temperature data to physical units (°C)
void Convert_Temperature_To_Physical(MPU9250_Data *mpu_data)
{
    mpu_data->temperature = (float)(mpu_data->temp_raw / 340.0) + 36.53; // Convert to °C
}



void Apply_Offsets_accel(MPU9250_Data *mpu_data)
{
    // Apply calibration offsets to the raw sensor data
    mpu_data->accel_x_raw -= (int16_t)((float)mpu_data->accel_offset[0] * ((float)mpu_data->accel_scale / (float)16384));
    mpu_data->accel_y_raw -= (int16_t)((float)mpu_data->accel_offset[1] * ((float)mpu_data->accel_scale / (float)16384));
    mpu_data->accel_z_raw -= (int16_t)((float)mpu_data->accel_offset[2] * ((float)mpu_data->accel_scale / (float)16384));

    mpu_data->accel_z_raw += mpu_data->accel_scale;
}


void Apply_Offsets_gyro(MPU9250_Data *mpu_data)
{
    mpu_data->gyro_x_raw -= mpu_data->gyro_offset[0];
    mpu_data->gyro_y_raw -= mpu_data->gyro_offset[1];
    mpu_data->gyro_z_raw -= mpu_data->gyro_offset[2];
}


void Apply_Offsets(MPU9250_Data *mpu_data)
{
    Apply_Offsets_accel(mpu_data);
    Apply_Offsets_gyro(mpu_data);
    //Apply_Calibration_Offsets_mag(mpu_data);
}




void MPU9250_set_accel_offset(MPU9250_Data *mpu_data)
{
    mpu_data->accel_offset[0] = ACCEL_X_OFFSET;
    mpu_data->accel_offset[1] = ACCEL_Y_OFFSET;
    mpu_data->accel_offset[2] = ACCEL_Z_OFFSET;
}

void MPU9250_set_gyro_offset(MPU9250_Data *mpu_data)
{
    // Initialize the offsets to 0
    for (int i = 0; i < 3; i++)
    {
        mpu_data->gyro_offset[i] = 0.0f;
    }
    // Collect data for calibration (e.g., 100 samples)
    for (int i = 0; i < 100; i++)
    {
        // Read sensor data (accel, gyro, mag) from the actual sensor
        MPU9250_Read_Gyroscope(mpu_data);

        mpu_data->gyro_offset[0] += mpu_data->gyro_x_raw;
        mpu_data->gyro_offset[1] += mpu_data->gyro_y_raw;
        mpu_data->gyro_offset[2] += mpu_data->gyro_z_raw;

        Delay_ms(10);
    }
    // Average the offsets over the number of samples (100 in this case)
    for (int i = 0; i < 3; i++)
    {
        mpu_data->gyro_offset[i] /= 100.0f;

    }
}



void Read_MPU9250(MPU9250_Data *mpu_data)
{
    if (is_mpu9250_init == 0)
        MPU9250_Init(mpu_data);


    // Read the raw sensor data (accelerometer, gyroscope, and magnetometer)
    MPU9250_Read_Accelerometer(mpu_data);
    MPU9250_Read_Gyroscope(mpu_data);
    MPU9250_Read_Magnetometer(mpu_data);
    MPU9250_Read_Temperature(mpu_data);

    // Apply calibration offsets to the raw data
    Apply_Offsets(mpu_data);

    // Optionally, convert the raw data to physical units (e.g., Gs for accelerometer, deg/s for gyroscope)
    Convert_Accelerometer_To_Physical(mpu_data);
    Convert_Gyroscope_To_Physical(mpu_data);
    Convert_Magnetometer_To_Physical(mpu_data);
    Convert_Temperature_To_Physical(mpu_data);
}


void MPU9250_Get_Offset(MPU9250_Data *mpu_data)
{
    if (is_mpu9250_init == 0)
        MPU9250_Init(mpu_data);

    Set_Accelerometer_Scale(mpu_data, ACCEL_SCALE_2G);  // Example: 2g scale

    // Initialize the offsets to 0
    for (int i = 0; i < 3; i++) {
        mpu_data->accel_offset[i] = 0.0f;
        mpu_data->gyro_offset[i] = 0.0f;
        mpu_data->mag_offset[i] = 0.0f;
    }

    // Collect data for calibration (e.g., 100 samples)
    for (int i = 0; i < 100; i++) {
        // Read sensor data (accel, gyro, mag) from the actual sensor
        MPU9250_Read_Accelerometer(mpu_data);
        MPU9250_Read_Gyroscope(mpu_data);
        MPU9250_Read_Magnetometer(mpu_data);

        // Add the raw readings to the offset totals
        mpu_data->accel_offset[0] += mpu_data->accel_x_raw;
        mpu_data->accel_offset[1] += mpu_data->accel_y_raw;
        mpu_data->accel_offset[2] += mpu_data->accel_z_raw;

        mpu_data->gyro_offset[0] += mpu_data->gyro_x_raw;
        mpu_data->gyro_offset[1] += mpu_data->gyro_y_raw;
        mpu_data->gyro_offset[2] += mpu_data->gyro_z_raw;

        mpu_data->mag_offset[0] += mpu_data->mag_x_raw;
        mpu_data->mag_offset[1] += mpu_data->mag_y_raw;
        mpu_data->mag_offset[2] += mpu_data->mag_z_raw;

        Delay_ms(10);
    }

    // Average the offsets over the number of samples (100 in this case)
    for (int i = 0; i < 3; i++) {
        mpu_data->accel_offset[i] /= 100.0f;  // Averaging over 100 samples
        mpu_data->gyro_offset[i] /= 100.0f;
        mpu_data->mag_offset[i] /= 100.0f;
    }

}

