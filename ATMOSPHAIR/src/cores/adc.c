#include "adc.h"

//* _ GLOBAL VARIABLE DECLARATIONS _____________________________________________

volatile ADC_DATA_t         ADC_data[ADC_CHANNEL_COUNT]; 


//* _ STATIC VARIABLE DECLARATIONS _____________________________________________

static volatile ADC_STATE_t curr_state                = ADC_IDLE; 
static volatile uint8_t     selected_adc_channel_id   = 0; 
static uint32_t             last_conversion_timestamp = 0; 


//* _ STATIC FUNCTION DECLARATIONS _____________________________________________

static void ADC_IDLE_state(void); 
static void ADC_START_CHANNEL_CONVERSION_state(void); 
static void ADC_WAIT_END_OF_CHANNEL_CONVERSION_state(void); 
static void ADC_CONVERSION_DONE_state(void); 


//* _ LUT ______________________________________________________________________

static const ADC_POSINPUT adc_channel_from_id[] = {
    ADC_POSINPUT_AIN0,  ADC_POSINPUT_AIN1,  ADC_POSINPUT_AIN2,
    ADC_POSINPUT_AIN3,  ADC_POSINPUT_AIN4,  ADC_POSINPUT_AIN5,
    ADC_POSINPUT_AIN6,  ADC_POSINPUT_AIN7,  ADC_POSINPUT_AIN8,
    ADC_POSINPUT_AIN9,  ADC_POSINPUT_AIN10, ADC_POSINPUT_AIN11,
    ADC_POSINPUT_AIN18, ADC_POSINPUT_AIN19, ADC_POSINPUT_BANDGAP,
    ADC_POSINPUT_SCALEDVDDCORE, ADC_POSINPUT_SCALEDAVDD,
    ADC_POSINPUT_DAC0, ADC_POSINPUT_SCALEDVDD, 
    ADC_POSINPUT_OPAMP01, ADC_POSINPUT_OPAMP2,
};



void ADC_init(void)
{
    // Start the ADC peripheral and register the function callback. 
    ADC_Enable(); 
    ADC_CallbackRegister(ADC_callback, (uintptr_t)NULL); 
    return; 
}


void ADC_task(void)
{
    switch (curr_state)
    {
        case ADC_IDLE: 
            ADC_IDLE_state(); 
            break; 
        
        case ADC_START_CHANNEL_CONVERSION: 
            ADC_START_CHANNEL_CONVERSION_state(); 
            break; 
            
        case ADC_WAIT_END_OF_CHANNEL_CONVERSION: 
            ADC_WAIT_END_OF_CHANNEL_CONVERSION_state(); 
            break; 
        
        case ADC_CONVERSION_DONE: 
            ADC_CONVERSION_DONE_state(); 
            break; 
            
        default:
            curr_state = ADC_IDLE; 
            break; 
    }
    
    return; 
}


void ADC_callback(ADC_STATUS status, uintptr_t context) 
{
    // ADC channel select index error, reset the whole machine state. 
    if (selected_adc_channel_id >= ADC_CHANNEL_COUNT)
    {
        curr_state = ADC_IDLE; 
        return; 
    }
    
    // Get the newly converted data from the ADC and mark it has new value. 
    ADC_data[selected_adc_channel_id].data = ADC_ConversionResultGet(); 
    ADC_data[selected_adc_channel_id].data_is_new = true;
    
    // Increment the channel selection and go to the START_CHANNEL_CONVERSION 
    // state. 
    selected_adc_channel_id += 1; 
    curr_state = ADC_START_CHANNEL_CONVERSION; 
    return; 
}


//* _ STATIC FUNCTION IMPLEMENTATIONS __________________________________________

static void ADC_IDLE_state(void)
{
    selected_adc_channel_id = 0; 
    curr_state = ADC_START_CHANNEL_CONVERSION; 
    return; 
}


static void ADC_START_CHANNEL_CONVERSION_state(void)
{    
    // Go to the next state when all ADC conversion are done. 
    if (selected_adc_channel_id >= ADC_CHANNEL_COUNT)
    {
        last_conversion_timestamp = SYSTICK_millis(); 
        curr_state = ADC_CONVERSION_DONE; 
        return; 
    }
    
    // Select the ADC channel and start the conversion. 
    ADC_ChannelSelect(
            adc_channel_from_id[selected_adc_channel_id], 
            ADC_NEGINPUT_AVSS
    );
    
    ADC_ConversionStart();
    
    // Save the timestamp of the last conversion request and go to the next 
    // state. 
    last_conversion_timestamp = SYSTICK_millis(); 
    curr_state = ADC_WAIT_END_OF_CHANNEL_CONVERSION; 
    return; 
}


static void ADC_WAIT_END_OF_CHANNEL_CONVERSION_state(void)
{
    if (SYSTICK_millis() - last_conversion_timestamp <= CONVERSION_TIMEOUT_MS)
        return; 
    
    curr_state = ADC_IDLE;
    return; 
}


static void ADC_CONVERSION_DONE_state(void)
{
    if (SYSTICK_millis() - last_conversion_timestamp <= WAIT_BETWEEN_CYCLE_MS)
        return; 
    
    curr_state = ADC_IDLE; 
    return; 
}


//* _ UTILITY FUNCTION IMPLEMENTATIONS _________________________________________
