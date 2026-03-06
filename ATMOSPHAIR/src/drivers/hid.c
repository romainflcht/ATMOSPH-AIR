#include "hid.h"


static INTERRUPT_STATUS_t   scroll_status = {0}; 
static INTERRUPT_STATUS_t   select_status = {0}; 

HID_EVENT_t                 scroll_hid    = {0}; 
HID_EVENT_t                 select_hid    = {0}; 


static void scroll_button_callback(uintptr_t context); 
static void select_button_callback(uintptr_t context); 
static void button_action(HID_BUTTON_t trigger); 



void HID_init(void)
{
    EIC_CallbackRegister(SCROLL_BUTTON, scroll_button_callback, (uintptr_t)NULL); 
    EIC_CallbackRegister(SELECT_BUTTON, select_button_callback, (uintptr_t)NULL); 
    
    EIC_InterruptEnable(SCROLL_BUTTON); 
    EIC_InterruptEnable(SELECT_BUTTON); 
}


static void scroll_button_callback(uintptr_t context)
{
    button_action(SCROLL_BUTTON); 
    return; 
}


static void select_button_callback(uintptr_t context)
{
    button_action(SELECT_BUTTON); 
    return; 
}


static void button_action(HID_BUTTON_t trigger)
{
    bool                is_pressed; 
    uint32_t            timestamp; 
    INTERRUPT_STATUS_t* status; 
    HID_EVENT_t*        hid; 
    
    
    switch (trigger)
    {
        case SCROLL_BUTTON: 
            is_pressed = HID_SCROLL_Get() == 0; 
            status = &scroll_status; 
            hid = &scroll_hid; 
            break; 
            
        case SELECT_BUTTON: 
            is_pressed = HID_SELECT_Get() == 0; 
            status = &select_status; 
            hid = &select_hid; 
            break;
            
        default:
            return; 
    }
    
    
    timestamp  = SYSTICK_millis(); 
    
    if (is_pressed)
    {
        status->is_triggered = 1; 
        status->last_trigger_timestamp = timestamp; 
        return; 
    }
    
    if (status->is_triggered)
    {
        if (timestamp - status->last_trigger_timestamp >= LONG_PRESS_TIME_DETECT)
            hid->type = LONG_PRESS; 
        
        else
            hid->type = PRESS; 
        
        hid->timestamp = timestamp; 
    }
    
    return; 
}