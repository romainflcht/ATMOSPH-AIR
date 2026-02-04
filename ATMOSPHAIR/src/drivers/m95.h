#ifndef _M95_H_
#define _M95_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../cores/uart.h"
#include <string.h>


//* _ DEFINITIONS ______________________________________________________________

#define M95_COMMAND_END_CHAR    "\r\n"
#define RX_BUFFER_SIZE          512
#define RESPONSE_BUFFER_SIZE    RX_BUFFER_SIZE
#define MAX_TX_COMMAND_SIZE     20

#define M95_INIT_CONFIG         X("ATE0" M95_COMMAND_END_CHAR,               6) \
                                X("AT+CMEE=1" M95_COMMAND_END_CHAR,         11) \
                                X("AT+CRC=0" M95_COMMAND_END_CHAR,          10) \
                                X("AT+CNMI=0,0,0,0,0" M95_COMMAND_END_CHAR, 19) \
                                X("AT+CREG=0" M95_COMMAND_END_CHAR,         11) \
                                X("AT+CGREG=0" M95_COMMAND_END_CHAR,        12)


#define M95_AT_COMMANDS         X(AT, "AT" M95_COMMAND_END_CHAR, STATUS)                    \
                                X(SIM_STATUS, "AT+CPIN?" M95_COMMAND_END_CHAR, STATUS)      \
                                X(SIGNAL_STRENGTH, "AT+CSQ" M95_COMMAND_END_CHAR, STATUS)   \


#define CONTAINS(buf, str)      strstr(buf, str) 

//* _ ENUMERATIONS _____________________________________________________________


typedef enum m95_read_states
{
    M95_READ_IDLE, 
    M95_READ_RESPONSE,
    M95_READ_PROCESS_COMMAND, 
}   M95_READ_STATES_t;


typedef enum m95_write_states
{
    M95_WRITE_IDLE, 
    M95_WRITE_SIM_DATA_SEND,
    M95_WRITE_SIM_DATA_VERIFY,
    M95_WRITE_SIGNAL_STRENGTH_SEND,
    M95_WRITE_SIGNAL_STRENGTH_VERIFY,
}   M95_WRITE_STATES_t;


typedef enum at_command_id
{
    AT, 
    SIM_STATUS,
    SIGNAL_STRENGTH, 
    NULL_COMMAND, 
}   AT_COMMAND_ID_t;


typedef enum at_command_status
{
    OK, 
    ERROR,         
    NOT_PROCESSED, 
}   AT_COMMAND_STATUS_t;


typedef enum at_response_type
{
    STATUS, 
    NUMERICAL,
}   AT_RESPONSE_TYPE_t;


typedef enum sim_status
{
    NOT_INSERTED, 
    LOCKED, 
    READY, 
}   SIM_STATUS_t;


//* _ STRUCTURES _______________________________________________________________

typedef struct tx_data
{
    AT_COMMAND_ID_t     id; 
    AT_COMMAND_STATUS_t status; 
}   TX_DATA_t;


typedef struct rx_data
{
    volatile uint32_t   buf_index;
    uint8_t             buf[RESPONSE_BUFFER_SIZE];
    volatile bool       waiting_process; 
}   RX_DATA_t;


typedef struct at_command
{
    const AT_COMMAND_ID_t       id; 
    const char*                 command; 
    const size_t                length; 
    const AT_RESPONSE_TYPE_t    response_type; 
}   AT_COMMAND_t;


typedef struct m95_status
{
    SIM_STATUS_t    sim_status; 
    uint8_t         signal_strength; 
}   M95_STATUS_t;


//* _ FUNCTION DECLARATIONS ____________________________________________________

void M95_init(void); 

void M95_write_task(void); 

void M95_read_task(void); 

#endif