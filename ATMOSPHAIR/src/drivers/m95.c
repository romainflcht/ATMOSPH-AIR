#include "m95.h"
#include "cores/systick.h"

//* _ GLOBAL VARIABLES _________________________________________________________

M95_STATUS_t M95_status = {0};

//* _ STATIC VARIABLES _________________________________________________________

static M95_WRITE_STATES_t   curr_write_state = M95_IDLE; 
static M95_READ_STATES_t    curr_read_state  = M95_RESPONSE_IDLE; 

static const AT_COMMAND_t   AT_LUT[] = {
    #define X(id, command, is_post_resp)  \
        {id, command, sizeof(command) - 1, is_post_resp}, 
    
        M95_AT_COMMANDS
    #undef X
};

static TX_DATA_t            tx_data = {
    .last_command            = NULL, 
    .status                  = NOT_PROCESSED, 
    .last_transmit_timestamp = 0, 
}; 

static RX_DATA_t            rx_data = {
    .buf_index       = 0, 
    .buf             = {0}, 
    .waiting_process = false,
}; 


//* _ STATIC FUNCTION DECLARATIONS _____________________________________________

// Write state functions.

static void M95_WRITE_IDLE_state(void); 
static void M95_WRITE_COMMAND_state(const AT_COMMAND_t* to_send); 
static void M95_VERIFY_COMMAND_state(M95_WRITE_STATES_t on_success, M95_WRITE_STATES_t on_fail);
static void M95_PUBLISH_PAYLOAD_state(void); 
static void M95_WAIT_NEXT_PUBLISH_state(void); 

// Read state functions.

static void M95_READ_IDLE_state(void); 
static void M95_READ_RESPONSE_state(void); 
static void M95_READ_PROCESS_COMMAND_state(void); 

// Utility functions. 

static void M95_response_buffer_reset(void); 
static void M95_transmit_buffer_reset(void); 
static void M95_parse_sim_status(const uint8_t* buf); 
static void M95_parse_signal_strength(const uint8_t* buf); 

/// @fn static void M95_parse_operator_name(const uint8_t* buf); 
/// @brief this function parse string like: 
///        +QSPN: "","","OPERATOR",0,"20801"
///        it only retrieve the OPERATOR string. 
/// @param buf buffer that contains the string. 
static void M95_parse_operator_name(const uint8_t* buf); 
static void M95_parse_gprs_status(const uint8_t* buf); 
static void M95_parse_mqtt_open(const uint8_t* buf); 
static void M95_parse_mqtt_conn(const uint8_t* buf); 
static void M95_parse_mqtt_publish(const uint8_t* buf); 

//* _  FUNCTION IMPLEMENTATION _________________________________________________


void M95_init(void)
{
    uint8_t init_command[MAX_TX_COMMAND_SIZE]; 
    uint8_t dummy[RESPONSE_BUFFER_SIZE]; 
    
    GSM_PWRKEY_Set(); 
    SYSTICK_DelayMs(2000); 
    GSM_PWRKEY_Clear(); 
    
    SYSTICK_DelayMs(5000); 

    GSM_PWRKEY_Set(); 
    SYSTICK_DelayMs(2000); 
    GSM_PWRKEY_Clear(); 
    
    // Wait for previous commands to be sent. 
    while (SERCOM0_USART_WriteCountGet() > 0); 
    
    // Send initialization commands.
    #define X(command, wait)    strncpy(init_command, command, sizeof(command));    \
                                SERCOM0_USART_Write(init_command, sizeof(command) - 1); \
                                SYSTICK_DelayMs(wait); 
        
        M95_INIT_CONFIG
    #undef X

    // Clear response data for each initialization command before starting 
    // both state machines. 
    if (SERCOM0_USART_ReadCountGet() > 0)
        SERCOM0_USART_Read(dummy, RESPONSE_BUFFER_SIZE); 
    
    return; 
}


