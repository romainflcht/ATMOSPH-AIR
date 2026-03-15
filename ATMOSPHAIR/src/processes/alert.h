#ifndef _ALERT_H_
#define _ALERT_H_


//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include <stdio.h>
#include "../drivers/sen6x.h"
#include "../utils/utils.h"

#define PM_0_5_ALERT_THRESHOLD          10.0f   // ug/m3
#define PM_1_0_ALERT_THRESHOLD          10.0f   // ug/m3
#define PM_2_5_ALERT_THRESHOLD          25.0f   // ug/m3
#define PM_4_0_ALERT_THRESHOLD          30.0f   // ug/m3
#define PM_10_0_ALERT_THRESHOLD         50.0f   // ug/m3

#define RH_ALERT_THRESHOLD              60.0f   // %
#define TEMP_ALERT_THRESHOLD            45.0f   // °C

#define VOC_ALERT_THRESHOLD             200.0f  // Index
#define NOX_ALERT_THRESHOLD             200.0f  // Index
#define CO2_ALERT_THRESHOLD             5000.0f // PPM
#define HCHO_ALERT_THRESHOLD            100.0f  // PPB

#define CO_ALERT_THRESHOLD              100.0f  // PPM
#define O2_ALERT_THRESHOLD              100.0f  // PPM
#define H2S_ALERT_THRESHOLD             100.0f  // PPM
#define FLAMMABLE_GASES_ALERT_THRESHOLD 100.0f  // PPB



typedef struct alert_threshold
{
    float*  data; 
    float   low_threshold; 
    float   high_threshold; 
}   ALERT_THRESHOLD_t;


typedef union alert_detection
{
    uint32_t alert; 
    struct 
    {
        uint8_t pm_0_5  : 1; 
        uint8_t pm_1_0  : 1; 
        uint8_t pm_2_5  : 1; 
        uint8_t pm_4_0  : 1; 
        uint8_t pm_10_0 : 1; 
        uint8_t rh      : 1; 
        uint8_t temp    : 1; 
        uint8_t voc     : 1; 
        uint8_t nox     : 1; 
        uint8_t co2     : 1; 
        uint8_t hcho    : 1; 
        uint32_t dummy  : 21; 
    };
}   ALERT_DETECTION_t;


extern ALERT_DETECTION_t alert_detected;


void check_alert_threshold(void); 

#endif 