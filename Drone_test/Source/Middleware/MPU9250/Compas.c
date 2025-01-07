#include <math.h>  // Include math.h for atan2 function
#include "MPU9250_Interface.h"

#define PI 3.14159265358979323846  // Define the constant PI

// Function to calculate compass heading (in degrees)
float Calculate_Compass_Heading()
{
    // Declare the data structure to hold MPU9250 data
    MPU9250_Data mpu_data;

    // Initialize the MPU9250 sensor (including reading the magnetometer data)
    MPU9250_Read_Magnetometer(&mpu_data);

    // Apply calibration offsets (if any)
    float mag_x = mpu_data.mag_x_raw - mpu_data.mag_offset[0];
    float mag_y = mpu_data.mag_y_raw - mpu_data.mag_offset[1];

    // Calculate heading using the arctangent function
    float heading = atan2(mag_y, mag_x);  // atan2 returns the angle in radians

    // Convert radians to degrees
    heading = heading * (180.0 / PI);

    // Normalize heading to the range [0, 360)
    if (heading < 0)
    {
        heading += 360.0f;
    }

    // Return the compass heading in degrees
    return heading;
}
