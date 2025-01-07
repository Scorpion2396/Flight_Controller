#ifndef GPIOdriver_H
#define GPIOdriver_H

#include "Common_Datatype.h"

void GPIO_Init(uint8_t port_num);
void DigitalWrite(GPIO_PORT port_x , GPIO_HI_LO value);
GPIO_HI_LO DigitalRead(GPIO_PORT port_x , GPIO_PU_PD state);

#endif

