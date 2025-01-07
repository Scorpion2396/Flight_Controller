#ifndef I2C_INTERFACE_H
#define I2C_INTERFACE_H

#include "TM4C123GH6PM.h"
#include "Common_Datatype.h"

extern void I2C_Init(I2C_h *handler, I2C_Channel I2C_Ch_Index, uint32_t I2C_Freq_KHz);
extern void I2C_Write(I2C_h handler, uint8_t slave_address , uint8_t data );
extern void I2C_Write_Memory(I2C_h handler, uint8_t slave_address , uint8_t slave_memory_address , uint8_t data );
void I2C_Write_Memory_Multiple(I2C_h handler, uint8_t slave_address , uint8_t slave_memory_address , uint8_t* data , uint32_t num_of_data);

extern int I2C_Read(I2C_h handler, uint8_t slave_address);
extern int I2C_Read_Memory(I2C_h handler, uint8_t slave_address , uint8_t slave_memory_address);
extern void I2C_Read_Multiple(I2C_h handler, uint8_t slave_address , uint8_t *data , uint32_t num_of_data);
extern void I2C_Read_Memory_Multiple(I2C_h handler, uint8_t slave_address , uint8_t slave_memory_address , uint8_t *data , uint32_t num_of_data);

extern void I2C_Begin(uint8_t slave_address);
extern void I2C_Transmit(uint8_t data);
extern void I2C_End();

#endif
