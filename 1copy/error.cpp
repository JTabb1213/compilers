#include "error.h"
#include <iostream>

void print_error(Error &e)
{
    std::cout << "Error code was: " << e.error << " occuring at line, column: "
              << e.line << ", " << e.col << std::endl;
}