#include "TM4C123GH6PM.h"
#include "UART.h"

/********************* Structure Defination*** ************************/
UART_config_type UART_config[] = 
{
  {
    .UART_Perif_Addr     = (UART0_Type*)UART0,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOA,
    .GPIO_Perif_Index    = 0,
    .RX_Pin              = 0,
    .TX_Pin              = 1,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART1,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOB,
    .GPIO_Perif_Index    = 1,
    .RX_Pin              = 0,
    .TX_Pin              = 1,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART2,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOD,
    .GPIO_Perif_Index    = 3,
    .RX_Pin              = 6,
    .TX_Pin              = 7,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART3,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOC,
    .GPIO_Perif_Index    = 2,
    .RX_Pin              = 6,
    .TX_Pin              = 7,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART4,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOC,
    .GPIO_Perif_Index    = 2,
    .RX_Pin              = 4,
    .TX_Pin              = 5,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART5,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOE,
    .GPIO_Perif_Index    = 4,
    .RX_Pin              = 4,
    .TX_Pin              = 5,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART6,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOD,
    .GPIO_Perif_Index    = 3,
    .RX_Pin              = 4,
    .TX_Pin              = 5,
    .GPIOPCTL_Index      = 1
  },
  {
    .UART_Perif_Addr     = (UART0_Type*)UART7,
    .GPIO_Perif_Addr     = (GPIOA_Type*)GPIOE,
    .GPIO_Perif_Index    = 4,
    .RX_Pin              = 0,
    .TX_Pin              = 1,
    .GPIOPCTL_Index      = 1
  }
};

/********************************************************************************/




/*********************************************************************************/

void UART_init(UART_h *handler, UART_Channel UART_Ch_Index, uint32_t UART_BaudRate)
{
    uint32_t UART_Index = (uint32_t)UART_Ch_Index;
    uint32_t BaudRate_Integer;
    float    BaudRate_Float ;

    SYSCTL->RCGCGPIO |= (1<<(UART_config[UART_Index].GPIO_Perif_Index));                            /* enable clock to GPIO PORT*/
    SYSCTL->RCGCUART |= (1<<(UART_Index));                                                          /* enable clock to UART */
    Delay(1);
    /* UART initialization */
    UART_config[UART_Index].UART_Perif_Addr->CTL = 0;                                               /* UART module disbable */
    
    BaudRate_Integer = ((SYS_CLOCK_HZ / 16) / UART_BaudRate);                                       /* (16000000/16)/UART_BaudRate */
    UART_config[UART_Index].UART_Perif_Addr->IBRD = BaudRate_Integer;                               /* for 9600 baud rate, integer = 104 */
    
    BaudRate_Float     = ((float)(SYS_CLOCK_HZ / 16)/(float)UART_BaudRate) - (float)BaudRate_Integer;
    BaudRate_Integer   = (uint32_t)((BaudRate_Float*(float)64.0) + (float)0.5);
    UART_config[UART_Index].UART_Perif_Addr->FBRD = BaudRate_Integer;                             /* for 9600 baud rate, fractional = 11*/
    
    UART_config[UART_Index].UART_Perif_Addr->CC     = 0;                                            /*select system clock*/
    UART_config[UART_Index].UART_Perif_Addr->LCRH   = UART_LCRH_WLEN_8BITS;                         /* data lenght 8-bit, not parity bit, no FIFO */
    UART_config[UART_Index].UART_Perif_Addr->CTL    = UART_CTL_ENABLE;                              /* Enable UART module, Rx and Tx */

    /* Configure them digital and enable alternate function */
    UART_config[UART_Index].GPIO_Perif_Addr->DEN   |=   ((1<<(UART_config[UART_Index].RX_Pin)) |
                                                        (1<<(UART_config[UART_Index].TX_Pin)));    /* set PE4 and PE5 as digital */

    UART_config[UART_Index].GPIO_Perif_Addr->AFSEL |=   ((1<<(UART_config[UART_Index].RX_Pin)) |
                                                        (1<<(UART_config[UART_Index].TX_Pin)));    /* Use PE4,PE5 alternate function */

    UART_config[UART_Index].GPIO_Perif_Addr->AMSEL &=   ~((1<<(UART_config[UART_Index].RX_Pin)) |
                                                        (1<<(UART_config[UART_Index].TX_Pin)));    /* Turn off analg function*/

    UART_config[UART_Index].GPIO_Perif_Addr->PCTL  |=   ((UART_config[UART_Index].GPIOPCTL_Index << ((UART_config[UART_Index].RX_Pin)*4)) |
                                                        (UART_config[UART_Index].GPIOPCTL_Index << ((UART_config[UART_Index].TX_Pin)*4)));      /* configure PE4 and PE5 for UART */
	
    handler->uart_ch_idx    = UART_Ch_Index;
    handler->uart_baudrate  = UART_BaudRate;
    handler->uart_init      = 1;
}

