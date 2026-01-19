#ifndef _UTILS_H_
#define _UTILS_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 


#define CRC_8_POLYNOMIAL    0x31
#define CRC_8_INIT_VAL      0xFF
#define CRC_8_SIZE          8

//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn uint8_t crc_8_check(const uint8_t* data, uint32_t length); 
/// @brief calculate the CRC 8 Dallas/Maxim of an array of bytes. You can 
///        either calculate the CRC and compare it the the one given by the 
///        sensor or execute this function on the data + the CRC and check if 
///        the result is 0.
/// @param data array of bytes.
/// @param length of the byte array. 
/// @return the CRC code calculated. 
uint8_t crc_8_check(const uint8_t* data, uint32_t length); 

#endif