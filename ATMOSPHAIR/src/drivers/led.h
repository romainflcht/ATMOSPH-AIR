#ifndef _LED_H_
#define _LED_H_

//* _ INCLUDES _________________________________________________________________

#include <stdlib.h>
#include "definitions.h" 

#include "../cores/pwm.h"
#include "../cores/systick.h"


//* _ DEFINITIONS ______________________________________________________________

#define SLOW_CYCLE_SPEED      1
#define MEDIUM_CYCLE_SPEED    5
#define FAST_CYCLE_SPEED      15

#define LED_REFRESH_RATE_MS   35


//* _ ENUMERATION DEFINITIONS __________________________________________________

typedef enum led_state
{
    LED_IDLE, 
}   LED_STATE_t;

typedef enum led_channel
{
    LED_GREEN, 
    LED_RED, 
    LED_COUNT, 
}   LED_CHANNEL_t;


//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef struct led
{
    TCC1_CHANNEL_t  channel; 
    bool            is_active; 
    uint32_t        step; 
    uint32_t        duty; 
}   LED_t;


//* _ FUNCTION DECLARATIONS ____________________________________________________

void LED_init(void); 

void LED_task(void); 

void LED_cycle_start(LED_CHANNEL_t led_channel, uint32_t cycle_speed); 

void LED_cycle_stop(LED_CHANNEL_t led_channel); 

#endif 