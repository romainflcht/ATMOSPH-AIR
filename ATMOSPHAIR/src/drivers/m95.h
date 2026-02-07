#ifndef _M95_H_
#define _M95_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include <string.h>


//* _ DEFINITIONS ______________________________________________________________

#define MQTT_DEVICE_NAME            "atmosphAIR"
#define MQTT_DEVICE_USER            ""
#define MQTT_DEVICE_PASSWD          ""
#define MQTT_SERVER_URL             "atmosphair.duckdns.org"
#define MQTT_SERVER_PORT            "45678"

#define RESPONSE_BUFFER_SIZE        512
#define MAX_TX_COMMAND_SIZE         35
#define COMMAND_TIMEOUT_MS          10000
#define M95_COMMAND_END_CHAR        "\r\n"
#define OPERATOR_NAME_BUF_LENGTH    20


#define M95_INIT_CONFIG         X("AT" M95_COMMAND_END_CHAR,                10) \
                                X("ATE0" M95_COMMAND_END_CHAR,              10) \
                                X("AT+IPR=115200" M95_COMMAND_END_CHAR,     10) \
                                X("AT+CMEE=1" M95_COMMAND_END_CHAR,         10) \
                                X("AT+CRC=0" M95_COMMAND_END_CHAR,          10) \
                                X("AT+CNMI=0,0,0,0,0" M95_COMMAND_END_CHAR, 10) \
                                X("AT+CREG=0" M95_COMMAND_END_CHAR,         10) \
                                X("AT+CGREG=0" M95_COMMAND_END_CHAR,        10) \
                                X("AT&W" M95_COMMAND_END_CHAR,              10) \
                                X("AT+QIDEACT" M95_COMMAND_END_CHAR,        300) \
                                X("AT+QIFGCNT=0" M95_COMMAND_END_CHAR,      300) \
                                X("AT+QICSGP=1,\"orange\",\"orange\",\"\"" M95_COMMAND_END_CHAR, 300) \
                                X("AT+QIREGAPP" M95_COMMAND_END_CHAR,       300)


#define M95_AT_COMMANDS         X(AT,      "AT" M95_COMMAND_END_CHAR, false)            \
                                X(CPIN,    "AT+CPIN?" M95_COMMAND_END_CHAR, false)      \
                                X(CSQ,     "AT+CSQ" M95_COMMAND_END_CHAR, false)        \
                                X(QSPN,    "AT+QSPN" M95_COMMAND_END_CHAR, false)       \
                                X(QIACT,   "AT+QIACT" M95_COMMAND_END_CHAR, false)      \
                                X(QIDEACT, "AT+QIDEACT" M95_COMMAND_END_CHAR, false)    \
                                X(QISTAT,  "AT+QISTAT" M95_COMMAND_END_CHAR, true)      \
                                X(QMTOPEN, "AT+QMTOPEN=0,\"" MQTT_SERVER_URL "\"," MQTT_SERVER_PORT M95_COMMAND_END_CHAR, true)    \
                                X(QMTCONN, "AT+QMTCONN=0,\"" MQTT_DEVICE_NAME "\",\"" MQTT_DEVICE_USER "\",\"" MQTT_DEVICE_PASSWD "\"" M95_COMMAND_END_CHAR, true)    \
                                X(QMTPUB,  "AT+QMTPUB=0,0,0,0," M95_COMMAND_END_CHAR, true)   \
                                X(QMTDISC, "AT+QMTDISC=0" M95_COMMAND_END_CHAR, true)

#define CONTAINS(buf, str)      (strstr(buf, str)) 

#define IS_ALPHA_CHAR(c)        ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))

//* _ ENUMERATIONS _____________________________________________________________

typedef enum m95_write_states
{
    M95_IDLE, 
    M95_ASK_SIM_DATA,
    M95_VERIFY_SIM_DATA,
    M95_ASK_SIGNAL_STRENGTH,
    M95_VERIFY_SIGNAL_STRENGTH,
    M95_ASK_OPERATOR,
    M95_VERIFY_OPERATOR,
            
    M95_ASK_GPRS_START, 
    M95_VERIFY_GPRS_START, 
    M95_ASK_GPRS_STATUS, 
    M95_VERIFY_GPRS_STATUS, 
    M95_ASK_MQTT_OPEN, 
    M95_VERIFY_MQTT_OPEN, 
            
    M95_WRITE_END, 
            
            
}   M95_WRITE_STATES_t;


typedef enum m95_read_states
{
    M95_RESPONSE_IDLE, 
    M95_RESPONSE_GET,
    M95_RESPONSE_PROCESS, 
}   M95_READ_STATES_t;


typedef enum at_command_id
{
    AT,         ///< Ping to the module. 
    CPIN,       ///< Check the sim car status. 
    CSQ,        ///< Get the signal strength. 
    QSPN,       ///< Get the operator name. 
            
    QIACT,      ///< Activate the GPRS and TCPIP stack. 
    QIDEACT,    ///< Deactivate the GPRS and TCPIP stack. 
    QISTAT,     ///< Check the GPRS and TCPIP stack status.
            
    QMTOPEN,    ///< Open the MQTT connection.
    QMTCONN,    ///< Connect the module to the MQTT broker. 
    QMTPUB,     ///< Publish data to a MQTT topic. 
    QMTDISC,    ///< Close the MQTT connection. 
            
    NULL_COMMAND, 
}   AT_COMMAND_ID_t;


typedef enum at_command_status
{
    OK, 
    ERROR,         
    NOT_PROCESSED, 
}   AT_COMMAND_STATUS_t;


typedef enum sim_status
{
    NOT_INSERTED, 
    LOCKED, 
    READY, 
}   SIM_STATUS_t;


//* _ STRUCTURES _______________________________________________________________

typedef struct at_command
{
    const AT_COMMAND_ID_t   id; 
    const char*             command; 
    const size_t            length; 
    const bool              is_post_resp; 
}   AT_COMMAND_t;


typedef struct tx_data
{
    AT_COMMAND_STATUS_t status; 
    const AT_COMMAND_t* last_command; 
    uint32_t            last_transmit_timestamp; 
}   TX_DATA_t;


typedef struct rx_data
{
    volatile uint32_t   buf_index;
    uint8_t             buf[RESPONSE_BUFFER_SIZE];
    volatile bool       waiting_process; 
}   RX_DATA_t;


typedef struct m95_status
{
    SIM_STATUS_t    sim_status; 
    uint8_t         signal_strength; 
    char            operator_name[OPERATOR_NAME_BUF_LENGTH]; 
    uint8_t         operator_name_length; 
}   M95_STATUS_t;


typedef struct mqtt_conn_status
{
    bool gprs_is_up; 
    bool mqtt_is_open; 
    bool mqtt_is_conn; 
}   MQTT_CONN_STATUS_t;

//* _ FUNCTION DECLARATIONS ____________________________________________________

void M95_init(void); 

void M95_write_task(void); 

void M95_read_task(void); 

#endif