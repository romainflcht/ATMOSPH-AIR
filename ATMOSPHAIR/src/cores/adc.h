#ifndef _ADC_H_
#define _ADC_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../cores/systick.h"


//* _ DEFINITIONS ______________________________________________________________

#define ADC_CHANNEL_COUNT       4

#define CONVERSION_TIMEOUT_MS   10
#define WAIT_BETWEEN_CYCLE_MS   1


#if ADC_CHANNEL_COUNT > 21 || ADC_CHANNEL_COUNT < 0
    #error "~[ERR: DEVICE SELECTION] ADC_CHANNEL_COUNT needs to be between 0 and 21."
#endif


//* _ ENUMERATIONS _____________________________________________________________

typedef enum adc_state
{
    ADC_IDLE, 
    ADC_START_CHANNEL_CONVERSION,
    ADC_WAIT_END_OF_CHANNEL_CONVERSION,
    ADC_CONVERSION_DONE,         
}   ADC_STATE_t;


//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef struct adc_data
{
    uint16_t    data; 
    bool        data_is_new; 
}   ADC_DATA_t;


//* _ FUNCTION DECLARATIONS ____________________________________________________

void ADC_init(void); 


void ADC_task(void); 

void ADC_callback(ADC_STATUS status, uintptr_t context); 

#endif
