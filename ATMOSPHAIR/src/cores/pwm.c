#include "pwm.h"


void TCC1_set_duty_cycle(TCC1_CHANNEL_t channel, float duty)
{    
    if (channel >= TCC1_CHANNEL_COUNT)
        return; 
        
    TCC1_REGS->TCC_CC[channel] = (uint32_t)((duty / 100.0) * TCC1_PERIOD_VALUE);
}