#include "token.h"
#include <iostream>

void print_token(const Token &t)
{
    std::cout << "Token id: " << t.id
              << " | line: " << t.line
              << " | column: " << t.column;
    if (!t.value.empty())
    {
        std::cout << " | Value: \"" << t.value << "\"";
    }

    std::cout << std::endl;
}
