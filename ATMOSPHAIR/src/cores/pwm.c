#include "pwm.h"


void TCC1_set_duty_cycle(TCC1_CHANNEL_t channel, float duty)
{
    // Check if the given channel exist then change the duty cycle. 
    if (channel >= TCC1_CHANNEL_COUNT)
        return; 
        
    TCC1_REGS->TCC_CC[channel] = (uint32_t)((duty / 100.0) * TCC1_PERIOD_VALUE);
}