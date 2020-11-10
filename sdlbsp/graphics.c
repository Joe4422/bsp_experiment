// Includes
#include "graphics.h"

// Global variables
static SDL_Window *		window = NULL;
static SDL_Surface *	surface = NULL;

static int screenWidth = 640;
static int screenHeight = 480;

STATUS Graphics_Init
(
    char *          title,
    unsigned int    width,
    unsigned int    height
)
{
    screenWidth = width;
    screenHeight = height;

    // Initialise video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return STATUS_GRAPHICS_FAILURE;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        return STATUS_GRAPHICS_FAILURE;
    }

    surface = SDL_GetWindowSurface(window);
    if (surface == NULL)
    {
        return STATUS_GRAPHICS_FAILURE;
    }

    return STATUS_OK;
}

STATUS Graphics_Deinit(void)
{
    SDL_DestroyWindow(window);

    SDL_Quit();

    return STATUS_OK;
}