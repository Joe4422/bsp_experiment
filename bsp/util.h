#ifndef UTIL_H_
#define UTIL_H_

// Includes
#include <stdbool.h>

// Typedefs
typedef struct point
{
    int x;
    int y;
} point_t;

typedef struct line
{
    point_t start;
    point_t end;
    bool front;
} line_t;

// Function declarations
void Line_SetStart(line_t* line, int x, int y);

void Line_SetEnd(line_t* line, int x, int y);

void Line_SetPoints(line_t* line, int sX, int sY, int eX, int eY);

#endif /* UTIL_H_ */