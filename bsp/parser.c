// Includes
#include "parser.h"

#include <stdlib.h>
#include <string.h>

// Defines
#define INPUT_BUFFER_SIZE	128

// Function definitions
line_struct_t Parser_ParseFile(FILE* file)
{
	int lines = 0;
	line_struct_t out;
	char c;
	int i;

	for (c = getc(file); c != EOF; c = getc(file))
	{
		if (c == '\n')
		{
			lines++;
		}
	}

	out.lines = malloc(lines * sizeof(line_t));
	out.lineCount = lines;

	fseek(file, 0, SEEK_SET);
	for (i = 0; i < lines; i++)
	{
		fscanf(file, "%d %d %d %d\n", &(out.lines[i].start.x), &(out.lines[i].start.y), &(out.lines[i].end.x), &(out.lines[i].end.y));
	}

	return out;
}
