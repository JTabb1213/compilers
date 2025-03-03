#include <iostream>
using std::cout;
using std::endl;

#include <sys/mman.h>

int main()
{
    unsigned char *prog;
    int value;
    int p_offset = 0;

    int value2 = -7;

    prog = (unsigned char *)mmap(0, 50000,
                                 PROT_EXEC | PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    prog[p_offset++] = 0xb8; // MOV EAX, 05H
    prog[p_offset++] = 0x05;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x50; // PUSH EAX
    prog[p_offset++] = 0xb8; // MOV EAX, 03H
    prog[p_offset++] = 0x03;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x41; // MOV R10, EAX
    prog[p_offset++] = 0x89;
    prog[p_offset++] = 0xc2;
    prog[p_offset++] = 0x41; // POP R9
    prog[p_offset++] = 0x8f;
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
    prog[p_offset++] = 0xc3; // RET

    //  cout << "value2: " << value2 << endl;

    // This calls the code array, honest!
    value = ((int (*)(void))prog)();

    cout << value << endl;

    munmap(0, 50000);

    return 0;
}