/*
 * SPI_Interface.h
 *
 *  Created on: 18-Jul-2021
 *      Author: saksham
 */

/******************************************************************************************************************************/

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

#include "Common_Datatype.h"

extern void UART_init(UART_h *handler, UART_Channel UART_Ch_Index, uint32_t UART_BaudRate);
extern void UART_print(UART_h handler, uint8_t *data);
extern void UART_println(UART_h handler, uint8_t *data);
extern void UART_print_int(UART_h handler, uint32_t data);
extern void UART_print_float(UART_h handler, float data);
extern void UART_print_ch(UART_h handler, uint8_t data);
extern void UART_Transmit(UART_h handler, uint8_t data);

extern uint8_t UART_Receive(UART_h handler);
extern uint32_t UART_Receive_Long();

extern void uart_rx_irq_enable(UART_h handler);;


#endif /* DRIVER_SPI_SPI_H_ */
