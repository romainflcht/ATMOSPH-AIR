#include "widgets.h"
#include "drivers/m95.h"
#include "drivers/sen6x.h"


//* _ EXTERN ASSETS ____________________________________________________________
extern const uint8_t*   NETWORK_STATUS[5]; 
extern const uint8_t*   SERVER_STATUS[2]; 
extern const uint8_t*   SPEAKER_STATUS[2]; 
extern const uint8_t    WARNING_STATUS_ASSET[]; 
extern const uint8_t    MENU_LEFT_ASSET[]; 
extern const uint8_t    MENU_RIGHT_ASSET[]; 


//* _ EXTERN DATA USED BY WIDGET _______________________________________________

extern M95_STATUS_t        M95_status; 
extern MQTT_CONN_STATUS_t  mqtt_status; 
extern bool                speaker_is_active; 
extern SEN6X_DATA_t         sen6x_data; 

static MEASURE_WIDGET_t MEASURE_WIDGET_LUT[] = {
    {
        .title = "PM 0.5", 
        .type = PM_0_5_WIDGET, 
        .unit = "PPM", 
        .value_ptr = &(sen6x_data.PM_0_5),
    }, 
    {
        .title = "PM 1.0", 
        .type = PM_1_0_WIDGET, 
        .unit = "PPM", 
        .value_ptr = &(sen6x_data.PM_1_0),
    }, 
    {
        .title = "PM 2.5", 
        .type = PM_2_5_WIDGET, 
        .unit = "PPM", 
        .value_ptr = &(sen6x_data.PM_2_5),
    }, 
    {
        .title = "PM 4.0", 
        .type = PM_4_0_WIDGET, 
        .unit = "PPM", 
        .value_ptr = &(sen6x_data.PM_4_0),
    }, 
    {
        .title = "PM 10.0", 
        .type = PM_10_0_WIDGET, 
        .unit = "PPM", 
        .value_ptr = &(sen6x_data.PM_10_0),
    }, 
    {
        .title = "TEMP", 
        .type = TEMP_WIDGET, 
        .unit = "'C", 
        .value_ptr = &(sen6x_data.temp),
    }, 
    {
        .title = "HUMIDITY", 
        .type = RH_WIDGET, 
        .unit = "%", 
        .value_ptr = &(sen6x_data.humidity),
    }, 
}; 


void draw_menu_widget(uint32_t x, uint32_t y, uint32_t battery_percent)
{
    uint32_t i; 
    uint32_t battery_level; 
    uint32_t network_strength; 
    bool is_warn_triggered; 
    bool is_server_connected; 
    
    battery_level       = (battery_percent * BATTERY_LEVEL_DIVIDER) / 100;
    is_warn_triggered   = M95_status.fatal_err; 
    is_server_connected = mqtt_status.mqtt_is_conn && mqtt_status.mqtt_is_open; 
    network_strength    = (M95_status.signal_strength * MAX_NETWORK_STRENGTH) / MAX_RSSI_VAL; 
    

    display_img(x, y, MENU_LEFT_WIDTH, MENU_LEFT_HEIGHT, MENU_LEFT_ASSET); 
    display_img(x + DISPLAY_WIDTH - MENU_RIGHT_WIDTH, y, MENU_RIGHT_WIDTH, MENU_RIGHT_HEIGHT, MENU_RIGHT_ASSET); 
    display_fast_h_line(x, y, DISPLAY_WIDTH, MAX_INTENSITY); 
    display_fast_h_line(x, y + DISPLAY_HEIGHT - 1, DISPLAY_WIDTH, MAX_INTENSITY); 
    
    for (i = 0; i < battery_level; i += 1)
        display_fast_h_line(
                x + 2, 
                y + DISPLAY_HEIGHT - 3 - i * 3, 
                y + 6, 
                ((i * BATTERY_COLOR_DIVIDER_COUNT) / BATTERY_LEVEL_DIVIDER) * QUARTER_INTENSITY
            ); 
    
    display_printf(x, y + 1, MIN_INTENSITY, 0, "%d%%", battery_percent); 
        
    if (is_warn_triggered)
        display_img(x + 14, y + 19, WARNING_STATUS_WIDTH, WARNING_STATUS_HEIGHT, WARNING_STATUS_ASSET); 

    display_img(x + 12, y + 39, NETWORK_STATUS_WIDTH, NETWORK_STATUS_HEIGHT, NETWORK_STATUS[network_strength % MAX_NETWORK_STRENGTH]); 
    display_img(x + 11, y + 31, SERVER_STATUS_WIDTH, SERVER_STATUS_HEIGHT, SERVER_STATUS[is_server_connected]); 
    display_img(x + 12, y + 51, SPEAKER_STATUS_WIDTH, SPEAKER_STATUS_HEIGHT, SPEAKER_STATUS[speaker_is_active]); 
}


void draw_measurement_widget(uint32_t x, uint32_t y, MEASURE_WIDGET_ID_t measure_widget_id)
{
    MEASURE_WIDGET_t    measure_widget; 
    uint32_t            value_len; 
    uint32_t            unit_len; 
    char                buffer[16]; 
    
    if (measure_widget_id >= MEASURE_WIDGET_COUNT)
        return; 
    
    measure_widget = MEASURE_WIDGET_LUT[measure_widget_id]; 
    
    value_len = sprintf(buffer, "%." DECIMAL_COUNT "f", *(measure_widget.value_ptr));
    unit_len = strlen(measure_widget.unit); 
    
    display_draw_str(x + 2, y, measure_widget.title, MAX_INTENSITY, FONT_10X16); 
    
    
    display_draw_str(
        x + (MEASURE_WIDGET_WIDTH / 2) - (value_len * 10) / 2, 
        MEASURE_WIDGET_HEIGHT / 2 - 8, buffer, 
        MAX_INTENSITY, 
        FONT_10X16_BOLD
    ); 
    
    display_draw_str(
        x + (MEASURE_WIDGET_WIDTH / 2) - (unit_len * 10) / 2, 
        31 + 5,
        measure_widget.unit, 
        MAX_INTENSITY, 
        FONT_10X16
    ); 
    return; 
}