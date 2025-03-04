#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <iostream>
#include "token.h"

extern size_t token_vector_index;

struct TreeNode
{
    std::string op;
    int value;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int val) : op(""), value(val), left(nullptr), right(nullptr) {}

    TreeNode(std::string opr, TreeNode *l, TreeNode *r)
        : op(opr), value(0), left(l), right(r) {}
};

bool isNumber(std::string);
TreeNode *E(std::vector<Token> t);
TreeNode *T(std::vector<Token> t);
TreeNode *F(std::vector<Token> t);
TreeNode *Ex(std::vector<Token> t);
void deleteTree(TreeNode *node);
void printTree(TreeNode *root, std::string indent = "");

#endif