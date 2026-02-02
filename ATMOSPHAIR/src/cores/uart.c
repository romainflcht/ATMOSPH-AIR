#include "uart.h"


bool SERCOM0_USART_WriteIsBusy(void)
{
    return SERCOM0_USART_WriteCountGet() > 0; 
}