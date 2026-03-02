#ifndef _BUZZER_H_
#define _BUZZER_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../utils/notes.h"

//* _ DEFINITIONS ______________________________________________________________

#define MELODY_EOP      0

#define TONE_FREQ(freq) ((freq == 0) ? 0 : CPU_CLOCK_FREQUENCY / freq)

//* _ ENUMERATION DEFINITIONS __________________________________________________

typedef enum buzzer_state
{
    BUZZER_IDLE, 
    BUZZER_PLAY_NOTE, 
    BUZZER_WAIT_NOTE, 
    BUZZER_STOP, 
}   BUZZER_STATE_t;

//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef struct note
{
    uint32_t frequency;
    uint32_t duration; 
}   NOTE_t;


void BUZZER_task(void); 
void BUZZER_play_melody(const NOTE_t* melody); 

#endif