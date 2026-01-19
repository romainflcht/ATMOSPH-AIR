#include "sen6x.h"

//* _ GLOBAL VARIABLE __________________________________________________________

SEN6X_DATA_t            sen6x_data; 

//* _ STATIC VARIABLES _________________________________________________________

static SEN6X_STATE_t    curr_state                      = IDLE; 
static uint8_t          rx_buffer[SEN6X_RX_BUF_LENGTH]  = {0}; 
static uint8_t          tx_buffer[SEN6X_COMMAND_LENGTH] = {0}; 
static uint16_t         last_command_executed           = 0; 
static uint32_t         last_command_timestamp          = 0; 

//* _ STATIC FUNCTION DECLARATIONS _____________________________________________

// TODO: Write functions descriptor comment. 
static void     IDLE_state(void); 
static void     MEASUREMENT_state(void); 
static void     WAIT_DATA_state(void); 
static void     READ_DATA_state(void); 
static void     PARSE_DATA_state(void); 

static void     sen6x_data_init(SEN6X_DATA_t* data);
static uint16_t get_command_wait_time(uint16_t command); 
static void     process_data(uint8_t* src, float divider, bool is_signed, float* dest); 


//* _  FUNCTION IMPLEMENTATION _________________________________________________

void sen6x_init(void)
{
    uint8_t command[2]; 
    
    sen6x_data_init(&sen6x_data);
    
    // Wait for the I²C peripheral to be available. 
    while (SERCOM1_I2C_IsBusy()); 

    #define X(reg)  command[0] = (reg >> 8);                                        \
                    command[1] = (reg & 0xFF);                                      \
                    SERCOM1_I2C_Write(SEN6X_ADDR, command, SEN6X_COMMAND_LENGTH);   \
                    while (SERCOM1_I2C_IsBusy());                                   \
                    SYSTICK_DelayMs(DEVICE_RESET_WAIT_TIME);                                          

        SEN6X_INIT_CONFIG
    #undef X

    return;
}


void sen6x_task(void)
{
    int i; 
    switch (curr_state)
    {
        case IDLE:
            IDLE_state();
            break; 
            
        case MEASUREMENT:
            MEASUREMENT_state(); 
            break; 
            
        case WAIT_DATA_W:
        case WAIT_DATA_R:
            WAIT_DATA_state();
            break; 
                
        case READ_DATA_W:
        case READ_DATA_R:
            READ_DATA_state();
            break; 
            
        case PARSE_DATA: 
            PARSE_DATA_state(); 
            break; 
            
        default: 
            for(i = 0; i < 24; i++ )
            {
                printf("%d, ", rx_buffer[i]); 
                printf("\r\n"); 
            }
            break; 

    }
    
    return; 
}


//* _ STATES FUNCTION IMPLEMENTATION ___________________________________________

static void IDLE_state(void)
{
    curr_state = MEASUREMENT; 
    return; 
}


static void MEASUREMENT_state(void)
{
    bool retval; 
    
    retval = false; 
    
    // Abort if the I²C bus is busy. 
    if (SERCOM1_I2C_IsBusy())
        return; 
    
    // Build the command to send. 
    tx_buffer[0] = (uint8_t)(START_MEASUREMENT >> 8);
    tx_buffer[1] = (uint8_t)(START_MEASUREMENT & 0xFF);
    
    // Send the command to the I²C bus. 
    retval = SERCOM1_I2C_Write(SEN6X_ADDR, tx_buffer, SEN6X_COMMAND_LENGTH); 
    if (!retval)
        return; 
    
    // If the command has been successfully sent, save the timestamp and go to 
    // the next state. 
    last_command_executed  = START_MEASUREMENT; 
    last_command_timestamp = SYSTICK_millis(); 
    curr_state             = WAIT_DATA_W; 
    return; 
}


