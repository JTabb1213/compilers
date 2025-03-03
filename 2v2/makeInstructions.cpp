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
    {                             // if there isnt an operator, then it must be a number
        int value2 = node->value; // get value at node and make the instruction to MOV it into register EAX
        prog[p_offset++] = 0xb8;  // MOV EAX, imm32

        prog[p_offset++] = (value2 & 0xff);
        value2 >>= 8;
        prog[p_offset++] = (value2 & 0xff);
        value2 >>= 8;
        prog[p_offset++] = (value2 & 0xff);
        value2 >>= 8;
        prog[p_offset++] = (value2 & 0xff);
    }
    else
    {                                              // If it's an operator
        traverseTree(node->right, prog, p_offset); // proccess the right tree, which will load its value into EAX
        prog[p_offset++] = 0x50;                   // PUSH EAX, or save its value onto the stack

        traverseTree(node->left, prog, p_offset); // Load right operand into EAX
        prog[p_offset++] = 0x59;                  // POP ECX, or get the value at the stack pointer (the right node/tree) into ECX

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

        else if (node->op == "*")
        {
            prog[p_offset++] = 0x0f; // IMULT EAX ECX
            prog[p_offset++] = 0xaf;
            prog[p_offset++] = 0xc1;
        }

        else if (node->op == "/")    // according to instruction manual, DIV uses registers EAX and EDX.
        {                            // EDX/AEX, so left should be in
            prog[p_offset++] = 0x31; // XOR EDX register, to hold the remainer
            prog[p_offset++] = 0xd2;

            prog[p_offset++] = 0xf7; // DIV EDX:EAX, result will be in EAX with quotient in EDX
            prog[p_offset++] = 0xf9;
        }

        else if (node->op == "mod")
        {
            prog[p_offset++] = 0x31; // XOR EDX register, to hold the remainer
            prog[p_offset++] = 0xd2;

            prog[p_offset++] = 0xf7; // DIV EDX:EAX, result will be in EAX with quotient in EDX
            prog[p_offset++] = 0xf9;

            // we want the remainder in EAX, but its in EDX, so XCHG them
            prog[p_offset++] = 0x87;
            prog[p_offset++] = 0xd0;
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

    value = ((int (*)(void))prog)(); // execute what is in memory

    std::cout << "Code size: " << p_offset << std::endl;
    std::cout << "Code execution: " << std::endl;
    std::cout << value << std::endl;
    munmap(0, 50000);
}