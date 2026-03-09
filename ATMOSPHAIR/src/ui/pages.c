#include "pages.h"


// _ STATIC VARIABLE DECLARATIONS ______________________________________________

static const PAGE_t PAGES_LUT[] = {
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_PM_0_5]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_PM_1_0]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_PM_2_5]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_PM_4_0]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_PM_10_0]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_VOC]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_CO2]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_CO]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_NOX]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_H2S]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_FLAMMABLE_GASES]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_O2]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_TEMP]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_RH]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_H2S]), 
        },
        .right_widget = &(const WIDGET_t){
            .type           = WIDGET_MEASUREMENT, 
            .measure_widget = &(MEASURE_WIDGET_LUT[WIDGET_BATTERY_CHARGE]), 
        }, 
    }, 
    {
        .left_widget  = &(const WIDGET_t){
            .type           = WIDGET_SETTINGS, 
            .settings_widget = &(SETTINGS_WIDGET_LUT[0]), 
        },
    }, 
};


void display_page(PAGE_INDEX_t page_index)
{
    // If the requested page doesn't exist, abort. 
    if (page_index >= ARRAY_SIZE(PAGES_LUT))
        return; 
    
    const WIDGET_t* left_widget = PAGES_LUT[page_index].left_widget; 
    const WIDGET_t* right_widget = PAGES_LUT[page_index].right_widget; 
    
    if (left_widget)
    {
        switch (left_widget->type)
        {
            case WIDGET_MEASUREMENT:
                draw_measurement_widget(
                    LEFT_WIDGET_X_POS, 
                    LEFT_WIDGET_Y_POS, 
                    left_widget->measure_widget
                ); 
                break; 
            case WIDGET_SETTINGS: 
                draw_settings_widget(
                    LEFT_WIDGET_X_POS, 
                    LEFT_WIDGET_Y_POS, 
                    left_widget->settings_widget
                ); 
                break; 
            
            default: 
                break; 
        }
    }
    
    if (right_widget)
    {
        switch (right_widget->type)
        {
            case WIDGET_MEASUREMENT:
                draw_measurement_widget(
                    RIGHT_WIDGET_X_POS, 
                    RIGHT_WIDGET_Y_POS, 
                    right_widget->measure_widget
                ); 
                break; 
            case WIDGET_SETTINGS: 
            default: 
                break; 
        }
    }
}