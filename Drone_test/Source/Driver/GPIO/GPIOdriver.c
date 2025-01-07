#include "GPIOdriver.h"
#include "TM4C123GH6PM.h"
#include "Common_Const.h"

GPIOA_Type* const PORT[6] = { GPIOA , GPIOB , GPIOC , GPIOD , GPIOE , GPIOF };

static uint8_t Port_IO_flag[6] = {(uint8_t)0};
static uint8_t GPIO_Init_flag  = (uint8_t)0;


/************************************* <THIS FUNCTION IS USE TO INITIALIZE DIGITAL INPUT/OUTPUT PORT> ************************************/
void GPIO_Init(uint8_t port_num)
{ 

    SYSCTL->RCGCGPIO |= (1 << port_num);
	
    PORT[port_num]->LOCK = 0x4C4F434B;

    GPIO_Init_flag |= (uint8_t)(1 << port_num);
}

/************************************************ <THIS FUNCTION IS USE FOR DIGITAL WRITE> **********************************************/
void DigitalWrite(GPIO_PORT port_x , GPIO_HI_LO value)
{
 //   mutex_lock(&lock);


	uint8_t port_num = ((port_x)/(uint8_t)10) ;
	uint8_t pin_num  = ((port_x)%(uint8_t)10) ;
        
    uint8_t IO_flag = (Port_IO_flag[port_num] & (1<<(pin_num)));
            IO_flag = (IO_flag >> pin_num);
	

	if(IO_flag == 0) 
	{
	    if((GPIO_Init_flag & (1 << port_num)) == 0)
	    {
	        GPIO_Init(port_num);
	    }

		PORT[port_num]->CR     |=  (1<<(pin_num));
	//	PORT[port_num]->AFSEL  &= ~(1<<(pin_num));
		PORT[port_num]->DIR    |=  (1<<(pin_num));
	    PORT[port_num]->DEN    |=  (1<<(pin_num));
	    
	    Port_IO_flag[port_num] |= (1<<(pin_num));
	}	
			
	else
	{
        ;
	} 
			
	if(value == HIGH)   
	{
        PORT[port_num]->DATA  |=  (1<<(pin_num));
	}
    		
    else
    {
    	PORT[port_num]->DATA  &=  ~(1<<(pin_num));
	}

//	mutex_unlock(&lock);
}


/************************************************ <THIS FUNCTION IS USE FOR DIGITAL READ> **********************************************/
GPIO_HI_LO DigitalRead(GPIO_PORT port_x , GPIO_PU_PD state)
{
		
	uint8_t port_num = ((port_x) / (uint8_t)10) ;
	uint8_t pin_num  = ((port_x) % (uint8_t)10) ;

    uint8_t IO_flag = (Port_IO_flag[port_num] & (1<<(pin_num)));
            IO_flag = (IO_flag >> pin_num);
	
    volatile uint8_t Input_Data = 0U;
	

	if(IO_flag == 0)
	{
	    if((GPIO_Init_flag & (1 << port_num)) == 0)
	    {
	        GPIO_Init(port_num);
	    }

		PORT[port_num]->CR     |=  (1<<(pin_num));
	//	PORT[port_num]->AFSEL  &= ~(1<<(pin_num));				
		PORT[port_num]->DIR    &= ~(1<<(pin_num));
	    PORT[port_num]->DEN    |=  (1<<(pin_num));
	    		
	    Port_IO_flag[port_num] |= ((uint8_t)1<<(pin_num));


	    if( state == PU )
	    {
	        PORT[port_num]->PUR   |=  (1<<(pin_num));
	    }

	    else
	    {
	        PORT[port_num]->PDR   |=  (1<<(pin_num));
	    }
    }


			 	
    Input_Data  = PORT[port_num]->DATA;      
	Input_Data &= (1 << (pin_num));
	Input_Data  = (Input_Data >> (pin_num));
                      
return(Input_Data);
    		
}
