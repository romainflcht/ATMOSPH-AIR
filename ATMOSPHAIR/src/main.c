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
#include "ui/assets.h"
#include "ui/widgets.h"
#include "ui/pages.h"
#include "ui/fonts.h"
#include "utils/utils.h"


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
       
    //* _ MAIN LOOP ____________________________________________________________
    while(true)
    {
        // Maintain state machines of all polled MPLAB Harmony modules.
        SYS_Tasks();
        BUZZER_task(); 
        SEN6X_task(); 
        M95_tasks(); 
        ADC_task(); 
        
        display_fill(MIN_INTENSITY); 
        draw_menu_widget(0, 0, 57); 

         display_page(); 
        
        SSD1362_refresh(); 
        
        if (scroll_hid.type != NO_ACTION)
        {
            if (scroll_hid.type == PRESS)
            {
                page_scroll(); 
                BUZZER_play_melody(UI_MELODY); 
            }
            else
            {
                
                BUZZER_play_melody(BOOT_MELODY); 
            }
            
            scroll_hid.type = NO_ACTION; 
        }
        
        if (interact_hid.type != 0)
        {
            if (interact_hid.type == PRESS)
            {
                page_interact();
                BUZZER_play_melody(UI_MELODY); 
            }
            else
                BUZZER_play_melody(BOOT_MELODY); 
            
            interact_hid.type = NO_ACTION; 
        }
    }

    // Execution should not come here during normal operation
    return EXIT_FAILURE;
}
