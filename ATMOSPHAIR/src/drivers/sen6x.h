#ifndef _SEN6X_H_
#define _SEN6X_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../cores/systick.h"
#include "../utils/utils.h"

//* _ DEFINITIONS ______________________________________________________________

#define SEN6X_ADDR              0x6B

#define SEN6X_RX_BUF_LENGTH     40
#define SEN6X_COMMAND_LENGTH    2

#define SEN6X_INIT_DELAY_MS     2500
#define SEN6X_INIT_CONFIG       X(0x0021) \
                                X(0xD304)

#define UINT_16_UNKNOWN_VAL     0xFFFF
#define INT_16_UNKNOWN_VAL      0x7FFF

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
    START_MEASUREMENT       = 0x0021, 
    STOP_MEASUREMENT        = 0x0104, 
    GET_DATA_READY          = 0x0202,
    READ_MEASURED_SEN65     = 0x0446, 
    READ_MEASURED_SEN66     = 0x0300, 
    READ_RAW_MEASURED_SEN65 = 0x0455, 
    READ_RAW_MEASURED_SEN66 = 0x0405, 
}   SEN6X_COMMAND_t;


typedef struct sen65_data
{
    float PM_1_0; 
    float PM_2_5; 
    float PM_4_0; 
    float PM_10_0;
    float humidity; 
    float temp;
    float VOC;
    float NOx;
}   SEN65_DATA_t;

//* _ FUNCTION DECLARATIONS ____________________________________________________

void sen6x_init(void); 

void sen6x_task(void); 


#endif