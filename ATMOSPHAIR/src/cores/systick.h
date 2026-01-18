#ifndef _SYSTICK_H_
#define _SYSTICK_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

//* _ FUNCTION DECLARATIONS ____________________________________________________


/// @fn void SYSTICK_init(void); 
/// @brief initialize the SYSTICK peripheral by registering the callback 
///        function and starting the peripheral timer. 
void SYSTICK_init(void); 


/// @fn uint32_t systick_millis(void); 
/// @brief Getter for the current millisecond counter value. 
/// @return the millisecond counter value. 
uint32_t SYSTICK_millis(void); 

#endif