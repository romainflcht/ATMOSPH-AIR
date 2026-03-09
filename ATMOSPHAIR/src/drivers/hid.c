#include "hid.h"


//* _ GLOBAL VARIABLE DECLARATIONS _____________________________________________

HID_EVENT_t scroll_hid   = {0}; 
HID_EVENT_t interact_hid = {0}; 


//* _ STATIC VARIABLE DECLARATION ______________________________________________

static INTERRUPT_STATUS_t scroll_status   = {0}; 
static INTERRUPT_STATUS_t interact_status = {0}; 


//* _ STATIC FUNCTION DECLARATIONS _____________________________________________

static void scroll_button_callback(uintptr_t context); 
static void interact_button_callback(uintptr_t context); 
static void button_action(HID_BUTTON_t trigger); 


void HID_init(void)
{
    // Register callback for each user button and enable interrupt. 
    EIC_CallbackRegister(SCROLL_BUTTON, scroll_button_callback, (uintptr_t)NULL); 
    EIC_CallbackRegister(INTERACT_BUTTON, interact_button_callback, (uintptr_t)NULL); 
    
    EIC_InterruptEnable(SCROLL_BUTTON); 
    EIC_InterruptEnable(INTERACT_BUTTON); 
}


static void scroll_button_callback(uintptr_t context)
{
    button_action(SCROLL_BUTTON); 
    return; 
}


static void interact_button_callback(uintptr_t context)
{
    button_action(INTERACT_BUTTON); 
    return; 
}


static void button_action(HID_BUTTON_t trigger)
{
    bool                is_pressed; 
    uint32_t            timestamp; 
    INTERRUPT_STATUS_t* status; 
    HID_EVENT_t*        hid; 
    
    // Get the timestamp of the button event. 
    timestamp = SYSTICK_millis(); 
    
    // Check which button triggered an interrupt and loads the interrupt status 
    // and hid struct accordingly to save the button state (pressed or 
    // released). 
    switch (trigger)
    {
        case SCROLL_BUTTON: 
            is_pressed = HID_SCROLL_Get() == 0; 
            status     = &scroll_status; 
            hid        = &scroll_hid; 
            break; 
            
        case INTERACT_BUTTON: 
            is_pressed = HID_SELECT_Get() == 0; 
            status     = &interact_status; 
            hid        = &interact_hid; 
            break;
            
        default:
            return; 
    }
    
    // The button has been pressed, save its state and the timestamp of the 
    // event. 
    if (is_pressed)
    {
        status->is_triggered = 1; 
        status->last_trigger_timestamp = timestamp; 
        return; 
    }
    
    // Check the button state to correct case of desyncronisation. 
    if (status->is_triggered)
    {
        // Button has been released, check the press time. 
        if (timestamp - status->last_trigger_timestamp >= LONG_PRESS_TIME_DETECT_MS)
            hid->type = LONG_PRESS; 
        
        else
            hid->type = PRESS; 
    }
    
    return; 
}
