/*
 * HCSR04.h
 *
 *  Created on: 21-Dec-2024
 *      Author: Saksham Raj
 */

#ifndef SOURCE_MIDDLEWARE_HCSR04_HCSR04_H_
#define SOURCE_MIDDLEWARE_HCSR04_HCSR04_H_

#include "Common_Datatype.h"
#include "Timer_Interface.h"
#include "timercnter_interface.h"



void HCSR04_init(GPIO_PORT trigger, GPIO_PORT echo);
uint32_t HCSR04_get_distance();


#endif /* SOURCE_MIDDLEWARE_HCSR04_HCSR04_H_ */
