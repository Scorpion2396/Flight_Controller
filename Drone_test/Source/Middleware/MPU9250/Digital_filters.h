/*
 * Digital_filters.h
 *
 *  Created on: 07-Jan-2025
 *      Author: Saksham Raj
 */

#ifndef SOURCE_MIDDLEWARE_MPU9250_DIGITAL_FILTERS_H_
#define SOURCE_MIDDLEWARE_MPU9250_DIGITAL_FILTERS_H_

#include "Common_Datatype.h"
#include "MPU9250_Interface.h"


void apply_high_pass_filter(MPU9250_Data *mpu_data, float dt);
void apply_low_pass_filter(MPU9250_Data *mpu_data, float dt);



#endif /* SOURCE_MIDDLEWARE_MPU9250_DIGITAL_FILTERS_H_ */