static void WAIT_DATA_state(void)
{
    bool        retval; 
    uint16_t    wait_time; 
    
    retval    = false; 
    wait_time = get_command_wait_time(last_command_executed); 
    
    // Wait for the DATA_READY command to be fully executed on the sensor side. 
    if (SYSTICK_millis() - last_command_timestamp <= wait_time)
        return; 
    
    // Abort if the I²C bus is busy. 
    if (SERCOM1_I2C_IsBusy())
        return; 
    
    // Send the command to the I²C bus. 
    if (curr_state == WAIT_DATA_W)
    {
        // Build the command to send. 
        tx_buffer[0] = (uint8_t)(GET_DATA_READY >> 8);
        tx_buffer[1] = (uint8_t)(GET_DATA_READY & 0xFF);
        retval = SERCOM1_I2C_Write(SEN6X_ADDR, tx_buffer, SEN6X_COMMAND_LENGTH);  
    }
    
    else if (curr_state == WAIT_DATA_R)
        retval = SERCOM1_I2C_Read(SEN6X_ADDR, rx_buffer, 3); 
    
    if (!retval)
        return; 
    
    // If the command has been successfully sent, save the timestamp and go to 
    // the next state. 
    last_command_executed  = GET_DATA_READY;
    last_command_timestamp = SYSTICK_millis(); 
    curr_state             += 1; 
    return; 
}


static void READ_DATA_state(void)
{
    bool        retval; 
    uint8_t     crc_check; 
    uint16_t    wait_time; 
    
    retval = false; 
    wait_time = get_command_wait_time(last_command_executed); 
    
    // Wait for the READ_MEASURED_SEN6x command to be fully executed on the 
    // sensor side. 
    if (SYSTICK_millis() - last_command_timestamp <= 25)
        return; 
    
    // Abort if the I²C bus is busy. 
    if (SERCOM1_I2C_IsBusy())
        return; 
    
    // Data is not ready, go back to the WAIT_DATA state. 
    if (curr_state == READ_DATA_W && rx_buffer[1] == 0)
    {
        curr_state = WAIT_DATA_W; 
        return; 
    }
    
    // Send the command to the I²C bus. 
    if (curr_state == READ_DATA_W)
    {
        // Build the command to send. 
        tx_buffer[0] = (uint8_t)(READ_MEASURED >> 8);
        tx_buffer[1] = (uint8_t)(READ_MEASURED & 0xFF);
        retval = SERCOM1_I2C_Write(SEN6X_ADDR, tx_buffer, SEN6X_COMMAND_LENGTH); 
    }
    
    else if (curr_state == READ_DATA_R)
        retval = SERCOM1_I2C_Read(SEN6X_ADDR, rx_buffer, SEN6X_MEASUREMENT_LENGTH); 

    if (!retval)
        return; 
    
    // If the command has been successfully sent, save the timestamp and go to 
    // the next state. 
    last_command_executed  = READ_MEASURED; 
    last_command_timestamp = SYSTICK_millis(); 
    curr_state             += 1; 
    return; 
}


