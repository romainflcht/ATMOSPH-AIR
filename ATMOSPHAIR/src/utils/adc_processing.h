#ifndef _ADC_PROCESSING_H_
#define _ADC_PROCESSING_H_

//* _ INCLUDES _________________________________________________________________

#include <stdlib.h>
#include "definitions.h" 

#include "../cores/adc.h"


#define ADC_Q               0.8e-3

// _ O2 SENSOR DEFINITIONS _____________________________________________________

#define O2_R_GAIN           33000
#define O2_AMP_PER_PPM      0.2e-9
#define O2_OUTPUT_V_OFFSET  2


typedef struct adc_processed_data
{
    float data; 
}   ADC_PROCESSED_DATA_t;





extern ADC_PROCESSED_DATA_t processed_data[ADC_CHANNEL_COUNT];


void O2_sensor_process(void);

#endif