#include <sys/mman.h>
#include "makeInstructions.h"

void traverseTree(TreeNode *node, char *prog, int &p_offset)
{
    if (node == nullptr)
    {
        return;
    }

    traverseTree(node->left, prog, p_offset);
    traverseTree(node->right, prog, p_offset);

    if (node->op.empty())
    { // if there isnt an operator, then it must be a number
        int num = node->value;
        prog[p_offset++] = 0xb8; // MOV EAX, num

        for (int i = 0; i < 4; i++)
        {
            prog[p_offset++] = (num & 0xff);
            num >>= 8;
        }
    }
    else
    {                                             // If it's an operator
        traverseTree(node->left, prog, p_offset); // Load left operand into EAX
        prog[p_offset++] = 0x50;                  // PUSH EAX (save left operand)

        traverseTree(node->right, prog, p_offset); // Load right operand into EAX
        prog[p_offset++] = 0x59;                   // POP ECX (restore left operand into ECX)

        if (node->op == "+")
        {
            prog[p_offset++] = 0x01; // ADD EAX, ECX
            prog[p_offset++] = 0xc8;
        }
        else if (node->op == "-")
        {
            prog[p_offset++] = 0x29; // SUB EAX, ECX
            prog[p_offset++] = 0xc8;
        }
    }
}

void generateIntelInstructions(TreeNode *node)
{
    char *prog;
    int value;
    int p_offset = 0;

    prog = (char *)mmap(0, 50000,
                        PROT_EXEC | PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // declare memory where instructions can be written

    traverseTree(node, prog, p_offset); // load all the instructions into memory

    prog[p_offset++] = 0xc3; // RET

    value = ((int (*)(void))prog)();
    std::cout << "RESULT IS: " << value << std::endl;
    munmap(0, 50000);
}