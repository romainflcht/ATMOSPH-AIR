#include "ssd1362.h"


static PIXEL_INTENSITY_t framebuffer[BUFFER_SIZE]; 


//* _ HARDWARE FUNCTIONS _______________________________________________________
void ssd1362_init(void)
{
    uint8_t command;
    
    // Wait for the SPI peripheral to be available. 
    while (SERCOM2_SPI_IsBusy()); 
    
    // Set the active low chip-select of the oled display and activate command 
    // mode. 
    DISPLAY_CS_Clear(); 
    DISPLAY_DATA_Clear(); 
    
    // Send all initialization commands defined in \ref ssd1362.h. 
    #define X(reg)  command = reg;                  \
                    SERCOM2_SPI_Write(&command, 1); \
                    while(SERCOM2_SPI_IsBusy());

        SSD1362_INIT_CONFIG
    #undef X

    // Unselect the oled display. 
    DISPLAY_CS_Set(); 
    DISPLAY_DATA_Set(); 
    
    // Set the callback to reset the chip select and data command pin. 
    SERCOM2_SPI_CallbackRegister(ssd1362_callback, (uintptr_t)NULL); 
    
    return; 
}


void ssd1362_callback(uintptr_t context)
{
    // Reset chip select and data select when the transimit is finished. 
    DISPLAY_CS_Set(); 
    DISPLAY_DATA_Set(); 
    return; 
}


bool ssd1362_refresh(void)
{
    // If the SPI peripheral isn't busy, send the framebuffer to the screen. 
    if (SERCOM2_SPI_IsBusy())
        return false; 
    
    DISPLAY_CS_Clear(); 
    return SERCOM2_SPI_Write((uint8_t*)framebuffer, BUFFER_SIZE); 
}

//* _ SOFTWARE FUNCTIONS _______________________________________________________

void display_fill(uint8_t intensity)
{
    int i; 
    
    // Fill the framebuffer of the given intensity. 
    for (i = 0; i < BUFFER_SIZE; i += 1)
        framebuffer[i].intensity = (intensity << BIT_PER_PIXEL) | intensity; 
    
    return; 
}


void display_set_pixel(uint32_t x, uint32_t y, uint8_t intensity)
{
    // Check if the given coordinates are in the screen boundaries. 
    if (COORD_ISINVALID(x, y))
        return; 
    
    // Set the correct half of the byte depending on the x position (each byte 
    // correspond to two pixels).
    if (x % 2 == 0)
        framebuffer[x / (8 / BIT_PER_PIXEL) + y * DISPLAY_LOGICAL_WIDTH].first_pixel = intensity; 
    else
        framebuffer[x / (8 / BIT_PER_PIXEL) + y * DISPLAY_LOGICAL_WIDTH].second_pixel = intensity; 
    
    return; 
}

void display_fast_h_line(uint32_t x, uint32_t y, uint32_t w, uint8_t intensity)
{
    uint32_t i; 

    // Trace point on an horizontal line, skipping Bresenham's line algorithm. 
    for (i = 0; i < w; i++)
        display_set_pixel(x + i, y, intensity); 

    return; 
}


void display_fast_v_line(uint32_t x, uint32_t y, uint32_t h, uint8_t intensity)
{
    uint32_t i; 

    // Trace point on a vertical line, skipping Bresenham's line algorithm.  
    for (i = 0; i < h; i++)
        display_set_pixel(x, y + i, intensity); 

    return; 
}



void display_draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t intensity)
{
    int i; 
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
    int iterations; 
    
    
    // Bresenham's line algorithm, see link below to understand how it's work. 
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm. 
    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = dx - dy;
    iterations = (dx > dy ? dx : dy) + 1;

    for (i = 0; i < iterations; i++) 
    {
        display_set_pixel(x0, y0, intensity);

        e2 = 2 * err;
        if (e2 > -dy) 
        {
            err -= dy;
            x0 += sx;
        }
        
        if (e2 < dx) 
        {
            err += dx;
            y0 += sy;
        }
    }
    
    return; 
}


