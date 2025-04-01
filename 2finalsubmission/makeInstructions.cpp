#include <sys/mman.h>
#include "makeInstructions.h"

void traverseTree(TreeNode *node, char *prog, int &p_offset)
{
    if (node == nullptr)
    {
        return;
    }

    // traverseTree(node->left, prog, p_offset);
    // traverseTree(node->right, prog, p_offset);

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
        prog[p_offset++] = 0x8f;                  // POP EBX, or get the value at the stack pointer (the right node/tree) into EBX
        prog[p_offset++] = 0xc1;

        // if (node->left == nullptr) // if there is no left child, load 0 into eax
        // {
        //    std::cout << "HHH" << std::endl;
        // }

        if (node->op == "+")
        {
            if (node->right == nullptr)
            {
                prog[p_offset++] = 0x31; // XOR ECX, makeing it 0
                prog[p_offset++] = 0xc9;
            }
            prog[p_offset++] = 0x01; // ADD EAX, ECX
            prog[p_offset++] = 0xc8;
        }
        else if (node->op == "-")
        {
            if (node->right == nullptr)
            {
                // std::cout << "NULL" << std::endl;
                prog[p_offset++] = 0x31; // XOR ECX, ECX
                prog[p_offset++] = 0xc9;

                prog[p_offset++] = 0x87; // XCG. Was saying positive for some reason, so i just changed it
                prog[p_offset++] = 0xc8;
            }
            prog[p_offset++] = 0x29; // SUB EAX, EBX
            prog[p_offset++] = 0xc8;
        }

        else if (node->op == "*")
        {
            prog[p_offset++] = 0x0f; // IMULT EAX EBX
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

        else if (node->op == "^")
        {
            prog[p_offset++] = 0x41; // MOV R10, EBX
            prog[p_offset++] = 0x89;
            prog[p_offset++] = 0xca;
            prog[p_offset++] = 0x41; // MOV R9, EAX
            prog[p_offset++] = 0x89;
            prog[p_offset++] = 0xc1;
            prog[p_offset++] = 0x45; // XOR R8, R8
            prog[p_offset++] = 0x31;
            prog[p_offset++] = 0xc0;
            prog[p_offset++] = 0x45; // TEST R10, R10
            prog[p_offset++] = 0x85;
            prog[p_offset++] = 0xd2;
            prog[p_offset++] = 0x7c; // JL 1EH
            prog[p_offset++] = 0x1e;
            prog[p_offset++] = 0x41; // INC R8
            prog[p_offset++] = 0xff;
            prog[p_offset++] = 0xc0;
            prog[p_offset++] = 0x45; // TEST R10, R10
            prog[p_offset++] = 0x85;
            prog[p_offset++] = 0xd2;
            prog[p_offset++] = 0x74; // JE 16H
            prog[p_offset++] = 0x16;
            prog[p_offset++] = 0x41; // TEST R10, 00000001H
            prog[p_offset++] = 0xf7;
            prog[p_offset++] = 0xc2;
            prog[p_offset++] = 0x01;
            prog[p_offset++] = 0x00;
            prog[p_offset++] = 0x00;
            prog[p_offset++] = 0x00;
            prog[p_offset++] = 0x74; // JE 04H
            prog[p_offset++] = 0x04;
            prog[p_offset++] = 0x45; // IMUL R8, R9
            prog[p_offset++] = 0x0f;
            prog[p_offset++] = 0xaf;
            prog[p_offset++] = 0xc1;
            prog[p_offset++] = 0x45; // IMUL R9, R9
            prog[p_offset++] = 0x0f;
            prog[p_offset++] = 0xaf;
            prog[p_offset++] = 0xc9;
            prog[p_offset++] = 0x41; // SAR R10, 1
            prog[p_offset++] = 0xd1;
            prog[p_offset++] = 0xfa;
            prog[p_offset++] = 0xeb; // JMP  -27H
            prog[p_offset++] = 0xe5;
            prog[p_offset++] = 0x44; // MOV EAX, R8
            prog[p_offset++] = 0x89;
            prog[p_offset++] = 0xc0;
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