#include "led.h"


static uint32_t last_update_timestamp; 


//* _ STATIC VARIABLE DECLARATIONS _____________________________________________

static LED_t LEDS[] = {
    {
        .channel   = TCC1_CHANNEL_0, 
        .is_active = 0, 
        .duty      = 0, 
        .step      = 0, 
    }, 
    {
        .channel   = TCC1_CHANNEL_1, 
        .is_active = 0, 
        .duty      = 0, 
        .step      = 0, 
    }, 
}; 


void LED_init(void)
{
    LED_CHANNEL_t i; 
    
    // Reset data for each LEDs and start the TCC1 peripheral. 
    for (i = 0; i < LED_COUNT; i += 1)
        LED_cycle_stop(i); 

    TCC1_PWMStart(); 
    return;
}


void LED_task(void)
{
    LED_CHANNEL_t i; 
    
    if (SYSTICK_millis() - last_update_timestamp < LED_REFRESH_RATE_MS)
        return; 
    
    // Check for each LED channel is they are active and if they are, increment 
    // their duty cycle. 
    for (i = 0; i < LED_COUNT; i += 1)
    {
        if (LEDS[i].is_active)
        {
            LEDS[i].duty += LEDS[i].step; 
            TCC1_set_duty_cycle(LEDS[i].channel, (LEDS[i].duty % 100)); 
        }
    }
    
    last_update_timestamp = SYSTICK_millis(); 
}


void LED_cycle_start(LED_CHANNEL_t led_channel, uint32_t cycle_speed)
{
    // If the given LED doesn't exist, abort. 
    if (led_channel >= LED_COUNT)
        return; 
    
    // If the channel is already activated, abort. 
    if (LEDS[led_channel].is_active)
        return; 
    
    // Start the cycle for the given LED. 
    LEDS[led_channel].duty      = 0; 
    LEDS[led_channel].step      = cycle_speed; 
    LEDS[led_channel].is_active = true; 
}


void LED_cycle_stop(LED_CHANNEL_t led_channel)
{
    // If the given LED doesn't exist, abort. 
    if (led_channel >= LED_COUNT)
        return; 
    
    // If the channel is already turned off, abort. 
    if (!LEDS[led_channel].is_active)
        return; 
    
    // Stop the cycle for the given LED. 
    TCC1_set_duty_cycle(LEDS[led_channel].channel, 0); 
    LEDS[led_channel].duty      = 0; 
    LEDS[led_channel].step      = 0; 
    LEDS[led_channel].is_active = false; 
}
