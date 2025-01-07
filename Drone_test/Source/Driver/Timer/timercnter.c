#include"timercnter.h"
#include "TM4C123GH6PM.h"

uint8_t is_timer_init = 0;

void init_Timer_Counter(void)
{

    // 1. Enable clock for WTIMER0
    SYSCTL->RCGCWTIMER |= (1U << 0); // Enable clock for WTIMER0
    __asm("NOP"); // Allow clock to stabilize
    __asm("NOP");
    __asm("NOP");

    // 2. Disable WTIMER0 before configuration
    WTIMER0->CTL &= ~(1U << 0); // Clear TAEN bit to disable Timer A

    // 3. Configure WTIMER0 for 64-bit mode
    WTIMER0->CFG = 0x0; // 64-bit timer configuration

    // 4. Configure Timer A for periodic mode and count up
    WTIMER0->TAMR = (0x2 << 0); // TAMR = 0x2 for periodic timer mode
    WTIMER0->TAMR |= (1U << 4); // Set TACDIR to count up

    // 5. Set the prescaler for 1 µs tick
    // Assuming a system clock of 16 MHz
    WTIMER0->TBPR = 15; // Prescaler for Timer B
    WTIMER0->TAPR = 15; // Prescaler for Timer A

    // 6. Load the start value into the GPTM Timer Interval Load Registers
    // Write TBILR first, then TAILR
    WTIMER0->TBILR = 0xFFFFFFFF; // Upper 32 bits
    WTIMER0->TAILR = 0xFFFFFFFF; // Lower 32 bits

    // 7. Clear any pending interrupts
    WTIMER0->ICR = (1U << 0); // Clear Timer A timeout interrupt flag

    // 8. Enable Timer A
    WTIMER0->CTL |= (1U << 0); // Set TAEN bit to enable Timer A

    is_timer_init = 1;
}


/************************************************************ <Start Timer-A-0 > *************************************************************/
void Start_Timer_counter(void)
{
    if(is_timer_init == 0)
    {
        init_Timer_Counter();
    }
    WTIMER0->CTL      |= (1<<0);                         // Enable Timer A
}



/************************************************************ <Start Timer-A-0 > *************************************************************/
void Stop_Timer_counter(void)
{
    WTIMER0->CTL      &= ~(1<<0);                         // Disabling Timer A
}



/************************************************************ <Refresh Timer-A-0 > *************************************************************/
void Refresh_Timer_counter(void)
{
    WTIMER0->CTL      &= ~(1<<0);                         // Disabling Timer A
    WTIMER0->CTL      |= (1<<0);                         // Enable Timer A
}

/************************************************************ <Refresh Timer-A-0 > *************************************************************/
unsigned int millis(void)
{
    uint64_t tick = micros();

    tick = tick/1000;

    return ((uint32_t)tick);
}

uint64_t micros(void)
{
    if(is_timer_init == 0)
    {
        init_Timer_Counter();
        Start_Timer_counter();
    }

    // Atomic read of the 64-bit timer value
    uint32_t low, high, high_check;
    do {
        high = WTIMER0->TBR; // Upper 32 bits
        low = WTIMER0->TAR;  // Lower 32 bits
        high_check = WTIMER0->TBR;
    } while (high != high_check);

    // Combine high and low into a single 64-bit value
    uint64_t ticks = ((uint64_t)high << 32) | low;
    return ticks/16; // Each tick corresponds to 1 µs
}


void Delay_us(uint32_t delay)
{
    uint64_t start_ticks = micros();  // Get the current timer value in microseconds
    uint64_t target_ticks = start_ticks + delay;  // Calculate the target time (start + delay)

    // Wait until the target time is reached
    while (micros() < target_ticks)
    {
        // Wait for the timer to reach the target time
    }
}

void Delay_ms(uint32_t delay)
{
    uint64_t start_ticks = millis();  // Get the current timer value in microseconds
    uint64_t target_ticks = start_ticks + delay;  // Calculate the target time (start + delay)

    // Wait until the target time is reached
    while (millis() < target_ticks)
    {
        // Wait for the timer to reach the target time
    }
}


