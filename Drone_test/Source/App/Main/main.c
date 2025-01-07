#include "main.h"
#include "MPU9250_Interface.h"
#include <stdio.h>

uint8_t str[100] = {[0 ... 99] = '\0'};

UART_h uart0_h;
MPU9250_Data mpu_data;

// Timing variables for dt (delta time)
uint32_t last_time = 0;
uint32_t current_time = 0;
float dt = 0.0f;

#define __IF_KELMAN_FILTER__

void main()
{
    DigitalWrite(PF2,HIGH);

    UART_init(&uart0_h, UART_0, 115200);

    MPU9250_Init(&mpu_data);

    while (1)
    {
//        Timer_scheduler();

//        if(Flag_20ms)
        {
            /* Read the sensor data (accelerometer and gyroscope) */
            Read_MPU9250(&mpu_data);
            //MPU9250_Get_Offset(&mpu_data);

            /* Get the current time in milliseconds */
            current_time = millis();  // Use your platform's time function
            dt = (float)(current_time - last_time) / 1000.0f;  // Convert to seconds

            //calculate_velocity(&mpu_data, dt);
//            sprintf(str, "%0.2f,%0.2f,%0.2f\r\n", mpu_data.accel_x, mpu_data.accel_y, mpu_data.accel_z);
//            UART_print(uart0_h, str);
            //sprintf(str, "%d,%d,%d\r\n", mpu_data.accel_x_raw, mpu_data.accel_y_raw, mpu_data.accel_z_raw);
            // sprintf(str, "%d,%d,%d\r\n", mpu_data.accel_offset[0], mpu_data.accel_offset[1], mpu_data.accel_offset[2]);
            //UART_print(uart0_h, str);

#ifdef __IF_KELMAN_FILTER__
            /* Apply Kalman filter */
//            Kalman_Filter_Angles(&mpu_data, dt);

            EKF_Filter_Angles(&mpu_data, dt);

#else
            /* Apply complementary filter */
            Complementary_Filter(&mpu_data, dt);
#endif



            // Print the angles
            sprintf(str, "%0.2f,%0.2f\r\n", mpu_data.angle_roll, mpu_data.angle_pitch);
//           sprintf(str, "%0.2f,%0.2f,%0.2f,%0.2f\r\n", mpu_data.quaternion[0], mpu_data.quaternion[1], mpu_data.quaternion[2], mpu_data.quaternion[3]);
            UART_print(uart0_h, str);

        /* Update the last time */
            last_time = current_time;

        }
    }
}
