#include <iostream>
#include "lex.h"

int main()
{
    const char *source_code = "source1.cpp"; // Sample input

    Error err = lex_init(source_code);
    if (err.error != NCC_OK)
    {
        std::cerr << "Lexical analysis initialization failed!" << std::endl;
        return 1;
    }

    Token token;
    while (true)
    {
        err = get_token(token);
        if (err.error != NCC_OK)
        {
            print_error(err);
            std::cout << "Error occured while reading a: " << token.value << std::endl;
            return 1;
        }
        print_token(token);
        if (token.id == TOKEN_EOF)
            break;
    }

    return 0;
}
