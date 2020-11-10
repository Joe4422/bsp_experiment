#include "util.h"
#include "bsp.h"
#include "parser.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	line_struct_t lineStruct;
	FILE* fp;

	if (argc < 2)
	{
		printf("Too few arguments! Expecting a file name.\r\n");
		return 0;
	}

	fp = fopen(argv[1], "r+");

	if (fp == NULL)
	{
		printf("Could not open file %s.\r\n", argv[1]);
		return 0;
	}

	lineStruct = Parser_ParseFile(fp);

    node_t* bspTree = BSP_BuildTree(lineStruct.lines, lineStruct.lineCount);

	if (bspTree == NULL)
	{
		printf("Out of memory! Exiting...\r\n");
		return 1;
	}

    BSP_PrintTree(bspTree, 0);

	return 0;
}