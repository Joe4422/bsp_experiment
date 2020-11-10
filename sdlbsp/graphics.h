#ifndef GRAPHICS_H_
#define GRAPHICS_H_

// Includes
#include <SDL2/SDL.h>
#include "error.h"

// Function declarations
STATUS Graphics_Init
(
    char *          title,
    unsigned int    width,
    unsigned int    height
);

STATUS Graphics_Deinit(void);

#endif