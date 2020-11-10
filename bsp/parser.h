#ifndef PARSER_H_
#define PARSER_H_

// Includes
#include "util.h"

#include <stdio.h>

// Typedefs
typedef struct line_struct
{
	unsigned int lineCount;
	line_t* lines;
} line_struct_t;

// Function declarations
line_struct_t Parser_ParseFile(FILE* file);

#endif /* PARSER_H_ */