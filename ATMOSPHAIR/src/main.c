#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "definitions.h"

#include "cores/systick.h"

#include "drivers/ssd1362.h"
#include "drivers/sen6x.h"
#include "drivers/m95.h"
#include "cores/adc.h"


extern SEN6X_DATA_t         sen6x_data; 
extern volatile ADC_DATA_t  ADC_data[4]; 


//* _ ENTRY POINT ______________________________________________________________
int main(void)
{
//    char text_buffer[128]; 
    

    
    //* _ MODULE INITIALIZATIONS _______________________________________________
    SYS_Initialize(NULL);
//    ssd1362_init(); 
    
//    display_fill(MIN_INTENSITY); 
//    display_draw_str(DISPLAY_WIDTH / 2 - 45, DISPLAY_HEIGHT / 2 - 4, "INITIALIZING...", MAX_INTENSITY); 
    
//    ssd1362_refresh(); 
    
    SYSTICK_init(); 
//    ADC_init(); 
    M95_init(); 
//    sen6x_init(); 
    
//    display_fill(MIN_INTENSITY); 
//    display_draw_str(DISPLAY_WIDTH / 2 - 45, DISPLAY_HEIGHT / 2 - 4, "INITIALIZED!", MAX_INTENSITY); 
    
//    ssd1362_refresh(); 
    
    //* _ MAIN LOOP ____________________________________________________________
    while(true)
    {
        // Maintain state machines of all polled MPLAB Harmony modules.
        SYS_Tasks();
//        sen6x_task(); 
        M95_write_task(); 
        M95_read_task(); 
//        ADC_task(); 
        
//        display_fill(MIN_INTENSITY); 
//        sprintf(text_buffer, "ADCIN0: %d\n", ADC_data[0].data); 
//        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 0, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "ADCIN1: %d\n", ADC_data[1].data); 
//        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 1, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "ADCIN2: %d\n", ADC_data[2].data); 
//        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 2, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "ADCIN3: %d\n", ADC_data[3].data); 
//        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 3, text_buffer, MAX_INTENSITY); 
//        
//        sprintf(text_buffer, "PM1  : %.3f\n", sen6x_data.PM_1_0);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 0, text_buffer, MAX_INTENSITY); 
//        
//        sprintf(text_buffer, "PM2.5: %.3f\n", sen6x_data.PM_2_5);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 1, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "PM4  : %.3f\n", sen6x_data.PM_4_0);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 2, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "PM10 : %.3f\n", sen6x_data.PM_10_0);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 3, text_buffer, MAX_INTENSITY); 
//        
//        sprintf(text_buffer, "RH   : %.3f\n", sen6x_data.humidity);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 4, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "TEMP : %.3f\n", sen6x_data.temp);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 5, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "VOC  : %.3f\n", sen6x_data.VOC);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 6, text_buffer, MAX_INTENSITY); 
//
//        sprintf(text_buffer, "NOx  : %.3f\n", sen6x_data.NOx);
//        display_draw_str(0, FONT_CHAR_HEIGHT * 7, text_buffer, MAX_INTENSITY); 
//
//        ssd1362_refresh(); 
    }

    // Execution should not come here during normal operation
    return EXIT_FAILURE;
}
