#include "widgets.h"

// _ STATIC VARIABLE DECLARATIONS ______________________________________________

const MEASURE_WIDGET_t MEASURE_WIDGET_LUT[] = {
    {
        .title                = "PM 0.5", 
        .icon                 = PM_ICON_ASSET, 
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.PM_0_5),
    }, 
    {
        .title                = "PM 1.0", 
        .icon                 = PM_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.PM_1_0),
    }, 
    {
        .title                = "PM 2.5", 
        .icon                 = PM_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.PM_2_5),
    }, 
    {
        .title                = "PM 4.0", 
        .icon                 = PM_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.PM_4_0),
    }, 
    {
        .title                = "PM 10.0", 
        .icon                 = PM_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.PM_10_0),
    }, 
    {
        .title                = "TEMP", 
        .icon                 = TEMP_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "C", 
        .measurement.as_float = &(SEN6X_data.temp),
    }, 
    {
        .title                = "HUMIDITY", 
        .icon                 = RH_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "%", 
        .measurement.as_float = &(SEN6X_data.humidity),
    }, 
    {
        .title                = "VOC", 
        .icon                 = VOC_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.VOC),
    }, 
    {
        .title                = "NOx", 
        .icon                 = NOX_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.NOx),
    }, 
    {
        .title                = "CO2", 
        .icon                 = CO2_H2S_ICON_ASSET,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.CO2),
    }, 
    {
        .title                = "HCHO", 
        .icon                 = NULL,
        .icon_size            = WIDGET_ICON_SIZE, 
        .val_type             = FLOAT,
        .unit                 = "INDEX", 
        .measurement.as_float = &(SEN6X_data.HCHO),
    }, 
    {
        .title              = "H2S", 
        .icon               = CO2_H2S_ICON_ASSET,
        .icon_size          = WIDGET_ICON_SIZE, 
        .val_type           = INTEGER,
        .unit               = "PPM", 
        .measurement.as_int = &(ADC_data[ADC_HS2].data),
    }, 
    {
        .title              = "O2", 
        .icon               = O2_ICON_ASSET,
        .icon_size          = WIDGET_ICON_SIZE, 
        .val_type           = INTEGER,
        .unit               = "PPM", 
        .measurement.as_int = &(ADC_data[ADC_O2].data),
    }, 
    {
        .title              = "CO", 
        .icon               = CO_ICON_ASSET,
        .icon_size          = WIDGET_ICON_SIZE, 
        .val_type           = INTEGER,
        .unit               = "PPM", 
        .measurement.as_int = &(ADC_data[ADC_CO].data),
    }, 
    {
        .title              = "GASES", 
        .icon               = FLAMMABLE_GASES_ICON_ASSET,
        .icon_size          = WIDGET_ICON_SIZE, 
        .val_type           = INTEGER,
        .unit               = "PPM", 
        .measurement.as_int = &(ADC_data[ADC_FLAMMABLE_GASES].data),
    }, 
    {
        .title              = "BATTERY", 
        .icon               = BATTERY_ICON_ASSET,
        .icon_size          = WIDGET_ICON_SIZE, 
        .val_type           = INTEGER,
        .unit               = "%", 
        .measurement.as_int = &(ADC_data[ADC_BATTERY_CHARGE].data),
    }, 
}; 


const SETTING_WIDGET_t SETTINGS_WIDGET_LUT[] = {
    {
        .title  = "AUDIO SETTINGS",
        .icon   = SETTINGS_ICON_ASSET,
        .icon_size = WIDGET_ICON_SIZE, 
        .action = {
            .icon      = MUTE_ACTION_ASSET, 
            .icon_size = MUTE_ACTION_SIZE, 
            .name      = "TOGGLE SPEAKER",
            .f_ptr     = BUZZER_toggle_mute, 
        }
    }, 
}; 


