#ifndef _ASSESTS_H_
#define _ASSESTS_H_

//* _ INCLUDES _________________________________________________________________

#include <stdlib.h>
#include "definitions.h"

#include "../drivers/ssd1362.h"


//* _ DEFINITIONS ______________________________________________________________

#define MENU_LEFT_WIDTH             34
#define MENU_LEFT_HEIGHT            DISPLAY_HEIGHT

#define MENU_RIGHT_WIDTH            14
#define MENU_RIGHT_HEIGHT           DISPLAY_HEIGHT

#define WARNING_STATUS_WIDTH        9
#define WARNING_STATUS_HEIGHT       9

#define NETWORK_STATUS_WIDTH        11
#define NETWORK_STATUS_HEIGHT       9

#define SERVER_STATUS_WIDTH         14
#define SERVER_STATUS_HEIGHT        5

#define SPEAKER_STATUS_WIDTH        12
#define SPEAKER_STATUS_HEIGHT       10

#define MEASURE_WIDGET_ICON_WIDTH   12
#define MEASURE_WIDGET_ICON_HEIGHT  12


//* _ EXTERN ASSETS DECLARATIONS _______________________________________________

extern const uint8_t    MENU_LEFT_ASSET[];
extern const uint8_t    MENU_RIGHT_ASSET[];
extern const uint8_t    WARNING_STATUS_ASSET[];
extern const uint8_t    NETWORK_0_ASSET[];
extern const uint8_t    NETWORK_1_ASSET[];
extern const uint8_t    NETWORK_2_ASSET[];
extern const uint8_t    NETWORK_3_ASSET[];
extern const uint8_t    NETWORK_4_ASSET[];
extern const uint8_t    SERVER_0_ASSET[];
extern const uint8_t    SERVER_1_ASSET[];
extern const uint8_t    SPEAKER_0_ASSET[];
extern const uint8_t    SPEAKER_1_ASSET[];
extern const uint8_t    FLAMMABLE_GASES_ICON_ASSET[]; 
extern const uint8_t    PM_ICON_ASSET[]; 
extern const uint8_t    NOX_ICON_ASSET[]; 
extern const uint8_t    VOC_ICON_ASSET[]; 
extern const uint8_t    TEMP_ICON_ASSET[]; 
extern const uint8_t    RH_ICON_ASSET[]; 
extern const uint8_t    CO2_H2S_ICON_ASSET[];
extern const uint8_t    CO_ICON_ASSET[]; 
extern const uint8_t    O2_ICON_ASSET[]; 
extern const uint8_t    BATTERY_ICON_ASSET[]; 

extern const uint8_t    SETTINGS_ICON_ASSET[]; 

extern const uint8_t*   NETWORK_STATUS[5];
extern const uint8_t*   SERVER_STATUS[2];
extern const uint8_t*   SPEAKER_STATUS[2];

#endif