static void PARSE_DATA_state(void)
{
    if (SYSTICK_millis() - last_command_timestamp <= 25)
        return; 
    
    #if SEN_DEVICE_USED == SEN60
        process_data(rx_buffer + 0, 10, false, &(sen6x_data.PM_1_0)); 
        process_data(rx_buffer + 3, 10, false, &(sen6x_data.PM_2_5)); 
        process_data(rx_buffer + 6, 10, false, &(sen6x_data.PM_4_0)); 
        process_data(rx_buffer + 9, 10, false, &(sen6x_data.PM_10_0));
        process_data(rx_buffer + 12, 100, true, &(sen6x_data.PM_0_5));  
    
    #elif SEN_DEVICE_USED == SEN63C
        process_data(rx_buffer + 0, 10, false, &(sen6x_data.PM_1_0)); 
        process_data(rx_buffer + 3, 10, false, &(sen6x_data.PM_2_5)); 
        process_data(rx_buffer + 6, 10, false, &(sen6x_data.PM_4_0)); 
        process_data(rx_buffer + 9, 10, false, &(sen6x_data.PM_10_0));

        process_data(rx_buffer + 12, 100, true, &(sen6x_data.humidity)); 
        process_data(rx_buffer + 15, 200, true, &(sen6x_data.temp)); 
        process_data(rx_buffer + 18, 1, true, &(sen6x_data.CO2)); 
    
    #elif SEN_DEVICE_USED == SEN65
        process_data(rx_buffer + 0, 10, false, &(sen6x_data.PM_1_0)); 
        process_data(rx_buffer + 3, 10, false, &(sen6x_data.PM_2_5)); 
        process_data(rx_buffer + 6, 10, false, &(sen6x_data.PM_4_0)); 
        process_data(rx_buffer + 9, 10, false, &(sen6x_data.PM_10_0));

        process_data(rx_buffer + 12, 100, true, &(sen6x_data.humidity)); 
        process_data(rx_buffer + 15, 200, true, &(sen6x_data.temp)); 
        process_data(rx_buffer + 18, 10, true, &(sen6x_data.VOC)); 
        process_data(rx_buffer + 21, 10, true, &(sen6x_data.NOx)); 
    
    #elif SEN_DEVICE_USED == SEN66
        process_data(rx_buffer + 0, 10, false, &(sen6x_data.PM_1_0)); 
        process_data(rx_buffer + 3, 10, false, &(sen6x_data.PM_2_5)); 
        process_data(rx_buffer + 6, 10, false, &(sen6x_data.PM_4_0)); 
        process_data(rx_buffer + 9, 10, false, &(sen6x_data.PM_10_0));

        process_data(rx_buffer + 12, 100, true, &(sen6x_data.humidity)); 
        process_data(rx_buffer + 15, 200, true, &(sen6x_data.temp)); 
        process_data(rx_buffer + 18, 10, true, &(sen6x_data.VOC)); 
        process_data(rx_buffer + 21, 10, true, &(sen6x_data.NOx)); 
        process_data(rx_buffer + 24, 1, false, &(sen6x_data.CO2)); 
    
    
    #elif SEN_DEVICE_USED == SEN68
        process_data(rx_buffer + 0, 10, false, &(sen6x_data.PM_1_0)); 
        process_data(rx_buffer + 3, 10, false, &(sen6x_data.PM_2_5)); 
        process_data(rx_buffer + 6, 10, false, &(sen6x_data.PM_4_0)); 
        process_data(rx_buffer + 9, 10, false, &(sen6x_data.PM_10_0));

        process_data(rx_buffer + 12, 100, true, &(sen6x_data.humidity)); 
        process_data(rx_buffer + 15, 200, true, &(sen6x_data.temp)); 
        process_data(rx_buffer + 18, 10, true, &(sen6x_data.VOC)); 
        process_data(rx_buffer + 21, 10, true, &(sen6x_data.NOx)); 
        process_data(rx_buffer + 24, 10, false, &(sen6x_data.HCHO)); 
    
    #endif
    
    last_command_executed = 0; 
    curr_state = WAIT_DATA_W; 
    return; 
}


//* _ UTILITY FUNCTIONS ________________________________________________________

static void sen6x_data_init(SEN6X_DATA_t* data)
{
    uint32_t i; 
    
    // Set all values to 0 at device startup. 
    for (i = 0; i < SEN6X_RX_BUF_LENGTH; i += 1)
        rx_buffer[i] = 0; 

    data->PM_1_0   = 0; 
    data->PM_2_5   = 0; 
    data->PM_4_0   = 0; 
    data->humidity = 0; 
    data->temp     = 0; 
    data->VOC      = 0; 
    data->NOx      = 0; 
    data->CO2      = 0; 
    data->HCHO     = 0; 
    return; 
}


static uint16_t get_command_wait_time(uint16_t command)
{
    uint16_t wait_time; 
    
    wait_time = 0; 
    switch (command)
    {
        case DEVICE_RESET: 
            wait_time = DEVICE_RESET_WAIT_TIME; 
            break; 
            
        case START_MEASUREMENT: 
            wait_time = DEVICE_START_WAIT_TIME; 
            break; 
            
        case STOP_MEASUREMENT: 
            wait_time = DEVICE_STOP_WAIT_TIME; 
            break; 
            
        case GET_DATA_READY: 
        case READ_MEASURED: 
            wait_time = DEVICE_READ_WAIT_TIME; 
            break; 
    }
    
    return wait_time + WAIT_TIME_SAFETY_MS; 
}


static void process_data(uint8_t* src, float divider, bool is_signed, float* dest)
{
    uint8_t     calculated_crc; 
    uint16_t    raw_data;
    
    // Check the CRC provided by the sensor to validate data integrity. 
    calculated_crc = crc_8_check(src, 3); 
    if (calculated_crc != 0)
        return; 
    
    // Calculate the two bytes data. 
    raw_data = (src[0] << 8 | src[1]); 
    
    // Check if the sensor sent relevant data. 
    if (!is_signed && raw_data == UINT_16_UNKNOWN_VAL)
        return; 
    
    else if (is_signed && raw_data == INT_16_UNKNOWN_VAL)
        return; 
    
    // Saves data to the correct location. 
    if (is_signed)
        *dest = (int16_t)raw_data / divider; 
    
    else
        *dest = (uint16_t)raw_data / divider; 
        
    return; 
}