void M95_write_task(void)
{
    switch (curr_write_state)
    {
        case M95_IDLE:
            M95_WRITE_IDLE_state(); 
            break; 
            
        case M95_ASK_SIM_DATA: 
            M95_WRITE_COMMAND_state(&AT_LUT[CPIN]); 
            break;
        
        case M95_VERIFY_SIM_DATA: 
            M95_VERIFY_COMMAND_state(M95_ASK_SIGNAL_STRENGTH, M95_ASK_SIM_DATA); 
            break; 
            
        case M95_ASK_SIGNAL_STRENGTH:
            M95_WRITE_COMMAND_state(&AT_LUT[CSQ]); 
            break; 
            
        case M95_VERIFY_SIGNAL_STRENGTH:
            M95_VERIFY_COMMAND_state(M95_ASK_OPERATOR, M95_ASK_SIGNAL_STRENGTH); 
            break; 
        
        case M95_ASK_OPERATOR:
            M95_WRITE_COMMAND_state(&AT_LUT[QSPN]); 
            break; 
            
        case M95_VERIFY_OPERATOR:
            M95_VERIFY_COMMAND_state(M95_IDLE, M95_ASK_OPERATOR);  
            break; 
            
        // MQTT. 
        case M95_ASK_GPRS_START:
            M95_WRITE_COMMAND_state(&AT_LUT[QIACT]); 
            break; 
            
        case M95_VERIFY_GPRS_START:
            M95_VERIFY_COMMAND_state(M95_ASK_GPRS_STATUS, M95_ASK_GPRS_STOP); 
            break; 
        
        case M95_ASK_GPRS_STOP:
            M95_WRITE_COMMAND_state(&AT_LUT[QIDEACT]); 
            break; 
            
        case M95_VERIFY_GPRS_STOP:
            M95_VERIFY_COMMAND_state(M95_ASK_GPRS_START, M95_ASK_GPRS_STOP); 
            break; 
        
        case M95_ASK_GPRS_STATUS:
            M95_WRITE_COMMAND_state(&AT_LUT[QISTAT]); 
            break; 
            
        case M95_VERIFY_GPRS_STATUS:
            M95_VERIFY_COMMAND_state(M95_ASK_MQTT_OPEN, M95_ASK_GPRS_STATUS); 
            break; 
        
        case M95_ASK_MQTT_OPEN:
            M95_WRITE_COMMAND_state(&AT_LUT[QMTOPEN]); 
            break; 
            
        case M95_VERIFY_MQTT_OPEN:
            M95_VERIFY_COMMAND_state(M95_ASK_MQTT_CONN, M95_ASK_MQTT_OPEN); 
            break; 
        
        case M95_ASK_MQTT_CONN:
            M95_WRITE_COMMAND_state(&AT_LUT[QMTCONN]); 
            break; 
            
        case M95_VERIFY_MQTT_CONN:
            M95_VERIFY_COMMAND_state(M95_ASK_MQTT_PUBLISH, M95_ASK_MQTT_CONN); 
            break; 
        
        case M95_ASK_MQTT_PUBLISH:
            M95_WRITE_COMMAND_state(&AT_LUT[QMTPUB_DATA]); 
            break; 
            
        case M95_VERIFY_MQTT_PUBLISH:
            M95_VERIFY_COMMAND_state(M95_PUBLISH_PAYLOAD, M95_ASK_MQTT_PUBLISH); 
            break; 
        
        case M95_PUBLISH_PAYLOAD:
            M95_PUBLISH_PAYLOAD_state(); 
            break; 
            
        case M95_VERIFY_PAYLOAD:
            M95_VERIFY_COMMAND_state(M95_WAIT_NEXT_PUBLISH, M95_ASK_MQTT_PUBLISH); 
            break; 
            
        case M95_WAIT_NEXT_PUBLISH: 
            M95_WAIT_NEXT_PUBLISH_state(); 
            break; 

        case M95_WRITE_END: 
        default: 
            curr_write_state = M95_IDLE;
            break; 
    }
    
    return; 
}


static void M95_WRITE_IDLE_state(void)
{
    static bool is_init = true; 
    
    if (is_init)
        is_init = false; 
    
    else if (SYSTICK_millis() - tx_data.last_transmit_timestamp < COMMAND_TIMEOUT_MS)
        return; 
    
    curr_write_state = M95_ASK_GPRS_START; 
    return; 
}


