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
    SERCOM2_SPI_CallbackRegister(SPI_callback, (uintptr_t)NULL); 
    
    return; 
}


void SPI_callback(uintptr_t context)
{
    // Reset chip select and data select when the transimit is finished. 
    DISPLAY_CS_Set(); 
    DISPLAY_DATA_Set(); 
    return; 
}


bool SSD1362_refresh(void)
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

void display_draw_char(uint32_t x, uint32_t y, char c, uint8_t fg_intensity, const uint8_t* font_data)
{
    const uint8_t*  char_addr; 
    uint8_t         font_height; 
    uint8_t         font_width;
    uint8_t         font_ascii_offset; 
    uint8_t         bytes_per_col; 
    uint16_t        current_pixel; 
    
    if (!font_data)
        return; 
    
    // Get font information from font header. 
    font_width        = font_data[HEADER_FONT_WIDTH]; 
    font_height       = font_data[HEADER_FONT_HEIGHT]; 
    font_ascii_offset = font_data[HEADER_FONT_OFFSET]; 
    
    bytes_per_col = (font_height > sizeof(uint8_t) * 8) ? 2 : 1; 
    
    // Get the address of the character we want to print from the font array. 
    char_addr = font_data + (FONT_HEADER_SIZE + (c - font_ascii_offset) * font_width * bytes_per_col); 
    
    for (int i = 0; i < font_width; i += 1)
    {
        // Get the raw byte to operate on it. 
        if (bytes_per_col == 1)
            current_pixel = *char_addr;
        else
            current_pixel = (char_addr[1] << 8) | char_addr[0]; 
        
        // Loop through each bit and set the pixel as foreground intensity if it
        // is high, background intensity if it is low. 
        for (int j = 0; j < font_height; j += 1)
        {
            if (current_pixel & 0x01)
                display_set_pixel(x + i, y + j, fg_intensity); 
            
            // Offset the byte by one to see the next bit. 
            current_pixel = current_pixel >> 1; 
        }
        
        // Increment the character address to get the next byte that form the 
        // character. 
        char_addr += bytes_per_col; 
    }
    
    return; 
}


void display_draw_str(uint32_t x, uint32_t y, const char* str, uint8_t fg_intensity, FONTS_t font)
{
    const uint8_t*  font_data; 
    uint8_t         font_height; 
    uint8_t         font_width;
    uint8_t         font_spacing;
    uint32_t        index; 
    uint32_t        x_cur; 
    uint32_t        y_cur; 
    
    
    switch (font)
    {
        case FONT_6X8:
            font_data = FONT_6x8; 
            break; 
            
        case FONT_10X16: 
            font_data = FONT_10x16; 
            break; 
            
        case FONT_10X16_BOLD: 
            font_data = FONT_10x16_BOLD;
            break; 
        
        default: 
            return; 
    }
    
    font_spacing = font_data[HEADER_CHAR_SPACING]; 
    font_width   = font_data[HEADER_FONT_WIDTH]; 
    font_height  = font_data[HEADER_FONT_HEIGHT]; 
    
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
            y_cur += font_height; 
            x_cur = 0; 
        }
        
        else
        {
            display_draw_char(
                    x + (x_cur * font_width) + font_spacing, 
                    y + y_cur, 
                    str[index], 
                    fg_intensity, 
                    font_data
            ); 

            x_cur += 1; 
        }
        
        index += 1; 
    }
}

uint32_t display_printf(uint32_t x, uint32_t y, uint8_t fg_intensity, FONTS_t font, const char* format, ...)
{
    char    str[PRINTF_BUFFER_SIZE]; 
    size_t  char_count;
    va_list args;
    
    va_start(args, format);

    // Allocate and format the string and print it. 
    char_count = vsnprintf(str, PRINTF_BUFFER_SIZE, format, args); 
    
    // Free allocated ressources.
    va_end(args);
    
    if (char_count > 0)
        display_draw_str(x, y, str, fg_intensity, font); 

    return (uint32_t)char_count; 
}


void display_img(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint8_t* img)
{
    int x_index; 
    int y_index; 
    uint8_t curr_pixel; 

    for (y_index = 0; y_index < h; y_index += 1)
        for (x_index = 0; x_index < (w + 1) / 2; x_index += 1)
        {
            if (COORD_ISINVALID((x + (x_index * 2)), (y + y_index)))
                continue; 
            
            curr_pixel = img[x_index + y_index * ((w + 1) / 2)]; 

            framebuffer[(x / 2 + x_index) + (y + y_index) * DISPLAY_LOGICAL_WIDTH].intensity = curr_pixel; 
        }
    
    return;
}
