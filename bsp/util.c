// Includes
#include "util.h"

// Function definitions
void Line_SetStart(line_t* line, int x, int y)
{
    line->start.x = x;
    line->start.y = y;
}

void Line_SetEnd(line_t* line, int x, int y)
{
    line->end.x = x;
    line->end.y = y;
}

void Line_SetPoints(line_t* line, int sX, int sY, int eX, int eY)
{
    Line_SetStart(line, sX, sY);
    Line_SetEnd(line, eX, eY);
}