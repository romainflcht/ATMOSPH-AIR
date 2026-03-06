#ifndef _HID_H_
#define _HID_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../cores/systick.h"


#define LONG_PRESS_TIME_DETECT   500


//* _ ENUMERATION DEFINITIONS __________________________________________________

typedef enum hid_button
{
    SCROLL_BUTTON = EIC_PIN_1, 
    SELECT_BUTTON = EIC_PIN_2, 
}   HID_BUTTON_t; 


typedef enum hid_event_type
{
    NO_ACTION, 
    PRESS, 
    LONG_PRESS, 
}   HID_EVENT_TYPE_t;


typedef struct interrupt_status
{
    bool        is_triggered;
    uint32_t    last_trigger_timestamp; 
}   INTERRUPT_STATUS_t;


typedef struct hid_event
{
    HID_EVENT_TYPE_t    type; 
    uint32_t            timestamp; 
}   HID_EVENT_t;

void HID_init(void); 

#endif