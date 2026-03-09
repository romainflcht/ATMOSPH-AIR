#ifndef _BUZZER_H_
#define _BUZZER_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../utils/notes.h"
#include "../cores/systick.h"

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


//* _ EXTERN VARIABLES _________________________________________________________

extern bool speaker_is_active; 
extern const NOTE_t BOOT_MELODY[]; 
extern const NOTE_t ERR_MELODY[]; 
extern const NOTE_t UI_MELODY[]; 


//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn void BUZZER_task(void); 
/// @brief maintains the buzzer state machine. 
void BUZZER_task(void); 

/// @fn void BUZZER_play_melody(const NOTE_t* melody); 
/// @brief initiate playing a given melody.
/// @param melody the melody that will be played. 
void BUZZER_play_melody(const NOTE_t* melody); 


/// @fn void BUZZER_toggle_mute(void); 
/// @brief toggle the mute variable to disable buzzer beeping. 
void BUZZER_toggle_mute(void); 

#endif