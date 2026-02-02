#ifndef _DIPLAY_H_
#define _DIPLAY_H_

//* _ INCLUDES _________________________________________________________________

#include <stdlib.h>
#include "definitions.h"
#include "../utils/fonts.h"


//* _ DEFINITIONS ______________________________________________________________

#define DISPLAY_WIDTH           256
#define DISPLAY_HEIGHT          64
#define BIT_PER_PIXEL           4

#define DISPLAY_LOGICAL_WIDTH   256 / (8 / BIT_PER_PIXEL)
#define DISPLAY_LOGICAL_HEIGHT  64
#define BUFFER_SIZE             DISPLAY_LOGICAL_WIDTH * DISPLAY_LOGICAL_HEIGHT

#define MAX_INTENSITY           0x0F
#define HALF_INTENSITY          MAX_INTENSITY / 2
#define MIN_INTENSITY           0x00


#define SSD1362_INIT_CONFIG X(0xFD) \
                            X(0x12) \
                            X(0xAE) \
                            X(0x15) \
                            X(0x00) \
                            X(0x7F) \
                            X(0x75) \
                            X(0x00) \
                            X(0x3F) \
                            X(0x81) \
                            X(0x2F) \
                            X(0xA0) \
                            X(0xC3) \
                            X(0xA1) \
                            X(0x00) \
                            X(0xA2) \
                            X(0x00) \
                            X(0xA4) \
                            X(0xA8) \
                            X(0x3F) \
                            X(0xAB) \
                            X(0x01) \
                            X(0xAD) \
                            X(0x8E) \
                            X(0xB1) \
                            X(0x22) \
                            X(0xB3) \
                            X(0xA0) \
                            X(0xB6) \
                            X(0x04) \
                            X(0xB9) \
                            X(0xBC) \
                            X(0x10) \
                            X(0xBD) \
                            X(0x01) \
                            X(0xBE) \
                            X(0x07) \
                            X(0xAF)


//* _ MACRO DEFINITIONS ________________________________________________________
                            
#define COORD_ISINVALID(x, y) (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT)

//* _ ENUMERATIONS _____________________________________________________________

typedef enum ssd1362_reg
{
    ON           = 0xAE,
    OFF          = 0xAF,
    SET_COL_ADDR = 0x15, 
    SET_ROW_ADDR = 0x75,
    SET_CONTRAST = 0x81, 
    INVERT       = 0xA7,
}   SSD1362_REG_t;


//* _ STRUCTURES _______________________________________________________________

typedef union pixel_intensity
{
    uint8_t intensity; 
    struct
    {
        uint8_t second_pixel: 4; 
        uint8_t first_pixel : 4; 
    };
}   PIXEL_INTENSITY_t;


//* _ HARDWARE FUNCTION DECLARATIONS ___________________________________________

/// @fn void display_init(); 
/// @brief send all initialization command to the OLED controller and register 
///        the SPI callback. 
void ssd1362_init(void); 


/// @fn void ssd1362_callback(uintptr_t context); 
/// @brief set high to chip select and data/command pin (unselect the SPI 
///        device). 
/// @param context not used. 
void SPI_callback(uintptr_t context); 


/// @fn bool ssd1362_refresh(void);  
/// @brief send the framebuffer data to the screen. 
/// @return false in case the SPI peripheral is busy, true is the request has 
///         been accepted by the peripheral. 
bool ssd1362_refresh(void); 


//* _ SOFTWARE GRAPHICAL FUNCTION DECLARATIONS _________________________________

/// @fn void display_fill(uint8_t intensity);
/// @brief fills the framebuffer with the passed color.  
/// @param intensity value between 0 and 15 to set the grayscale of every pixel. 
void display_fill(uint8_t intensity); 


