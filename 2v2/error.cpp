#include "error.h"
#include <iostream>
#include <unordered_map>

std::unordered_map<int, std::string> errorNames = {
    {NCC_OK, "NCC_OK"},
    {NCC_FILE_NOT_FOUND, "NCC_EOF"},
    {NCC_OUTOFBOUNDS, " NCC_OUTOFBOUNDS"},
    {NCC_EOF, "NCC_EOF"},
    {NCC_UNEXPECTEDENDOFFILE, "NCC_UNEXPECTEDENDOFFILE"}};

void print_error(Error &e)
{
    std::cout << errorNames[e.error] << std::endl;
}