static void M95_WRITE_COMMAND_state(const AT_COMMAND_t* to_send)
{
    size_t retval; 
    
    // Not enough space in write ring buffer, abort. 
    if (SERCOM0_USART_WriteFreeBufferCountGet() < to_send->length)
        return; 
    
    // Send the command and check if it was correctly written to the write 
    // ring buffer. 
    retval = SERCOM0_USART_Write((uint8_t*)to_send->command, to_send->length); 
    if (retval != to_send->length)
        return; 
    
    // Saves last command sent data and go to the next state. 
    tx_data.last_command = to_send; 
    tx_data.status = NOT_PROCESSED; 
    tx_data.last_transmit_timestamp = SYSTICK_millis(); 
    curr_write_state += 1; 
    return; 
}


static void M95_VERIFY_COMMAND_state(M95_WRITE_STATES_t on_success, M95_WRITE_STATES_t on_fail)
{
    // The last command send has not been processed yet, abort. 
    if (tx_data.status == NOT_PROCESSED)
    {
        // If the command has not been processed after the timeout delay, 
        // reset the state machine.
        if (SYSTICK_millis() - tx_data.last_transmit_timestamp >= COMMAND_TIMEOUT_MS)
        {
            curr_write_state = M95_IDLE; 
            M95_transmit_buffer_reset(); 
        }
        
        return; 
    }

    // If an error is detected, go back one state. 
    else if (tx_data.status == ERROR)
    {
        if (curr_write_state > 0)
            curr_write_state = on_fail; 
        
        M95_transmit_buffer_reset(); 
        return; 
    }
    
    // No error detected. 
    curr_write_state = on_success; 
    
    // Reset the transmit buffer. 
    M95_transmit_buffer_reset(); 
    return; 
}


static void M95_PUBLISH_PAYLOAD_state(void)
{
    size_t      retval; 
    uint8_t     payload[MAX_TX_COMMAND_SIZE]; 
    uint32_t    payload_len;
    
    
    payload_len = snprintf(payload, MAX_TX_COMMAND_SIZE, "{\"temp\":%d,\"rh\":%d}" M95_PUBLISH_SEND_CHAR, 12, 50); 
    
    
    // Not enough space in write ring buffer, abort. 
    if (SERCOM0_USART_WriteFreeBufferCountGet() < payload_len)
        return; 
    
    // Send the command and check if it was correctly written to the write 
    // ring buffer. 
    retval = SERCOM0_USART_Write((uint8_t*)payload, payload_len); 
    if (retval != payload_len)
        return; 
    
    // Saves last command sent data and go to the next state. 
    tx_data.last_command = &AT_LUT[QMTPUB_DATA]; 
    tx_data.status = NOT_PROCESSED; 
    tx_data.last_transmit_timestamp = SYSTICK_millis(); 
    curr_write_state += 1; 
}


static void M95_WAIT_NEXT_PUBLISH_state(void)
{
    if (SYSTICK_millis() - tx_data.last_transmit_timestamp < COMMAND_TIMEOUT_MS)
        return; 
    
    curr_write_state = M95_ASK_MQTT_PUBLISH; 
    return; 
}


//* _ READ STATE MACHINES ______________________________________________________

void M95_read_task(void)
{
    switch (curr_read_state)
    {
        case M95_RESPONSE_IDLE: 
            M95_READ_IDLE_state(); 
            break; 
            
        case M95_RESPONSE_GET:
            M95_READ_RESPONSE_state(); 
            break; 
            
        case M95_RESPONSE_PROCESS:
            M95_READ_PROCESS_COMMAND_state(); 
            break; 
            
        default:
            curr_read_state = M95_RESPONSE_IDLE; 
    }
}


//* _ STATES FUNCTION IMPLEMENTATION ___________________________________________

static void M95_READ_IDLE_state(void)
{
    curr_read_state = M95_RESPONSE_GET; 
    return; 
}