/// @fn void display_set_pixel(uint32_t x, uint32_t y, uint8_t intensity); 
/// @brief draws a pixel at x, y coordinate. 
/// @param x         x coordinate. 
/// @param y         y coordinate. 
/// @param intensity value between 0 and 15 to set the grayscale of the pixel. 
void display_set_pixel(uint32_t x, uint32_t y, uint8_t intensity); 


/// @fn void display_fast_h_line(uint32_t x, uint32_t y, uint32_t w, uint8_t intensity); 
/// @brief draws an horizontal line skipping Bresenham's line algorithm. 
/// @param x0        first x coordinate. 
/// @param y0        first y coordinate. 
/// @param w         length of the line. 
/// @param intensity value between 0 and 15 to set the grayscale of the line. 
void display_fast_h_line(uint32_t x, uint32_t y, uint32_t w, uint8_t intensity); 


/// @fn void display_fast_v_line(uint32_t x, uint32_t y, uint32_t w, uint8_t intensity); 
/// @brief draws a vertical line skipping Bresenham's line algorithm. 
/// @param x0        first x coordinate. 
/// @param y0        first y coordinate. 
/// @param h         length of the line. 
/// @param intensity value between 0 and 15 to set the grayscale of the line. 
void display_fast_v_line(uint32_t x, uint32_t y, uint32_t h, uint8_t intensity); 


/// @fn void display_draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t intensity); 
/// @brief draws a line between two x, y coordinates. 
/// @param x0        first x coordinate. 
/// @param y0        first y coordinate. 
/// @param x1        second x coordinate. 
/// @param y1        second y coordinate. 
/// @param intensity value between 0 and 15 to set the grayscale of the line. 
void display_draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t intensity); 


/// @fn void display_draw_circle(uint32_t cx, uint32_t cy, uint32_t r, uint8_t intensity); 
/// @brief use the midpoint algorithm to draw a circle. 
/// @param cx        x coordinate of the center of the circle. 
/// @param cy        y coordinate of the center of the circle. 
/// @param r         radius of the circle. 
/// @param intensity value between 0 and 15 to set the grayscale of the circle. 
void display_draw_circle(uint32_t cx, uint32_t cy, uint32_t r, uint8_t intensity); 


/// @fn void display_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t intensity);
/// @brief draw a rectangle outline. 
/// @param x         x coordinate. 
/// @param y         y coordinate. 
/// @param w         width of the rectangle. 
/// @param h         height of the rectangle. 
/// @param intensity value between 0 and 15 to set the grayscale of the rectangle. 
void display_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t intensity); 


/// @fn void display_draw_fillrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t intensity);
/// @brief draw a filled rectangle. 
/// @param x         x coordinate. 
/// @param y         y coordinate. 
/// @param w         width of the rectangle. 
/// @param h         height of the rectangle. 
/// @param intensity value between 0 and 15 to set the grayscale of the rectangle. 
void display_draw_fillrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t intensity);


/// @fn display_draw_char(uint32_t x, uint32_t y, char c, uint8_t fg_intensity); 
/// @brief draws a character contained in the ACSII table. 
/// @param x            x coordinate. 
/// @param y            y coordinate. 
/// @param c            character to draw. 
/// @param fg_intensity foreground intensity. 
void display_draw_char(uint32_t x, uint32_t y, char c, uint8_t fg_intensity); 


/// @fn void display_draw_str(uint32_t x, uint32_t y, char* str, uint8_t fg_intensity);
/// @brief draws a string on the screen. 
/// @param x            x coordinate. 
/// @param y            y coordinate. 
/// @param str          string to draw. 
/// @param fg_intensity foreground intensity. 
void display_draw_str(uint32_t x, uint32_t y, char* str, uint8_t fg_intensity); 


/// @fn void display_img(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t* img);  
/// @brief displays an image contained in an uint8_t array. 
/// @param x   x coordinate. 
/// @param y   y coordinate. 
/// @param w   width of the image. 
/// @param h   height of the image. 
/// @param img uint8_t array that contains pixel data of the image. 
void display_img(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint8_t* img); 

#endif