#ifndef _PAGES_H_
#define _PAGES_H_

//* _ INCLUDES _________________________________________________________________
#include <stdlib.h>
#include "definitions.h" 

#include "../utils/utils.h"
#include "widgets.h"


//* _ DEFINITIONS ______________________________________________________________


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
}   PAGE_INDEX_t;


//* _ STRUCTURE DEFINITIONS ____________________________________________________

typedef struct page
{
    WIDGET_ID_t    left_widget_id; 
    WIDGET_ID_t    right_widget_id; 
}   PAGE_t;


//* _ FUNCTION DECLARATIONS ____________________________________________________

/// @fn void display_page(PAGE_INDEX_t page_index); 
/// @brief display a page composed of either one or two widget on the screen. 
/// @param page_index correspond to which page to display from the LUT page 
///                   table. 
void display_page(PAGE_INDEX_t page_index); 

#endif
