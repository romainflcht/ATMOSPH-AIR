#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "definitions.h"

#include "cores/systick.h"

#include "drivers/ssd1362.h"
#include "drivers/sen6x.h"


extern SEN6X_DATA_t sen6x_data; 


const uint8_t image_data[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 
    0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 
    0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 0x0F, 0x77, 0x77, 0x77, 0x77, 0xF0, 
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


//* _ ENTRY POINT ______________________________________________________________
int main(void)
{
    char text_buffer[1024]; 
    
    //* _ MODULE INITIALIZATIONS _______________________________________________
    SYS_Initialize(NULL);
    SYSTICK_init(); 
    ssd1362_init(); 
    
    display_fill(MIN_INTENSITY); 
    display_draw_str(0, 0, "INITIALIZING...", MAX_INTENSITY); 
    
    ssd1362_refresh(); 
    
    sen6x_init(); 
    
    display_fill(MIN_INTENSITY); 
    display_draw_str(0, 0, "INITIALIZED!", MAX_INTENSITY); 
    
    ssd1362_refresh(); 
    
    

    
    //* _ MAIN LOOP ____________________________________________________________
    while(true)
    {
        // Maintain state machines of all polled MPLAB Harmony modules.
        SYS_Tasks();

        sen6x_task(); 
        sprintf(
                text_buffer, 
                "PM1: %.3f\nPM2.5: %.3f\nPM4: %.3f\nPM10: %.3f\n"
                "HUMIDITY: %.3f\nTEMP: %.3f\nVOC: %.3f\nNOx: %.3f", 
                sen6x_data.PM_1_0, 
                sen6x_data.PM_2_5, 
                sen6x_data.PM_4_0, 
                sen6x_data.PM_10_0, 
                sen6x_data.humidity, 
                sen6x_data.temp, 
                sen6x_data.VOC, 
                sen6x_data.NOx
        );
        
        display_fill(MIN_INTENSITY); 
        display_draw_str(0, 0, text_buffer, MAX_INTENSITY); 
        ssd1362_refresh(); 
    }

    // Execution should not come here during normal operation
    return EXIT_FAILURE;
}


// _ SAVES _____________________________________________________________________

//        display_draw_rect(0, 0, DISPLAY_WIDTH - 1, 10, MAX_INTENSITY); 
//        display_draw_str(DISPLAY_WIDTH - 15 - 24, 2, "100%", MAX_INTENSITY); 
//        display_img(DISPLAY_WIDTH - 7, 1, 12, 8, image_data); 
//        
//        
//
//        ssd1362_refresh(); 
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
