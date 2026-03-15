#include "pages.h"


// _ GLOBAL VARIABLE DECLARATIONS ______________________________________________

PAGE_INDEX_t curr_page = PAGE_1; 


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


void display_page(void)
{
    // If the requested page doesn't exist, reset the page queue. 
    if (curr_page >= ARRAY_SIZE(PAGES_LUT))
        curr_page = PAGE_1; 
    
    const WIDGET_t* left_widget = PAGES_LUT[curr_page].left_widget; 
    const WIDGET_t* right_widget = PAGES_LUT[curr_page].right_widget; 

    // Check if a left widget is defined in this page, if so, draws it using the
    // correct drawing funtions depending on it's type. 
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
    
    // Same here for the right widget. Settings widget can't be rendered on the 
    // right because they're full screen. 
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
    
    return; 
}


void page_scroll(void)
{
    curr_page += 1; 
    
    // If the requested page doesn't exist, reset the page queue. 
    if (curr_page >= ARRAY_SIZE(PAGES_LUT))
        curr_page = PAGE_1; 
        
    return; 
}


void page_interact(void)
{
    // If the requested page doesn't exist, reset the page queue and abort. 
    if (curr_page >= ARRAY_SIZE(PAGES_LUT))
    {
        curr_page = PAGE_1; 
        return; 
    }
    
    const WIDGET_t* left_widget = PAGES_LUT[curr_page].left_widget; 
    
    // Fetch the action associated to the settings page and execute it if 
    // it's available. 
    if (left_widget->type == WIDGET_SETTINGS)
    {
        if (left_widget->settings_widget->action.f_ptr)
            left_widget->settings_widget->action.f_ptr(); 
    }

    return; 
}