static void M95_READ_RESPONSE_state(void)
{
    uint8_t character; 
    size_t  retval; 
    
    // If not data is available, abort. 
    if (SERCOM0_USART_ReadCountGet() < 1 && rx_data.waiting_process)
        return; 
    
    // Read one character and process it. 
    retval = SERCOM0_USART_Read(&character, 1);
    if (retval < 1)
        return; 
    
    // Check if the received byte is a separator, if so set the command waiting 
    // flag and go to the next state. 
    if (character == '\r' || character == '\n')
    {
        if (rx_data.buf_index < 1)
            return; 
        
        rx_data.waiting_process = true; 
        curr_read_state = M95_RESPONSE_PROCESS; 
        return; 
    }
    
    else if (character == '>')
    {
        rx_data.buf[rx_data.buf_index] = character; 
        
        rx_data.waiting_process = true; 
        curr_read_state = M95_RESPONSE_PROCESS;
        return;
    }
    
    rx_data.buf[rx_data.buf_index] = character; 
    rx_data.buf_index += 1; 
}


static void M95_READ_PROCESS_COMMAND_state(void)
{
    // Check if a command is waiting, if not abort the command processing and go
    // back to the previous state. 
    if (!rx_data.waiting_process)
    {
        M95_response_buffer_reset();
        curr_read_state = M95_RESPONSE_GET; 
        return; 
    }
    
    // Check for the command response and process it. 
    if (CONTAINS(rx_data.buf, "OK"))
    {
        if (!tx_data.last_command || !tx_data.last_command->is_post_resp)
            tx_data.status = OK; 
    }
    
    // Error parsing. 
    else if (CONTAINS(rx_data.buf, "ERROR"))
    {
        tx_data.status = ERROR; 
        switch(tx_data.last_command->id)
        {
            case CPIN:
                M95_status.sim_status = NOT_INSERTED; 
                break; 
                
            case CSQ:
                M95_status.signal_strength = 0; 
                break; 
        }
    }
    
    // SIM status data parsing. 
    else if (CONTAINS(rx_data.buf, "+CPIN: "))
        M95_parse_sim_status(rx_data.buf); 
    
    // Signal strength data parsing. 
    else if (CONTAINS(rx_data.buf, "+CSQ: "))
        M95_parse_signal_strength(rx_data.buf); 
    
    // Operator name data parsing. 
    else if (CONTAINS(rx_data.buf, "+QSPN: "))
        M95_parse_operator_name(rx_data.buf); 
    
    // GPRS stop parsing. 
    else if (CONTAINS(rx_data.buf, "DEACT OK"))
        tx_data.status = OK; 
    
    // GPRS status parsing. 
    else if (CONTAINS(rx_data.buf, "STATE: "))
        M95_parse_gprs_status(rx_data.buf); 
    
    // MQTT open result parsing. 
    else if (CONTAINS(rx_data.buf, "+QMTOPEN: "))
        M95_parse_mqtt_open(rx_data.buf); 
    
    // MQTT connection result parsing. 
    else if (CONTAINS(rx_data.buf, "+QMTCONN: "))
        M95_parse_mqtt_conn(rx_data.buf); 
    
    // MQTT publish result parsing. 
    else if (CONTAINS(rx_data.buf, "+QMTPUB: "))
        M95_parse_mqtt_publish(rx_data.buf); 
    
    // Prepare the MQTT publish. 
    else if (CONTAINS(rx_data.buf, ">"))
        tx_data.status = OK; 


    // Clear the response buffer. 
    M95_response_buffer_reset(); 
    
    // Go back to the read response state to get the next response. 
    curr_read_state = M95_RESPONSE_GET; 
    return; 
}


//* _ UTILITY FUNCTIONS ________________________________________________________

static void M95_response_buffer_reset(void)
{
    uint32_t i; 
    
    // Reset the command parsing buffer to get the next response to parse. 
    for (i = 0; i < rx_data.buf_index; i += 1)
        rx_data.buf[i] = 0; 
    
    rx_data.waiting_process = false; 
    rx_data.buf_index = 0; 
}


