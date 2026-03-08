#include "pages.h"


// _ STATIC VARIABLE DECLARATIONS ______________________________________________

static const PAGE_t PAGES_LUT[] = {
    {
        .left_widget_id  = WIDGET_PM_0_5, 
        .right_widget_id = WIDGET_PM_1_0, 
    }, 
    {
        .left_widget_id  = WIDGET_PM_2_5, 
        .right_widget_id = WIDGET_PM_4_0, 
    }, 
    {
        .left_widget_id  = WIDGET_PM_10_0, 
        .right_widget_id = WIDGET_VOC, 
    }, 
    {
        .left_widget_id  = WIDGET_CO2, 
        .right_widget_id = WIDGET_CO, 
    }, 
    {
        .left_widget_id  = WIDGET_NOX, 
        .right_widget_id = WIDGET_H2S, 
    }, 
    {
        .left_widget_id  = WIDGET_FLAMMABLE_GASES, 
        .right_widget_id = WIDGET_O2, 
    }, 
    {
        .left_widget_id  = WIDGET_TEMP, 
        .right_widget_id = WIDGET_RH, 
    }, 
    {
        .left_widget_id  = WIDGET_H2S, 
        .right_widget_id = WIDGET_BATTERY_CHARGE, 
    }, 
};


void display_page(PAGE_INDEX_t page_index)
{
    // If the requested page doesn't exist, abort. 
    if (page_index >= ARRAY_SIZE(PAGES_LUT))
        return; 
    
    
    // Else, shows left and right widget if they are defined. 
    if (PAGES_LUT[page_index].left_widget_id != NO_WIDGET)
        draw_measurement_widget(34, 1, PAGES_LUT[page_index].left_widget_id); 

    if (PAGES_LUT[page_index].right_widget_id != NO_WIDGET)
    {
        display_fast_v_line(DISPLAY_WIDTH / 2, 10, DISPLAY_HEIGHT - 20, MAX_INTENSITY); 
        draw_measurement_widget(138, 1, PAGES_LUT[page_index].right_widget_id); 
    }
}