void draw_menu_widget(uint32_t x, uint32_t y, uint32_t battery_percent)
{
    uint32_t    battery_level; 
    uint32_t    network_strength; 
    bool        is_warn_triggered; 
    bool        is_server_connected; 
    uint32_t    i; 
    
    // Get all data showed on the menu. 
    battery_level       = (battery_percent * BATTERY_LEVEL_DIVIDER) / 100;
    is_warn_triggered   = M95_status.fatal_err; 
    is_server_connected = MQTT_status.mqtt_is_conn && MQTT_status.mqtt_is_open; 
    network_strength    = (M95_status.signal_strength * MAX_NETWORK_STRENGTH) / MAX_RSSI_VAL; 
    
    // Draw left, right menu and horizontal bars. 
    display_img(x, y, MENU_LEFT_WIDTH, MENU_LEFT_HEIGHT, MENU_LEFT_ASSET); 
    display_img(x + DISPLAY_WIDTH - MENU_RIGHT_WIDTH, y, MENU_RIGHT_WIDTH, MENU_RIGHT_HEIGHT, MENU_RIGHT_ASSET); 
    display_fast_h_line(x, y, DISPLAY_WIDTH, MAX_INTENSITY); 
    display_fast_h_line(x, y + DISPLAY_HEIGHT - 1, DISPLAY_WIDTH, MAX_INTENSITY); 
    
    // Draw the battery level indicator. 
    for (i = 0; i < battery_level; i += 1)
        display_fast_h_line(
                x + 2, 
                y + DISPLAY_HEIGHT - 3 - i * 3, 
                y + 6, 
                ((i * BATTERY_COLOR_DIVIDER_COUNT) / BATTERY_LEVEL_DIVIDER) * QUARTER_INTENSITY
            ); 
    
    // Display the battery level. 
    display_printf(x, y + 1, MIN_INTENSITY, 0, "%d%%", battery_percent); 
    
    // Draw warning status icon if any event raised the waring flag. 
    if (is_warn_triggered)
        display_img(x + 14, y + 19, WARNING_STATUS_WIDTH, WARNING_STATUS_HEIGHT, WARNING_STATUS_ASSET); 

    // Draw the server connected icon. 
    display_img(x + 11, y + 31, SERVER_STATUS_WIDTH, SERVER_STATUS_HEIGHT, SERVER_STATUS[is_server_connected]); 
    
    // Draw the network strength icon. 
    display_img(x + 12, y + 39, NETWORK_STATUS_WIDTH, NETWORK_STATUS_HEIGHT, NETWORK_STATUS[network_strength % MAX_NETWORK_STRENGTH]); 
    
    // Draw the speaker active icon. 
    display_img(x + 12, y + 51, SPEAKER_STATUS_WIDTH, SPEAKER_STATUS_HEIGHT, SPEAKER_STATUS[speaker_is_active]); 
    return; 
}


void draw_measurement_widget(uint32_t x, uint32_t y, const MEASURE_WIDGET_t* measure_widget)
{ 
    uint32_t            value_len; 
    uint32_t            unit_len; 
    char                buffer[16]; 
    
    // If no widget is given, abort.
    if (!measure_widget)
        return; 
    
    
    // Get the length of strings that will be drawn onto the screen, used to 
    // center those strings. 
    if (measure_widget->val_type == FLOAT)
        value_len = sprintf(buffer, "%." DECIMAL_COUNT "f", *(measure_widget->measurement.as_float));
    
    else if (measure_widget->val_type == INTEGER)
        value_len = sprintf(buffer, "%d", *(measure_widget->measurement.as_int));

    unit_len = strlen(measure_widget->unit); 
    
    if (measure_widget->icon)
        display_img(
            x + 2, 
            y + 4, 
            measure_widget->icon_size, 
            measure_widget->icon_size, 
            measure_widget->icon
        ); 
    
    // Draw the title. 
    display_draw_str(
        x + 2 + ((measure_widget->icon) ? (WIDGET_ICON_WIDTH + 5) : 0), 
        y, 
        measure_widget->title, 
        MAX_INTENSITY, 
        FONT_10X16
    ); 
    
    // Draw the measurement value. 
    display_draw_str(
        x + (MEASURE_WIDGET_WIDTH / 2) - (value_len * FONT_10X12_WIDTH) / 2, 
        (MEASURE_WIDGET_HEIGHT / 2) - (FONT_10X12_HEIGHT / 2), 
        buffer, 
        MAX_INTENSITY, 
        FONT_10X16_BOLD
    ); 
    
    // Draw the unit. 
    display_draw_str(
        (x + (MEASURE_WIDGET_WIDTH / 2) - (value_len * FONT_10X12_WIDTH) / 2) \
            + value_len * FONT_10X12_WIDTH - unit_len * FONT_6X8_WIDTH + 5, 
        MEASURE_WIDGET_HEIGHT / 2 + (FONT_10X12_HEIGHT / 2) + 2,
        measure_widget->unit, 
        MAX_INTENSITY, 
        FONT_6X8
    ); 
    return; 
}


void draw_settings_widget(uint32_t x, uint32_t y, const SETTING_WIDGET_t* widget)
{
    uint32_t action_name_len; 
    
    // If no widget is provided, abort. 
    if (!widget)
        return; 
    
    
    action_name_len = strlen(widget->action.name); 
    
    // Draws widget icon. 
    if (widget->icon)
        display_img(
            x, y + 5, widget->icon_size, 
            widget->icon_size, widget->icon
        ); 
        
    // Draws widget title. 
    display_draw_str(
        x + WIDGET_ICON_WIDTH + 5, y, widget->title, 
        MAX_INTENSITY, FONT_10X16_BOLD
    ); 

    display_img(
        x + (SETTINGS_WIDGET_WIDTH / 2) - (widget->action.icon_size / 2), 
        y + FONT_10X12_HEIGHT + 2, widget->action.icon_size, 
        widget->action.icon_size, widget->action.icon
    ); 
    
    display_draw_str(
        x + (SETTINGS_WIDGET_WIDTH / 2) - ((action_name_len * FONT_10X12_WIDTH) / 2),
        y + FONT_10X12_HEIGHT + widget->action.icon_size + 4, 
        widget->action.name, MAX_INTENSITY, FONT_10X16
    ); 
}