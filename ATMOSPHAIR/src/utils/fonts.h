#ifndef _FONTS_H_
#define _FONTS_H_

#include "definitions.h"

//* _ DEFINITIONS ______________________________________________________________

#define FONT_HEADER_SIZE        4
#define FONT_FIRST_CHAR_OFFSET  0x20
#define FONT_CHAR_WIDTH         6
#define FONT_CHAR_HEIGHT        8

extern const uint8_t FONT_6x8[]; 
extern const uint8_t FONT_10x16[]; 
extern const uint8_t FONT_10x16_BOLD[]; 


typedef enum fonts
{
    FONT_6X8, 
    FONT_10X16, 
    FONT_10X16_BOLD, 
}   FONTS_t;

#endif