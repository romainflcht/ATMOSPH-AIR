#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "definitions.h"

#include "cores/systick.h"

#include "drivers/ssd1362.h"
#include "drivers/sen6x.h"
#include "cores/adc.h"


extern SEN6X_DATA_t         sen6x_data; 
extern volatile ADC_DATA_t  ADC_data[4]; 


const uint8_t image_data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
    0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 
    0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


//* _ ENTRY POINT ______________________________________________________________
int main(void)
{
    char text_buffer[128]; 
    
    //* _ MODULE INITIALIZATIONS _______________________________________________
    SYS_Initialize(NULL);
    ssd1362_init(); 
    
    display_fill(MIN_INTENSITY); 
    display_draw_str(DISPLAY_WIDTH / 2 - 45, DISPLAY_HEIGHT / 2 - 4, "INITIALIZING...", MAX_INTENSITY); 
    
    ssd1362_refresh(); 
    
    SYSTICK_init(); 
    ADC_init(); 
    sen6x_init(); 
    
    display_fill(MIN_INTENSITY); 
    display_draw_str(DISPLAY_WIDTH / 2 - 45, DISPLAY_HEIGHT / 2 - 4, "INITIALIZED!", MAX_INTENSITY); 
    
    ssd1362_refresh(); 
    
    //* _ MAIN LOOP ____________________________________________________________
    while(true)
    {
        // Maintain state machines of all polled MPLAB Harmony modules.
        SYS_Tasks();
        sen6x_task(); 
        ADC_task(); 
        
        display_fill(MIN_INTENSITY); 
        sprintf(text_buffer, "ADCIN0: %d\n", ADC_data[0].data); 
        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 0, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "ADCIN1: %d\n", ADC_data[1].data); 
        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 1, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "ADCIN2: %d\n", ADC_data[2].data); 
        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 2, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "ADCIN3: %d\n", ADC_data[3].data); 
        display_draw_str(FONT_CHAR_WIDTH * 15, FONT_CHAR_HEIGHT * 3, text_buffer, MAX_INTENSITY); 
        
        sprintf(text_buffer, "PM1  : %.3f\n", sen6x_data.PM_1_0);
        display_draw_str(0, FONT_CHAR_HEIGHT * 0, text_buffer, MAX_INTENSITY); 
        
        sprintf(text_buffer, "PM2.5: %.3f\n", sen6x_data.PM_2_5);
        display_draw_str(0, FONT_CHAR_HEIGHT * 1, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "PM4  : %.3f\n", sen6x_data.PM_4_0);
        display_draw_str(0, FONT_CHAR_HEIGHT * 2, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "PM10 : %.3f\n", sen6x_data.PM_10_0);
        display_draw_str(0, FONT_CHAR_HEIGHT * 3, text_buffer, MAX_INTENSITY); 
        
        sprintf(text_buffer, "RH   : %.3f\n", sen6x_data.humidity);
        display_draw_str(0, FONT_CHAR_HEIGHT * 4, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "TEMP : %.3f\n", sen6x_data.temp);
        display_draw_str(0, FONT_CHAR_HEIGHT * 5, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "VOC  : %.3f\n", sen6x_data.VOC);
        display_draw_str(0, FONT_CHAR_HEIGHT * 6, text_buffer, MAX_INTENSITY); 

        sprintf(text_buffer, "NOx  : %.3f\n", sen6x_data.NOx);
        display_draw_str(0, FONT_CHAR_HEIGHT * 7, text_buffer, MAX_INTENSITY); 

        ssd1362_refresh(); 
    }

    // Execution should not come here during normal operation
    return EXIT_FAILURE;
}


// _ SAVES _____________________________________________________________________

//  UART
//        while(SERCOM2_SPI_IsBusy());
    
//        SERCOM0_USART_Write("AT\r\n", 4); 
//        
//        for (int i = 0; i < 1000000; i += 1); 
//
//        SERCOM0_USART_Write("ATE0\r\n", 6); 
//        
//        
//        for (int i = 0; i < 1000000; i += 1); 
//
//        SERCOM0_USART_Write("AT+CSQ\r\n", 8); 
//
//sprintf(
//                text_buffer, 
//                "PM1: %.3f\nPM2.5: %.3f\nPM4: %.3f\nPM10: %.3f\n"
//                "HUMIDITY: %.3f\nTEMP: %.3f\nVOC: %.3f\nNOx: %.3f", 
//                sen6x_data.PM_1_0, 
//                sen6x_data.PM_2_5, 
//                sen6x_data.PM_4_0, 
//                sen6x_data.PM_10_0, 
//                sen6x_data.humidity, 
//                sen6x_data.temp, 
//                sen6x_data.VOC, 
//                sen6x_data.NOx
//        );