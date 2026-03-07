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
#include "drivers/buzzer.h"
#include "cores/pwm.h"
#include "drivers/hid.h"
#include "utils/assets.h"
#include "utils/widgets.h"
#include "utils/fonts.h"

extern const uint8_t MENU_LEFT_ASSET[]; 
extern const uint8_t MENU_RIGHT_ASSET[]; 

extern SEN6X_DATA_t         sen6x_data; 
extern volatile ADC_DATA_t  ADC_data[4]; 

extern HID_EVENT_t                 scroll_hid; 
extern HID_EVENT_t                 select_hid; 

extern const NOTE_t BOOT_MELODY[]; 
extern const NOTE_t ERR_MELODY[]; 
extern const NOTE_t UI_MELODY[]; 


//* _ ENTRY POINT ______________________________________________________________
int main(void)
{

    //* _ MODULE INITIALIZATIONS _______________________________________________
    SYS_Initialize(NULL);
    ssd1362_init(); 
    
    display_fill(MIN_INTENSITY); 
    display_draw_str(DISPLAY_WIDTH / 2 - 45, DISPLAY_HEIGHT / 2 - 4, "INITIALIZING...", MAX_INTENSITY, 1); 
    
    
    SSD1362_refresh(); 
    
    SYSTICK_init(); 
    ADC_init(); 
    M95_init(); 
    SEN6X_init(); 
    HID_init(); 
    
    display_fill(MIN_INTENSITY); 
    display_draw_str(DISPLAY_WIDTH / 2 - 45, DISPLAY_HEIGHT / 2 - 4, "INITIALIZED!", MAX_INTENSITY, 0); 
    
    SSD1362_refresh(); 
    
    TCC1_PWMStart(); 
    
    TCC1_set_duty_cycle(0, 0);
    TCC1_set_duty_cycle(1, 0);
    int batt = 100; 
       
    //* _ MAIN LOOP ____________________________________________________________
    while(true)
    {
        // Maintain state machines of all polled MPLAB Harmony modules.
        SYS_Tasks();
        BUZZER_task(); 
        SEN6X_task(); 
        M95_task(); 
        ADC_task(); 
        
        display_fill(MIN_INTENSITY); 
        draw_menu_widget(0, 0, batt); 

        if (batt > 50)
        {
            draw_measurement_widget(34, 1, TEMP_WIDGET); 
            draw_measurement_widget(138, 1, RH_WIDGET); 
        }
        
        else
        {
            draw_measurement_widget(34, 1, PM_1_0_WIDGET); 
            draw_measurement_widget(138, 1, PM_2_5_WIDGET); 
        }
 
        SSD1362_refresh(); 
        batt -= 1; 
        if (batt < 0)
            batt = 100; 
        

        
        if (scroll_hid.type != 0)
        {
            if (scroll_hid.type == 1)
                BUZZER_play_melody(UI_MELODY); 
            else
                BUZZER_play_melody(BOOT_MELODY); 
            
            scroll_hid.type = 0; 
        }
        
        if (select_hid.type != 0)
        {
            if (select_hid.type == 1)
                BUZZER_play_melody(UI_MELODY); 
            else
                BUZZER_play_melody(BOOT_MELODY); 
            
            select_hid.type = 0; 
        }
    }

    // Execution should not come here during normal operation
    return EXIT_FAILURE;
}
