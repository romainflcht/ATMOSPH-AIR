#ifndef _FONTS_H_
#define _FONTS_H_

#include "definitions.h"

//* _ DEFINITIONS ______________________________________________________________

#define FONT_HEADER_SIZE    4
#define HEADER_CHAR_SPACING 0
#define HEADER_FONT_WIDTH   1
#define HEADER_FONT_HEIGHT  2
#define HEADER_FONT_OFFSET  3

#define FONT_6X8_WIDTH      FONT_6x8[HEADER_FONT_WIDTH]
#define FONT_6X8_HEIGHT     FONT_6x8[HEADER_FONT_HEIGHT]
#define FONT_10X12_WIDTH    FONT_10x16[HEADER_FONT_WIDTH]
#define FONT_10X12_HEIGHT   FONT_10x16[HEADER_FONT_HEIGHT]


//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef enum fonts
{
    FONT_6X8,           ///< See fonts.c for more informations. 
    FONT_10X16,         ///< Consola bold. 
    FONT_10X16_BOLD,    ///< Consola Mono bold. 
}   FONTS_t;


//* _ EXTERN FONT DECLARATIONS _________________________________________________

extern const uint8_t FONT_6x8[]; 
extern const uint8_t FONT_10x16[]; 
extern const uint8_t FONT_10x16_BOLD[]; 

#endif