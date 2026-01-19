#ifndef _SEN6X_H_
#define _SEN6X_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../cores/systick.h"
#include "../utils/utils.h"

//* _ DEFINITIONS ______________________________________________________________

#define SEN60                           100
#define SEN63C                          101
#define SEN65                           102
#define SEN66                           103
#define SEN68                           104

/// @define SEN_DEVICE_USED
/// @brief this defines the SEN device used. Multiple options are available:
///         - SEN60  -> PM sensor. 
///         - SEN63C -> PM, RH, T and CO2 sensor. 
///         - SEN65  -> PM, RH, T, VOC and NOx sensor. 
///         - SEN66  -> PM, RH, T, CO2, VOC and NOx sensor.
///         - SEN68  -> PM, RH, T, VOC, NOx and HCHO sensor. 
#define SEN_DEVICE_USED                 SEN65

// Buffer configuration. 
#define SEN6X_RX_BUF_LENGTH             27
#define SEN6X_COMMAND_LENGTH            2

// Initialization. 
#define SEN6X_INIT_CONFIG               X(STOP_MEASUREMENT) \
                                        X(DEVICE_RESET)
// Timing. 
#define WAIT_TIME_SAFETY_MS             5
#define DEVICE_RESET_WAIT_TIME          1200
#define DEVICE_START_WAIT_TIME          50
#define DEVICE_STOP_WAIT_TIME           1000
#define DEVICE_READ_WAIT_TIME           20

// Constant. 
#define UINT_16_UNKNOWN_VAL             0xFFFF
#define INT_16_UNKNOWN_VAL              0x7FFF


// Device specific configuration. 
#if SEN_DEVICE_USED == SEN60
    #define SEN6X_ADDR                  0x6C
    #define SEN6X_MEASUREMENT_LENGTH    15

#elif SEN_DEVICE_USED == SEN63C
    #define SEN6X_ADDR                  0x6B
    #define SEN6X_MEASUREMENT_LENGTH    21

#elif SEN_DEVICE_USED == SEN65
    #define SEN6X_ADDR              0x6B
    #define SEN6X_MEASUREMENT_LENGTH    24

#elif SEN_DEVICE_USED == SEN66
    #define SEN6X_ADDR              0x6B
    #define SEN6X_MEASUREMENT_LENGTH    27

#elif SEN_DEVICE_USED == SEN68
    #define SEN6X_ADDR              0x6B
    #define SEN6X_MEASUREMENT_LENGTH    27

#else
    #error "~[ERR: DEVICE SELECTION] No SENx sensor selected."

#endif


//* _ ENUMERATIONS _____________________________________________________________

/// @enum SEN6X_STATE_t
/// @brief enumerate all available states of the SEN6x state machine. 
typedef enum sen6x_states
{
    IDLE,
    MEASUREMENT,
    WAIT_DATA_W,
    WAIT_DATA_R,
    READ_DATA_W, 
    READ_DATA_R, 
    PARSE_DATA, 
}   SEN6X_STATE_t;


/// @enum SEN6X_COMMAND_t
/// @brief enumerate available commands to operate the SEN6x sensor. 
typedef enum sen6x_command
{
    #if SEN_DEVICE_USED == SEN60
        DEVICE_RESET            = 0x3F8D, 
        START_MEASUREMENT       = 0x2152, 
        STOP_MEASUREMENT        = 0x3F86, 
        GET_DATA_READY          = 0xE4B8,
    #else
        DEVICE_RESET            = 0xD304, 
        START_MEASUREMENT       = 0x0021, 
        STOP_MEASUREMENT        = 0x0104, 
        GET_DATA_READY          = 0x0202,
    #endif
            
    #if SEN_DEVICE_USED == SEN60
        READ_MEASURED       = 0xEC05, 
        
    #elif SEN_DEVICE_USED == SEN63C
        READ_MEASURED       = 0x0471, 
        
    #elif SEN_DEVICE_USED == SEN65
        READ_MEASURED       = 0x0446, 
        
    #elif SEN_DEVICE_USED == SEN66
        READ_MEASURED       = 0x0300, 
        
    #elif SEN_DEVICE_USED == SEN68
        READ_MEASURED       = 0x0767, 
        
    #endif
}   SEN6X_COMMAND_t;


typedef struct sen6x_data
{
    float PM_0_5; 
    float PM_1_0; 
    float PM_2_5; 
    float PM_4_0; 
    float PM_10_0;
    float humidity; 
    float temp;
    float VOC;
    float NOx;
    float CO2; 
    float HCHO; 
}   SEN6X_DATA_t;

//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn void sen6x_init(void); 
/// @ brief initialize any SEN6x device. 
void sen6x_init(void); 

/// @fn void sen6x_task(void); 
/// @brief maintains the sensor measurement reading state machine. 
void sen6x_task(void); 


#endif