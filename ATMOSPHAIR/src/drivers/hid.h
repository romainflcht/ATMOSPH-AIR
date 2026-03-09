#ifndef _HID_H_
#define _HID_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../cores/systick.h"
#include "../ui/pages.h"


//* _ DEFINITIONS ______________________________________________________________

#define LONG_PRESS_TIME_DETECT_MS   200


//* _ ENUMERATION DEFINITIONS __________________________________________________

typedef enum hid_button
{
    SCROLL_BUTTON   = EIC_PIN_1, 
    INTERACT_BUTTON = EIC_PIN_2, 
}   HID_BUTTON_t; 


typedef enum hid_event_type
{
    NO_ACTION, 
    PRESS, 
    LONG_PRESS, 
}   HID_EVENT_TYPE_t;

//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef struct interrupt_status
{
    bool        is_triggered;
    uint32_t    last_trigger_timestamp; 
}   INTERRUPT_STATUS_t;


typedef struct hid_event
{
    HID_EVENT_TYPE_t    type; 
}   HID_EVENT_t;


//* _ EXTERN VARIABLE DECLARATIONS _____________________________________________

extern HID_EVENT_t scroll_hid; 
extern HID_EVENT_t interact_hid; 


//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn void HID_init(void); 
/// @ brief initialize HID push buttons. 
void HID_init(void); 

#endif