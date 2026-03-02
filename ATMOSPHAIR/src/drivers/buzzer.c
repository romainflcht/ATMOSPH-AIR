#include "buzzer.h"
#include "cores/systick.h"

static uint32_t         last_note_timestamp = 0; 
static BUZZER_STATE_t   curr_state          = BUZZER_IDLE; 
static const NOTE_t*    curr_melody         = NULL; 

static void BUZZER_IDLE_state(void); 
static void BUZZER_PLAY_NOTE_state(void); 
static void BUZZER_WAIT_NOTE_state(void); 
static void BUZZER_STOP_state(void); 

//* _ MELODY LUT _______________________________________________________________

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
    {.frequency = 1550,          .duration = 35},
    {.frequency = MELODY_EOP,   .duration = MELODY_EOP},
};

void BUZZER_task(void)
{
    switch(curr_state)
    {
        case BUZZER_IDLE:
            BUZZER_IDLE_state(); 
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
    if (curr_state != BUZZER_IDLE)
        return; 
    
    TCC0_PWMStart(); 
    curr_melody = melody; 
    curr_state  = BUZZER_PLAY_NOTE; 
    return; 
}


static void BUZZER_IDLE_state(void)
{
    return; 
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
    curr_melody = NULL; 
    curr_state = BUZZER_IDLE; 
    last_note_timestamp = 0;
    TCC0_PWMStop(); 
}