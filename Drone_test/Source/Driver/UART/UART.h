#ifndef _UART_
#define _UART_

#include "Common_Datatype.h"
#include "bsp.h"

#include "FreeRTOS.h"      // FreeRTOS kernel
#include "task.h"          // Task management and portYIELD_FROM_ISR
#include "semphr.h"        // Semaphore-related APIs

#define UART_FR_RXFE            (1 << 4)
#define UART_LCRH_WLEN_8BITS    0x60
#define UART_CTL_ENABLE         0x301

void UART_init(UART_h *handler, UART_Channel UART_Ch_Index, uint32_t UART_BaudRate);
void UART_print(UART_h handler, uint8_t *data);
void UART_println(UART_h handler, uint8_t *data);
void UART_print_int(UART_h handler, uint32_t data);
void UART_print_float(UART_h handler, float data);
void UART_print_ch(UART_h handler, uint8_t data);

void UART_Transmit(UART_h handler, uint8_t data);

uint8_t UART_Receive(UART_h handler);
uint32_t UART_Receive_Long();
uint8_t UART_RxReady(UART_h handler);

void uart_rx_irq_enable(UART_h handler);

void UART0_IRQHandler();

void Delay(unsigned long counter);

void UART0_IRQHandler_temp(uint8_t data);
void UART0_IRQHandler_User(uint8_t data)   __attribute__ ((weak, alias("UART0_IRQHandler_temp")));



#endif
