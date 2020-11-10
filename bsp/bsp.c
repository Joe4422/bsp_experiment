// Includes
#include "bsp.h"

#include <stdlib.h>
#include <stdio.h>

// Typedefs
typedef enum bsp_line_pos
{
	BSP_LINE_IN_FRONT,
	BSP_LINE_BEHIND,
	BSP_LINE_INTERSECT
} bsp_line_pos_t;

typedef enum bsp_point_pos
{
	BSP_POINT_IN_FRONT,
	BSP_POINT_BEHIND,
	BSP_POINT_ON_LINE
} bsp_point_pos_t;

// Function declarations
point_t BSP_GetLineIntersect(line_t lA, line_t lB);

bsp_point_pos_t BSP_GetPointPos(line_t line, point_t point);

bool BSP_SplitLine(line_t splitter, line_t splittee, line_t* first, line_t* second);

bool BSP_CheckInsideBoundingBox(point_t bbA, point_t bbB, point_t point);

bsp_line_pos_t BSP_GetRelativeLinePosition(line_t primary, line_t secondary);

// Function definitions
node_t* BSP_BuildTree(line_t lineArray[], int lineCount)
{
	int i = 0;

	// List of lines in front of the current node
	line_t* frontLines = malloc((lineCount - 1) * sizeof(line_t));
	int frontLinesIndex = 0;

	// List of lines behind the current node
	line_t* backLines = malloc((lineCount - 1) * sizeof(line_t));
	int backLinesIndex = 0;

	// Allocate and set up current node
	node_t* node = malloc(sizeof(node_t));
	node->left = NULL;
	node->right = NULL;
	node->line = lineArray[0];

	// Check if out of memory
	if (!frontLines || !backLines || !node)
	{
		return NULL;
	}

	// Check if further recursion required
	if (lineCount == 1)
	{
		return node;
	}	

	// Determine position of current node line with respect to each line in the array
	for (i = 1; i < lineCount; i++)
	{
		bsp_line_pos_t pos = BSP_GetRelativeLinePosition(node->line, lineArray[i]);
		
		if (pos == BSP_LINE_IN_FRONT)
		{
			frontLines[frontLinesIndex++] = lineArray[i];
		}
		else if (pos == BSP_LINE_BEHIND)
		{
			backLines[backLinesIndex++] = lineArray[i];
		}
		else if (pos == BSP_LINE_INTERSECT)
		{
			line_t front;
			line_t back;

			BSP_SplitLine(node->line, lineArray[i], &front, &back);
			frontLines[frontLinesIndex++] = front;
			backLines[backLinesIndex++] = back;
		}
	}

	// If there are lines behind, recursively process them
	if (backLinesIndex != 0)
	{
		node->left = BSP_BuildTree(backLines, backLinesIndex);
		if (node->left == NULL)
		{
			return NULL;
		}
	}
	// If there are lines in front, recursively process them
	if (frontLinesIndex != 0)
	{
		node->right = BSP_BuildTree(frontLines, frontLinesIndex);
		if (node->right == NULL)
		{
			return NULL;
		}
	}

	return node;
}

point_t BSP_GetLineIntersect(line_t lA, line_t lB)
{
	// Algorithm taken from https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/1968345#1968345
	int x12 = lA.start.x - lA.end.x;
	int x34 = lB.start.x - lB.end.x;
	int y12 = lA.start.y - lA.end.y;
	int y34 = lB.start.y - lB.end.y;

	int c = x12 * y34 - y12 * x34;

	int a = lA.start.x * lA.end.y - lA.start.y * lA.end.x;
	int b = lB.start.x * lB.end.y - lB.start.y * lB.end.x;

	int x = (a * x34 - b * x12) / c;
	int y = (a * y34 - b * y12) / c;

	return (point_t){ x, y };
}

bsp_point_pos_t BSP_GetPointPos(line_t line, point_t point)
{
	int x = point.x;
	int y = point.y;
	int x1 = line.start.x;
	int x2 = line.end.x;
	int y1 = line.start.y;
	int y2 = line.end.y;

	int out = ((x - x1) * (y2 - y1)) - ((y - y1) * (x2 - x1));

	if (out < 0)
	{
		return BSP_POINT_IN_FRONT;
	}
	else if (out > 0)
	{
		return BSP_POINT_BEHIND;
	}	
	else
	{
		return BSP_POINT_ON_LINE;
	}	
}

bool BSP_SplitLine(line_t splitter, line_t splittee, line_t* first, line_t* second)
{
	point_t intersect = BSP_GetLineIntersect(splitter, splittee);

	if (BSP_CheckInsideBoundingBox(splittee.start, splittee.end, intersect) == false)
	{
		return false;
	}

	first->start = splittee.start;
	first->end = intersect;
	first->front = splittee.front;
	second->start = intersect;
	second->end = splittee.end;
	second->front = splittee.front;

	return true;
}

