/*
 * HCSR04_Interface.h
 *
 *  Created on: 21-Dec-2024
 *      Author: Saksham Raj
 */

#ifndef SOURCE_INTERFACE_HCSR04_INTERFACE_H_
#define SOURCE_INTERFACE_HCSR04_INTERFACE_H_


#include "Common_Datatype.h"
#include "Timer_Interface.h"
#include "timercnter_interface.h"



extern void HCSR04_init(GPIO_PORT trigger, GPIO_PORT echo);
extern uint32_t HCSR04_get_distance();


#endif /* SOURCE_INTERFACE_HCSR04_INTERFACE_H_ */
