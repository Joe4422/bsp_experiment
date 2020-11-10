#ifndef BSP_H_
#define BSP_H_

// Includes
#include "util.h"

// Typedefs
typedef struct node
{
    struct node* left;
    struct node* right;
    line_t line;
} node_t;

// Function declarations
node_t* BSP_BuildTree(line_t lineArray[], int lineCount);

void BSP_PrintTree(node_t* node, int parent);

#endif /* BSP_H_ */