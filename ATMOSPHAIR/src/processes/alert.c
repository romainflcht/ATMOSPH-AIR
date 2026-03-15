#include "alert.h"


ALERT_DETECTION_t               alert_detected;


static const ALERT_THRESHOLD_t  DATA_THRESHOLD[] = {
    {
        .high_threshold = PM_0_5_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.PM_0_5), 
    }, 
    {
        .high_threshold = PM_1_0_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.PM_1_0), 
    },
    {
        .high_threshold = PM_2_5_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.PM_2_5), 
    },
    {
        .high_threshold = PM_4_0_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.PM_4_0), 
    },
    {
        .high_threshold = PM_10_0_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.PM_10_0), 
    },
    {
        .high_threshold = RH_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.humidity), 
    },
    {
        .high_threshold = TEMP_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.temp), 
    },
    {
        .high_threshold = VOC_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.VOC), 
    },
    {
        .high_threshold = NOX_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.NOx), 
    },
    {
        .high_threshold = CO2_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.CO2), 
    },
    {
        .high_threshold = HCHO_ALERT_THRESHOLD, 
        .data = &(SEN6X_data.HCHO), 
    },
};

void check_alert_threshold(void)
{
    uint32_t                 i; 
    const ALERT_THRESHOLD_t* current_data; 
    
    
    for (i = 0; i < ARRAY_SIZE(DATA_THRESHOLD); i += 1)
    {
        current_data = &(DATA_THRESHOLD[i]); 
        
        if (current_data->data)
        {
            if (*(current_data->data) > current_data->high_threshold)
                alert_detected.alert |= (1 << i); 
            
            else 
                alert_detected.alert &= ~(1 << i); 
        }
    }

    return; 
}