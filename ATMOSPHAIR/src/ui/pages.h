#ifndef _PAGES_H_
#define _PAGES_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../utils/utils.h"
#include "widgets.h"


//* _ DEFINITIONS ______________________________________________________________

#define LEFT_WIDGET_X_POS   34
#define LEFT_WIDGET_Y_POS   1
#define RIGHT_WIDGET_X_POS  138
#define RIGHT_WIDGET_Y_POS  1
        
//* _ ENUMERATIONS _____________________________________________________________

typedef enum page_index
{
    PAGE_1, 
    PAGE_2, 
    PAGE_3, 
    PAGE_4, 
    PAGE_5, 
    PAGE_6, 
    PAGE_7, 
    PAGE_8, 
    PAGE_9, 
    PAGE_10, 
}   PAGE_INDEX_t;


//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef struct page
{
    const WIDGET_t* const left_widget; 
    const WIDGET_t* const right_widget; 
}   PAGE_t;


//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn void display_page(PAGE_INDEX_t page_index); 
/// @brief display a page composed of either one or two widget on the screen. 
/// @param page_index correspond to which page to display from the LUT page 
///                   table. 
void display_page(PAGE_INDEX_t page_index); 

#endif
