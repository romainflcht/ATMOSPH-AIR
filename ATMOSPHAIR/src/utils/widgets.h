#ifndef _WIDGETS_H_
#define _WIDGETS_H_

//* _ INCLUDES _________________________________________________________________

#include <stdlib.h>
#include "definitions.h"

#include "../drivers/ssd1362.h"
#include "../drivers/buzzer.h"
#include "../drivers/m95.h"
#include "assets.h"


//* _ MENU WIDGET ______________________________________________________________

#define BATTERY_LEVEL_DIVIDER       18
#define BATTERY_COLOR_DIVIDER_COUNT 3
#define MAX_NETWORK_STRENGTH        5


// * _ MEASURE WIDGET __________________________________________________________

#define MEASURE_WIDGET_WIDTH    104
#define MEASURE_WIDGET_HEIGHT   62
#define DECIMAL_COUNT           "2"



typedef enum measure_widget_id
{
    PM_0_5_WIDGET, 
    PM_1_0_WIDGET, 
    PM_2_5_WIDGET, 
    PM_4_0_WIDGET, 
    PM_10_0_WIDGET, 
    TEMP_WIDGET, 
    RH_WIDGET, 
    MEASURE_WIDGET_COUNT, 
}   MEASURE_WIDGET_ID_t;


typedef struct measure_widget
{
    MEASURE_WIDGET_ID_t   type; 
    char                        title[16]; 
    char                        unit[16]; 
    float*                      value_ptr; 
}   MEASURE_WIDGET_t;

void draw_menu_widget(uint32_t x, uint32_t y, uint32_t battery_percent); 


void draw_measurement_widget(uint32_t x, uint32_t y, MEASURE_WIDGET_ID_t measure_widget); 

#endif