bool BSP_CheckInsideBoundingBox(point_t bbA, point_t bbB, point_t point)
{
	int bigX = bbA.x > bbB.x ? bbA.x : bbB.x;
	int bigY = bbA.y > bbB.y ? bbA.y : bbB.y;
	int littleX = bbA.x < bbB.x ? bbA.x : bbB.x;
	int littleY = bbA.y < bbB.y ? bbA.y : bbB.y;

	return (point.x <= bigX) && (point.x >= littleX) && (point.y <= bigY) && (point.y >= littleY);
}

bsp_line_pos_t BSP_GetRelativeLinePosition(line_t primary, line_t secondary)
{
	bsp_point_pos_t a = BSP_GetPointPos(primary, secondary.start);
	bsp_point_pos_t b = BSP_GetPointPos(primary, secondary.end);
	bsp_line_pos_t out;

	if (primary.front == true)
	{
		if ((a == BSP_POINT_IN_FRONT && b == BSP_POINT_IN_FRONT) ||
			(a == BSP_POINT_IN_FRONT && b == BSP_POINT_ON_LINE) ||
			(a == BSP_POINT_ON_LINE && b == BSP_POINT_IN_FRONT))
		{
			out = BSP_LINE_IN_FRONT;
			printf("f In front!\r\n");
		}
		else if ((a == BSP_POINT_IN_FRONT && b == BSP_POINT_BEHIND) || 
				 (a == BSP_POINT_BEHIND && b == BSP_POINT_IN_FRONT))
		{
			out = BSP_LINE_INTERSECT;
			printf("f Intersect!\r\n");
		}
		else if ((a == BSP_POINT_BEHIND && b == BSP_POINT_BEHIND) ||
				 (a == BSP_POINT_BEHIND && b == BSP_POINT_ON_LINE) ||
				 (a == BSP_POINT_ON_LINE && b == BSP_POINT_BEHIND))
		{
			out = BSP_LINE_BEHIND;
			printf("f Behind!\r\n");
		}
	}
	else if (primary.front == false)
	{
		if ((a == BSP_POINT_IN_FRONT && b == BSP_POINT_IN_FRONT) ||
			(a == BSP_POINT_IN_FRONT && b == BSP_POINT_ON_LINE) ||
			(a == BSP_POINT_ON_LINE && b == BSP_POINT_IN_FRONT))
		{
			out = BSP_LINE_BEHIND;
			printf("b Behind!\r\n");
		}
		else if ((a == BSP_POINT_IN_FRONT && b == BSP_POINT_BEHIND) || 
				 (a == BSP_POINT_BEHIND && b == BSP_POINT_IN_FRONT))
		{
			out = BSP_LINE_INTERSECT;
			printf("b Intersect!\r\n");
		}
		else if ((a == BSP_POINT_BEHIND && b == BSP_POINT_BEHIND) ||
				 (a == BSP_POINT_BEHIND && b == BSP_POINT_ON_LINE) ||
				 (a == BSP_POINT_ON_LINE && b == BSP_POINT_BEHIND))
		{
			out = BSP_LINE_IN_FRONT;
			printf("b In front!\r\n");
		}
	}

	return out;
}

void BSP_PrintTree(node_t* node, int parent)
{
    static int index = 0;
    int me = index;

    index++;

    if (node->left == NULL && node->right == NULL)
    {
        printf("Node %d (parent %d) goes (X=%d, Y=%d) to (X=%d, Y=%d), no left, no right.\r\n",
                me,
                parent,
                node->line.start.x,
                node->line.start.y,
                node->line.end.x,
                node->line.end.y);
    }
    else if (node->left != NULL && node->right == NULL)
    {
        printf("Node %d (parent %d) goes (X=%d, Y=%d) to (X=%d, Y=%d), has left, no right.\r\n",
                me,
                parent,
                node->line.start.x,
                node->line.start.y,
                node->line.end.x,
                node->line.end.y);
        BSP_PrintTree(node->left, me);
    }
    else if (node->left == NULL && node->right != NULL)
    {
        printf("Node %d (parent %d) goes (X=%d, Y=%d) to (X=%d, Y=%d), no left, has right.\r\n",
                me,
                parent,
                node->line.start.x,
                node->line.start.y,
                node->line.end.x,
                node->line.end.y);
        BSP_PrintTree(node->right, me);
    }
    else if (node->left != NULL && node->right != NULL)
    {
        printf("Node %d (parent %d) goes (X=%d, Y=%d) to (X=%d, Y=%d), has left, has right.\r\n",
                me,
                parent,
                node->line.start.x,
                node->line.start.y,
                node->line.end.x,
                node->line.end.y);
        BSP_PrintTree(node->left, me);
        BSP_PrintTree(node->right, me);
    }
}