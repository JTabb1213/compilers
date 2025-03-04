#ifndef MAKE_INSTRUCTIONS_H
#define MAKE_INSTRUCTIONS_H
#include "tree.h"

void generateIntelInstructions(TreeNode *node);
void traverseTree(TreeNode *node, char *prog, int &p_offset);

#endif