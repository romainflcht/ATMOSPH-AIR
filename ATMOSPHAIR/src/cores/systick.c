#include "systick.h"

//* _ STATIC VARIABLE DECLARATIONS _____________________________________________

// Millisecond timer, contains the running time of the CPU in millisecond. 
volatile static uint32_t systick_ms_counter = 0;


//* _ STATIC FUNCTION DECLARATIONS _____________________________________________

/// @fn void systick_overflow_callback(); 
/// @brief increment the millisecond counter each systick timer overflow 
///        (callback). 
/// @param context not used. 
static void systick_overflow_callback(uintptr_t context); 


//* _ FUNCTION IMPLEMENTATION __________________________________________________

void SYSTICK_init(void)
{
    SYSTICK_TimerCallbackSet(systick_overflow_callback, (uintptr_t)NULL); 
    SYSTICK_TimerStart();
}


uint32_t SYSTICK_millis(void)
{
    // Getter for the millisecond counter. 
    return (uint32_t)systick_ms_counter; 
}


//* _ STATIC FUNCTION IMPLEMENTATION ___________________________________________

static void systick_overflow_callback(uintptr_t context)
{
    // Increment to millisecond counter on each systick timer overflow. 
    systick_ms_counter += 1; 
    return; 
}