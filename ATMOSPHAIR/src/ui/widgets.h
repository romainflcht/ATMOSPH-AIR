#ifndef _WIDGETS_H_
#define _WIDGETS_H_

//* _ INCLUDES _________________________________________________________________

#include <stdlib.h>
#include "definitions.h"

#include "../drivers/ssd1362.h"
#include "../drivers/buzzer.h"
#include "../drivers/sen6x.h"
#include "../drivers/m95.h"
#include "../cores/adc.h"
#include "assets.h"

//* _ DEFINITIONS ______________________________________________________________

#define NO_WIDGET                   0xFF


// Menu widget.
#define BATTERY_LEVEL_DIVIDER       18
#define BATTERY_COLOR_DIVIDER_COUNT 3
#define MAX_NETWORK_STRENGTH        5


// Measure widget. 
#define MEASURE_WIDGET_WIDTH    104
#define MEASURE_WIDGET_HEIGHT   62
#define DECIMAL_COUNT           "2"


//* _ ENUMERATION DECLARATIONS _________________________________________________

typedef enum widget_type
{
    WIDGET_MEASUREMENT,  
    WIDGET_SETTINGS,  
}   WIDGET_TYPE_t;


typedef enum measure_widget_id
{
    WIDGET_PM_0_5, 
    WIDGET_PM_1_0, 
    WIDGET_PM_2_5, 
    WIDGET_PM_4_0, 
    WIDGET_PM_10_0, 
    WIDGET_TEMP, 
    WIDGET_RH, 
    WIDGET_VOC, 
    WIDGET_NOX, 
    WIDGET_CO2, 
    WIDGET_HCHO, 
    WIDGET_H2S,
    WIDGET_O2,
    WIDGET_CO,
    WIDGET_FLAMMABLE_GASES,
    WIDGET_BATTERY_CHARGE,
    WIDGET_COUNT, 
}   MEASURE_WIDGET_ID_t;


typedef enum reasure_widget_val_type
{
    FLOAT, 
    INTEGER, 
}   MEASURE_WIDGET_VAL_TYPE_t;


//* _ STRUCTURE DECLARATIONS ___________________________________________________

// _ measure widget ____________________________________________________________

typedef struct measure_widget
{
    MEASURE_WIDGET_ID_t         type; 
    MEASURE_WIDGET_VAL_TYPE_t   val_type; 
    uint8_t const*              icon; 
    char                        title[16]; 
    char                        unit[16]; 
    union   
    {
        volatile float*         as_float; 
        volatile uint16_t*      as_int; 
    }                           measurement;
}   MEASURE_WIDGET_t;


// _ settings widget ____________________________________________________________

typedef struct settings_actions_widget
{
    uint8_t const*  icon; 
    char            name[16]; 
    void            (*f_action)(); 
}   SETTINGS_ACTIONS_WIDGET;


typedef struct settings_widget
{
    uint8_t const*          icon; 
    char                    title[16]; 
    SETTINGS_ACTIONS_WIDGET actions[2]; 
}   SETTINGS_WIDGET_t;


// _ wrapper widget ____________________________________________________________

typedef struct widget
{
    WIDGET_TYPE_t type; 
    union
    {
        MEASURE_WIDGET_t const*     measure_widget;
        SETTINGS_WIDGET_t const*    settings_widget; 
    };
}   WIDGET_t;


//* _ EXTERN VARIABLE DECLARATIONS _____________________________________________

extern const MEASURE_WIDGET_t MEASURE_WIDGET_LUT[]; 
extern const SETTINGS_WIDGET_t SETTINGS_WIDGET_LUT[]; 


//* _ FUNCTION DECLARATIONS ____________________________________________________

void draw_menu_widget(uint32_t x, uint32_t y, uint32_t battery_percent); 


void draw_measurement_widget(uint32_t x, uint32_t y, const MEASURE_WIDGET_t* measure_widget); 


void draw_settings_widget(uint32_t x, uint32_t y, const SETTINGS_WIDGET_t* widget);

#endif