#include <iostream>
using std::cout;
using std::endl;

#include <sys/mman.h>

int main()
{
    char *prog;
    int value;
    int p_offset = 0;

    int value2 = -7;

    prog = (char *)mmap(0, 50000,
                        PROT_EXEC | PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    prog[p_offset++] = 0xb8; // MOV EAX, 2AH
    prog[p_offset++] = 0x2a;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0x00;
    prog[p_offset++] = 0xb9; // MOV ECX, -7
    prog[p_offset++] = (value2 & 0xff);
    value2 >>= 8;
    prog[p_offset++] = (value2 & 0xff);
    value2 >>= 8;
    prog[p_offset++] = (value2 & 0xff);
    value2 >>= 8;
    prog[p_offset++] = (value2 & 0xff);
    prog[p_offset++] = 0x01; // ADD EAX, ECX
    prog[p_offset++] = 0xc8;
    prog[p_offset++] = 0xc3; // RET

    //  cout << "value2: " << value2 << endl;

    // This calls the code array, honest!
    value = ((int (*)(void))prog)();

    cout << value << endl;

    munmap(0, 50000);

    return 0;
}