#ifndef _ADC_H_
#define _ADC_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../cores/systick.h"


//* _ DEFINITIONS ______________________________________________________________

#define CONVERSION_TIMEOUT_MS   10
#define WAIT_BETWEEN_CYCLE_MS   500

#define EMA_ALPHA_FACTOR        0.5


//* _ ENUMERATIONS _____________________________________________________________

typedef enum adc_states
{
    ADC_IDLE, 
    ADC_START_CHANNEL_CONVERSION,
    ADC_WAIT_END_OF_CHANNEL_CONVERSION,
    ADC_CONVERSION_DONE,         
}   ADC_STATES_t;


typedef enum adc_channel
{
    ADC_HS2, 
    ADC_O2,
    ADC_CO,
    ADC_FLAMMABLE_GASES,         
    ADC_BATTERY_CHARGE,
    ADC_CHANNEL_COUNT,      ///< Count of active ADC channels, need to be the last element in the enumeration. 
}   ADC_CHANNEL_t;


//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef struct adc_raw_data
{
    uint16_t    data;               ///< ADC conversion result. 
    uint16_t    ema_filtered_data;  ///< ADC conversion result filtered using the EMA algorithm. 
    bool        data_is_new;        ///< Data has been converted and ready to be read. 
}   ADC_RAW_DATA_t;


//* _ EXTERN VARIABLE DECLARATIONS _____________________________________________

extern volatile ADC_RAW_DATA_t ADC_data[ADC_CHANNEL_COUNT]; 


//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn void ADC_init(void); 
/// @brief initialize the ADC peripheral. 
void ADC_init(void); 


/// @fn void ADC_task(void); 
/// @brief maintains the ADC peripheral state machine. 
void ADC_task(void); 

#endif
