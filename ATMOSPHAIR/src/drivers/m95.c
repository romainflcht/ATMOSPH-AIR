#include "m95.h"


//* _ STATIC VARIABLES _________________________________________________________

static M95_WRITE_STATES_t   curr_write_state = M95_WRITE_IDLE; 
static M95_READ_STATES_t    curr_read_state  = M95_READ_IDLE; 

static TX_DATA_t            tx_data = {
    .id           = NULL_COMMAND, 
    .is_processed = true, 
}; 

static RX_DATA_t            rx_data = {
    .buf_index       = 0, 
    .buf             = {0}, 
    .waiting_process = false,
}; 

static const AT_COMMAND_t   AT_LUT[] = {
    #define X(id, command)  {id, command, sizeof(command)}, 
    
        M95_AT_COMMANDS
    #undef X
};

//* _ STATIC FUNCTION DECLARATIONS _____________________________________________

// Write state functions.

static void M95_WRITE_IDLE_state(void); 
static void M95_WRITE_SIM_DATA_state(void);

// Read state functions.

static void M95_READ_IDLE_state(void); 
static void M95_READ_RESPONSE_state(void); 
static void M95_READ_PROCESS_COMMAND_state(void); 

// Utility functions. 

static void M95_response_buffer_reset(void); 


//* _  FUNCTION IMPLEMENTATION _________________________________________________


void M95_init(void)
{
    uint8_t init_command[MAX_TX_COMMAND_SIZE]; 
    uint8_t dummy[RESPONSE_BUFFER_SIZE]; 
    
    // Wait for previous commands to be sent. 
    while (SERCOM0_USART_WriteCountGet() > 0); 
    
    // Send initialization commands.
    #define X(command, size)    strncpy(init_command, command, size);       \
                                SERCOM0_USART_Write(init_command, size);    \
                                SYSTICK_DelayMs(50); 
        
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
        case M95_WRITE_IDLE:
            M95_WRITE_IDLE_state(); 
            break; 
            
        case M95_WRITE_SIM_DATA_SEND: 
            M95_WRITE_SIM_DATA_state(); 
            break;
        
        case M95_WRITE_SIM_DATA_VERIFY: 
            break; 
            
        default: 
            curr_write_state = M95_WRITE_IDLE;
            break; 
    }
    
    return; 
}


static void M95_WRITE_IDLE_state(void)
{
    curr_write_state = M95_WRITE_SIM_DATA_SEND; 
    return; 
}


static void M95_WRITE_SIM_DATA_state(void)
{
    const AT_COMMAND_t* to_send; 
    size_t              retval; 
    
    // Get the command data to send. 
    to_send = &AT_LUT[SIM_READY]; 
    
    // Not enough space in write ring buffer, abort. 
    if (SERCOM0_USART_WriteFreeBufferCountGet() < to_send->length)
        return; 
    
    // Send the command and check if is was correctly written to the write 
    // ring buffer. 
    retval = SERCOM0_USART_Write((uint8_t*)to_send->command, to_send->length); 
    if (retval != to_send->length)
        return; 
    
    tx_data.id = to_send->id; 
    tx_data.is_processed = false; 
    curr_write_state = M95_WRITE_SIM_DATA_VERIFY; 
    return; 
}


void M95_read_task(void)
{
    switch (curr_read_state)
    {
        case M95_READ_IDLE: 
            M95_READ_IDLE_state(); 
            break; 
            
        case M95_READ_RESPONSE:
            M95_READ_RESPONSE_state(); 
            break; 
            
        case M95_READ_PROCESS_COMMAND:
            break; 
            
        default:
            curr_read_state = M95_READ_IDLE; 
    }
}


//* _ STATES FUNCTION IMPLEMENTATION ___________________________________________

static void M95_READ_IDLE_state(void)
{
    curr_read_state = M95_READ_RESPONSE; 
    return; 
}


static void M95_READ_RESPONSE_state(void)
{
    uint8_t byte; 
    size_t  retval; 
    
    // If not data is available, abort. 
    if (SERCOM0_USART_ReadCountGet() < 1 && rx_data.waiting_process)
        return; 
    
    // Read one byte and process it. 
    retval = SERCOM0_USART_Read(&byte, 1);
    if (retval < 1)
        return; 
    
    // Check if the received byte is a separator, if so set the command waiting 
    // flag and go to the next state. 
    if (byte == '\r' || byte == '\n')
    {
        if (rx_data.buf_index < 1)
            return; 
        
        rx_data.waiting_process = true; 
        curr_read_state = M95_READ_PROCESS_COMMAND; 
        return; 
    }
    
    rx_data.buf[rx_data.buf_index] = byte; 
    rx_data.buf_index += 1; 
}


static void M95_READ_PROCESS_COMMAND_state(void)
{
    
    // Check if a command is waiting, if not abort the command processing and go
    // back to the previous state. 
    if (!rx_data.waiting_process)
    {
        curr_read_state = M95_READ_RESPONSE; 
        return; 
    }
    
    // Check for the command response. 
    

    
    // Clear the response buffer. 
    M95_response_buffer_reset(); 
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