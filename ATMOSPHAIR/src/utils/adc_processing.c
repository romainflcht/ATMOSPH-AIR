#include "adc_processing.h"


ADC_PROCESSED_DATA_t processed_data[ADC_CHANNEL_COUNT]; 


void O2_sensor_process(void)
{
    uint16_t    adc_conv; 
    float       result; 
    
    adc_conv = ADC_data[ADC_O2].ema_filtered_data; 
    
    result = ((((float)adc_conv * ADC_Q) - O2_OUTPUT_V_OFFSET) / - O2_R_GAIN) / O2_AMP_PER_PPM; 
    
    processed_data[ADC_O2].data = result; 
}