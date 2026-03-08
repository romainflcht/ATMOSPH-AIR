#ifndef _PWM_H_
#define _PWM_H_

//* _ INCLUDES _________________________________________________________________

#include <stdlib.h>
#include "definitions.h" 


//* _ DEFINITIONS ______________________________________________________________

#define TCC1_PERIOD_VALUE 4799
#define DUTY_CYCLE_FULL   100
#define DUTY_CYCLE_HALF   50
#define DUTY_CYCLE_OFF    0


//* _ ENUMERATIONS _____________________________________________________________

typedef enum tcc1_channel
{
    TCC1_CHANNEL_0,     
    TCC1_CHANNEL_1, 
    TCC1_CHANNEL_COUNT, 
}   TCC1_CHANNEL_t;


//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn void TCC1_set_duty_cycle(TCC1_CHANNEL_t channel, float duty); 
/// @brief change the duty cycle of the given TCC1 channel. 
/// @param channel we want to change the duty cycle. 
/// @param duty cycle we want on percent (0 to 100). 
void TCC1_set_duty_cycle(TCC1_CHANNEL_t channel, float duty); 

#endif