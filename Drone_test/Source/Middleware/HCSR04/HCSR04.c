/*
 * HCSR04.c
 *
 *  Created on: 21-Dec-2024
 *      Author: Saksham Raj
 */

#include "HCSR04.h"

GPIO_PORT Trigger;
GPIO_PORT Echo;

void HCSR04_init(GPIO_PORT trigger, GPIO_PORT echo)
{
    Trigger = trigger;
    Echo    = echo;

    // Initialize GPIO pins for trigger and echo
    DigitalWrite(trigger, LOW);    // Ensure trigger is low initially

    // Initialize the Timer for pulse width measurement
    init_Timer_Counter();
}

uint32_t HCSR04_get_distance()
{
    uint64_t pulse_duration1 = 0,pulse_duration2 = 0;
    uint32_t pulse_duration = 0, distance = 0;
    GPIO_HI_LO echo_read = LOW;
    uint8_t temp = 0;

    // Send a 10us pulse to trigger the sensor
    DigitalWrite(Trigger, HIGH);  // Set the trigger pin high
    Delay_us(10);                            // Wait for 10ms to generate the pulse
    DigitalWrite(Trigger, LOW);   // Set the trigger pin low




    while (DigitalRead(Echo, PD) == LOW);  // Wait for rising edge of echo

    // Get the pulse duration from the timer (in microseconds)
    pulse_duration1 = micros();  // Get the timer value, which represents pulse width in microseconds

    // Wait for the echo pin to go low (end of pulse)
    while (DigitalRead(Echo, PD) == HIGH);  // Wait for falling edge of echo

    // Get the pulse duration from the timer (in microseconds)
    pulse_duration2 = micros();  // Get the timer value, which represents pulse width in microseconds

    // Calculate the distance in centimeters (distance = (pulse_duration / 2) / 29.1)
    // The speed of sound is approximately 343 m/s or 29.1 microseconds per cm.
    pulse_duration = (uint32_t)(pulse_duration2 - pulse_duration1);
    distance = (((pulse_duration / 2) * 10) / 291);

    return distance;  // Return the distance in centimeters
}