static void M95_transmit_buffer_reset(void)
{
    tx_data.last_command = NULL; 
    tx_data.status = NOT_PROCESSED; 
    tx_data.last_transmit_timestamp = SYSTICK_millis(); 
    return; 
}


static void M95_parse_sim_status(const uint8_t* buf)
{
    if (CONTAINS(buf, "SIM PIN"))
    {
        M95_status.sim_status = LOCKED; 
        return; 
    }
        
    M95_status.sim_status = READY; 
    return;  
}


static void M95_parse_signal_strength(const uint8_t* buf)
{
    uint32_t rssi; 
    
    rssi = atoi(buf + 6); 
    
    // We received 99 which indicate that the signal strength is unknown.
    if (rssi > 31)
        M95_status.signal_strength = 0;
    
    // Get the signal strength value. 
    else
        M95_status.signal_strength = rssi; 
    
    return; 
}


static void M95_parse_operator_name(const uint8_t* buf)
{
    uint32_t    i; 
    uint32_t    j; 
    uint32_t    comma_count; 
    
    // Find the operator name field in the response. 
    i = 0; 
    comma_count = 0; 
    while (buf[i] != '\0' && comma_count < 2) 
    {
        if (buf[i] == ',')
            comma_count += 1;
        i += 1;
    }
    
    // Operator name found, skip " and spaces before getting the operator name. 
    while (buf[i] != '\0' && (buf[i] == ' ' || buf[i] == '"')) 
        i++;
    
    // Get the operator name. 
    j = 0; 
    while (buf[i] != '\0' && buf[i] != '"' && j < OPERATOR_NAME_BUF_LENGTH)
    {
        M95_status.operator_name[j] = buf[i]; 
        i += 1; 
        j += 1; 
    }
    
    // Get the operator name length for easy display and manipulation. 
    M95_status.operator_name_length = j + 1; 
    return; 
}


static void M95_parse_gprs_status(const uint8_t* buf)
{
    if (CONTAINS(buf, "IP GPRSACT"))
        tx_data.status = OK; 
    
    else
        tx_data.status = ERROR;
    
    return;
}


static void M95_parse_mqtt_open(const uint8_t* buf)
{
    int32_t  retval;
    uint8_t* response; 
    
    response = strchr((const char*)buf, ','); 
    
    
    if (!response)
    {
        tx_data.status = ERROR; 
        return; 
    }
    
    retval = atoi(response + 1);
    
    if (retval == 0)
        tx_data.status = OK; 
    
    else
        tx_data.status = ERROR; 
    
    return;
}


static void M95_parse_mqtt_conn(const uint8_t* buf)
{
    uint32_t i; 
    uint32_t retvals[3];
    uint8_t* response; 
    
    
    response = strchr(buf, ':'); 
    if (!response)
    {
        tx_data.status = ERROR; 
        return; 
    }
    
    response += 1; 
    i = 0; 
    while (i < 3)
    {
        retvals[i] = atoi(response); 
        
        response = strchr(response, ','); 
        if (!buf)
        {
            tx_data.status = ERROR; 
            return; 
        }
        
        response += 1; 
        i += 1; 
    }

    if (retvals[1] == 0 && retvals[2] == 0)
        tx_data.status = OK; 
    
    else
        tx_data.status = ERROR; 
        
    return;
}


static void M95_parse_mqtt_publish(const uint8_t* buf)
{
    uint32_t i; 
    uint32_t retvals[3];
    uint8_t* response; 
    
    
    response = strchr(buf, ':'); 
    if (!response)
    {
        tx_data.status = ERROR; 
        return; 
    }
    
    response += 1; 
    i = 0; 
    while (i < 3)
    {
        retvals[i] = atoi(response); 
        
        response = strchr(response, ','); 
        if (!buf)
        {
            tx_data.status = ERROR; 
            return; 
        }
        
        response += 1; 
        i += 1; 
    }

    if (retvals[1] == 0 && retvals[2] == 0)
        tx_data.status = OK; 
    
    else
        tx_data.status = ERROR; 
        
    return;
}