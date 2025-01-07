/*
 * Complementary_filter.h
 *
 *  Created on: 29-Dec-2024
 *      Author: Saksham Raj
 */

#ifndef SOURCE_MIDDLEWARE_MPU9250_COMPLEMENTARY_FILTER_H_
#define SOURCE_MIDDLEWARE_MPU9250_COMPLEMENTARY_FILTER_H_

void Complementary_Filter(MPU9250_Data *mpu_data, float dt);
void apply_low_pass_filter(MPU9250_Data *mpu_data, float dt);
void apply_high_pass_filter(MPU9250_Data *mpu_data, float dt);
float low_pass_filter(float previous_value, float current_value, float alpha);



#endif /* SOURCE_MIDDLEWARE_MPU9250_COMPLEMENTARY_FILTER_H_ */
