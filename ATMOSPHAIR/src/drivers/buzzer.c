#include "buzzer.h"


//* _ GLOBAL VARIABLE DECLARATIONS _____________________________________________

bool speaker_is_active = 1; 


//* _ GLOBAL MELODY LUT ________________________________________________________

const NOTE_t BOOT_MELODY[] = {
    {.frequency = C7,         .duration = 75},
    {.frequency = FS7,         .duration = 75},
    {.frequency = B7,         .duration = 75},
    {.frequency = MELODY_EOP,   .duration = MELODY_EOP},
};

const NOTE_t ERR_MELODY[] = {
    {.frequency = B3,           .duration = 75},
    {.frequency = 0,            .duration = 75},
    {.frequency = B3,           .duration = 125},
    {.frequency = MELODY_EOP,   .duration = MELODY_EOP},
};

const NOTE_t UI_MELODY[] = {
    {.frequency = FS6,          .duration = 10},
    {.frequency = MELODY_EOP,   .duration = MELODY_EOP},
};

const NOTE_t DANGER_MELODY[] = {
    {.frequency = A4,           .duration = 250},
    {.frequency = A5,           .duration = 25},
    {.frequency = MELODY_EOP,   .duration = MELODY_EOP},
};


//* _ STATIC VARIABLE DECLARATIONS _____________________________________________

static uint32_t         last_note_timestamp = 0; 
static BUZZER_STATE_t   curr_state          = BUZZER_IDLE; 
static const NOTE_t*    curr_melody         = NULL; 


//* _ STATIC FUNCTION DECLARATIONS _____________________________________________

static void BUZZER_PLAY_NOTE_state(void); 
static void BUZZER_WAIT_NOTE_state(void); 
static void BUZZER_STOP_state(void); 


void BUZZER_task(void)
{
    switch(curr_state)
    {
        case BUZZER_IDLE:
            break; 
        
        case BUZZER_PLAY_NOTE:
            BUZZER_PLAY_NOTE_state(); 
            break; 
        
        case BUZZER_WAIT_NOTE:
            BUZZER_WAIT_NOTE_state(); 
            break; 
        
        case BUZZER_STOP:
            BUZZER_STOP_state(); 
            break; 
    }
}


void BUZZER_play_melody(const NOTE_t* melody)
{
    // If a melody is already playing, abort. 
    if (curr_state != BUZZER_IDLE || !speaker_is_active)
        return; 
    
    // Start the TCC, load the melody and go the the playing state. 
    TCC0_PWMStart(); 
    curr_melody = melody; 
    curr_state  = BUZZER_PLAY_NOTE; 
    return; 
}


void BUZZER_toggle_mute(void)
{
    speaker_is_active = !speaker_is_active; 
}


static void BUZZER_PLAY_NOTE_state(void)
{
    // If no melody is set, reset the state machine. 
    if (!curr_melody)
    {
        curr_state  = BUZZER_STOP; 
        curr_melody = NULL; 
        return; 
    }
    
    // Else play the note and go to the next state. 
    TCC0_PWM24bitPeriodSet(TONE_FREQ(curr_melody->frequency));
    last_note_timestamp = SYSTICK_millis(); 
    curr_state = BUZZER_WAIT_NOTE; 
    return; 
}


static void BUZZER_WAIT_NOTE_state(void)
{
    // If no melody is set, reset the state machine. 
    if (!curr_melody)
    {
        curr_state = BUZZER_STOP; 
        return; 
    }
    
    // Else, wait for the last note to be played. 
    if (SYSTICK_millis() - last_note_timestamp < curr_melody->duration)
        return; 
    
    // Get the next note, if the note is an EOF, reset the state machine. 
    curr_melody += 1; 
    if (curr_melody->frequency == MELODY_EOP && curr_melody->duration == MELODY_EOP)
        curr_state  = BUZZER_STOP; 
    else
        curr_state = BUZZER_PLAY_NOTE; 
    
    return; 
}


static void BUZZER_STOP_state(void)
{
    // Reached the end of the melody, turn off TCC peripheral and reset the 
    // state machine. 
    curr_melody = NULL; 
    curr_state = BUZZER_IDLE; 
    last_note_timestamp = 0;
    TCC0_PWMStop(); 
}