/*********************************************************************************/
void uart_rx_irq_enable(UART_h handler)
{
    uint32_t uart_index = (uint32_t)handler.uart_ch_idx;

    /* Enable the UART receive interrupt */
    UART_config[uart_index].UART_Perif_Addr->IM |= 0x10;  /* Enable RX interrupt */

    /* Enable the NVIC interrupt for the corresponding UART channel */
    switch (uart_index)
    {
        case 0:
            NVIC_EnableIRQ(UART0_IRQn);
            NVIC_SetPriority(UART0_IRQn, 5);
            break;
        case 1:
            NVIC_EnableIRQ(UART1_IRQn);
            NVIC_SetPriority(UART1_IRQn, 5);
            break;
        case 2:
            NVIC_EnableIRQ(UART2_IRQn);
            NVIC_SetPriority(UART2_IRQn, 5);
            break;
        case 3:
            NVIC_EnableIRQ(UART3_IRQn);
            NVIC_SetPriority(UART3_IRQn, 5);
            break;
        case 4:
            NVIC_EnableIRQ(UART4_IRQn);
            NVIC_SetPriority(UART4_IRQn, 5);
            break;
        case 5:
            NVIC_EnableIRQ(UART5_IRQn);
            NVIC_SetPriority(UART5_IRQn, 5);
            break;
        case 6:
            NVIC_EnableIRQ(UART6_IRQn);
            NVIC_SetPriority(UART6_IRQn, 5);
            break;
        case 7:
            NVIC_EnableIRQ(UART7_IRQn);
            NVIC_SetPriority(UART0_IRQn, 5);
            break;
        default: break;
    }
}

/*********************************************************************************/
// IRQ Handler for UART0 to receive data and send it to the queue
void UART0_IRQHandler(void)
{
    if (UART_config[UART_0].UART_Perif_Addr->MIS & 0x10) /* Check RX interrupt status */
    {
        uint8_t rx_data = UART_config[UART_0].UART_Perif_Addr->DR; /* Read received data */

        UART_config[UART_0].UART_Perif_Addr->ICR |= 0x10;          /* Clear interrupt flag */

        UART0_IRQHandler_User(rx_data);
    }
}

void UART0_IRQHandler_temp(uint8_t data)
{
    ;
}

/*********************************************************************************/
void UART_print_int(UART_h handler, uint32_t data)
{
    uint8_t str[5] = {[0 ... 4] = '\0'};

    sprintf(str,"%d",data);

    UART_print(handler, str);
}

/*********************************************************************************/
void UART_print_float(UART_h handler, float data)
{
    uint8_t str[5] = {[0 ... 4] = '\0'};

    sprintf(str,"%0.2f",data);

    UART_print(handler, str);
}

/*********************************************************************************/
void UART_print_ch(UART_h handler, uint8_t data)
{
    uint8_t str[2] = {'\0', '\0'};
    str[0] = data;

    UART_print(handler, str);
}

/*********************************************************************************/
void UART_print(UART_h handler, uint8_t *data)
{

     while(*data)
     {
        UART_Transmit(handler,(uint8_t)*(data++));
     }
}

/*********************************************************************************/
void UART_println(UART_h handler, uint8_t *data)
{

     while(*data)
     {
        UART_Transmit(handler,(uint8_t)*(data++));
     }
     UART_Transmit(handler,'\n');
     UART_Transmit(handler,'\r');
}


/*********************************************************************************/
void UART_Transmit(UART_h handler,uint8_t data)
{
    while((UART_config[handler.uart_ch_idx].UART_Perif_Addr->FR & 0x20) != 0);           /* wait until Tx buffer not full */
    UART_config[handler.uart_ch_idx].UART_Perif_Addr->DR = data;                         /* before giving it another byte */
}

/*********************************************************************************/
void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter; i++);
}


/*********************************************************************************/
uint8_t UART_Receive(UART_h handler)
{
    uint8_t rx_data = 0xff;

    /* Wait until there is data available to receive */
    while(!UART_RxReady(handler));

    /* Read the received data */
    rx_data = UART_config[handler.uart_ch_idx].UART_Perif_Addr->DR;

    return rx_data;
}


/*********************************************************************************/
uint8_t UART_RxReady(UART_h handler)
{
    return ((UART_config[(uint32_t)handler.uart_ch_idx].UART_Perif_Addr->FR & UART_FR_RXFE) == 0);
}
