#include "utils.h"


//* _ GENERAL UTILITY FUNCTIONS ________________________________________________

uint8_t crc_8_check(const uint8_t* data, uint32_t length)
{
    uint8_t crc; 
    int i; 
    int j; 
    
    crc = CRC_8_INIT_VAL; 
    
    // CRC-8-Dallas/Maxim check algorithm, see the page 57 of the SENSIRION 
    // SEN6x sensor. 
    for (i = 0; i < length; i += 1)
    {
        crc ^= data[i]; 
        
        for (j = 0; j < CRC_8_SIZE; j += 1)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC_8_POLYNOMIAL; 
            
            else
                crc = (crc << 1);
        }
    }
    
    return crc; 
}