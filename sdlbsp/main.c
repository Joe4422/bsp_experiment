// Includes
#include <SDL2/SDL.h>
#include <unistd.h>

#include "graphics.h"
#include "error.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

int main()
{
	if (Graphics_Init("Test Window", SCREEN_WIDTH, SCREEN_HEIGHT) != STATUS_OK)
	{
		printf("Error initialising graphics subsystem!\r\n");
		return 1;
	}

	sleep(3);

	if (Graphics_Deinit() != STATUS_OK)
	{
		printf("Error deinitialising graphics subsystem!\r\n");
		return 1;
	}

	return 0;
}