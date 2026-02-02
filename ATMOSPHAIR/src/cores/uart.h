#ifndef _UART_H_
#define _UART_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 


//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn bool SERCOM0_USART_IsBusy(void); 
/// @brief tells if the UART write peripheral is busy sending data. 
/// @return true if the peripheral is busy, false is not. 
bool SERCOM0_USART_WriteIsBusy(void); 

#endif