void display_draw_circle(uint32_t cx, uint32_t cy, uint32_t r, uint8_t intensity)
{
    int32_t x; 
    int32_t y;
    int32_t p; 

    // explaination of how this algorithm works are available here: 
    // https://www.youtube.com/watch?v=hpiILbMkF9w from NoBS Code. 
    x = 0; 
    y = -r; 
    p = -r; 

    while (x < -y)
    {
        if (p > 0)
        {
            y += 1; 
            p += 2 * (x + y) + 1; 
        }

        else 
            p += 2 * x + 1;  

        display_set_pixel(cx + x, cy + y, intensity); 
        display_set_pixel(cx - x, cy + y, intensity); 
        display_set_pixel(cx + x, cy - y, intensity); 
        display_set_pixel(cx - x, cy - y, intensity); 
        display_set_pixel(cx + y, cy + x, intensity); 
        display_set_pixel(cx + y, cy - x, intensity); 
        display_set_pixel(cx - y, cy + x, intensity); 
        display_set_pixel(cx - y, cy - x, intensity); 
        x += 1; 
    }
    
    return; 
}


void display_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t intensity)
{
    // Draws four lines for the rectangle outline. 
    display_fast_h_line(x, y, w, intensity); 
    display_fast_v_line(x, y, h, intensity); 
    display_fast_h_line(x, y + h, w, intensity); 
    display_fast_v_line(x + w, y, h + 1, intensity); 
    return; 
}


void display_draw_fillrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t intensity)
{
    uint32_t i; 

    // Draw horizontal lines for the height of the rectangle. 
    for (i = 0; i < h; i++)
        display_fast_h_line(x, y + i, w, intensity); 

    return; 
}

void display_draw_char(uint32_t x, uint32_t y, char c, uint8_t fg_intensity)
{
    uint8_t*   char_addr; 
    uint8_t     current_byte; 
    
    // Get the address of the character we want to print from the font array. 
    char_addr = FONT_SMALL + (4 + (c - FONT_FIRST_CHAR_OFFSET) * FONT_CHAR_WIDTH); 
    
    for (int i = 0; i < FONT_CHAR_WIDTH; i += 1)
    {
        // Get the raw byte to operate on it. 
        current_byte = *char_addr;
        
        // Loop through each bit and set the pixel as foreground intensity if it
        // is high, background intensity if it is low. 
        for (int j = 0; j < FONT_CHAR_HEIGHT; j += 1)
        {
            if (current_byte & 0x01)
                display_set_pixel(x + i, y + j, fg_intensity); 
            
            // Offset the byte by one to see the next bit. 
            current_byte = current_byte >> 1; 
        }
        
        // Increment the character address to get the next byte that form the 
        // character. 
        char_addr += 1; 
    }
    
    return; 
}


void display_draw_str(uint32_t x, uint32_t y, char* str, uint8_t fg_intensity)
{
    uint32_t index; 
    uint32_t x_cur; 
    uint32_t y_cur; 
    
    // Execute the "display_draw_char" function and increment the x position of 
    // each char. 
    index = 0; 
    x_cur = 0; 
    y_cur = 0; 
    while(str[index])
    {
        // If a NL or CR character is detected, reset the x cursor position and 
        // increment the y cursor position. 
        if (str[index] == '\n' || str[index] == '\r')
        {
            y_cur += FONT_CHAR_HEIGHT; 
            x_cur = 0; 
        }
        
        else
        {
            display_draw_char(
                    x + (x_cur * FONT_CHAR_WIDTH), 
                    y + y_cur, 
                    str[index], 
                    fg_intensity
            ); 

            x_cur += 1; 
        }
        
        index += 1; 
    }
}


void display_img(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t* img)
{
    int x_index; 
    int y_index; 
    
    // Loop through each byte of the image and place it to the framebuffer to 
    // the right offset calculated using images indexes. 
    for (y_index = 0; y_index < h; y_index += 1)
        for (x_index = 0; x_index < w / 2; x_index += 1)
            // TODO: Protect write to pixel out of display bound. 
            framebuffer[(x + x_index) + (y + y_index) * DISPLAY_LOGICAL_WIDTH].intensity = img[x_index + y_index * (w / 2)]; 
    
    return; 
}