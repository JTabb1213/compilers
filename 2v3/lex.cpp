#include "lex.h"
#include <iostream>
#include <cctype>
#include <sstream>

unsigned int utf_encoding(std::string utf)
{
    unsigned int utfInt;
    std::stringstream st;
    st << std::hex << utf;
    st >> utfInt;
    // std::cout << "Hex string converted into int for is: " << std::hex << utfInt << std::endl;

    if (utfInt <= 0x0000007f)
    { // if the utf int is in that range, do nothing but add an extra 0 and return the value
        // std::cout << "First row hex!" << std::endl;
        utfInt = utfInt & 0x7F; // get the last 7 digits, and make sure there is a 0 on the end.
    }

    else if (utfInt >= 0x00000080 && utfInt <= 0x000007FF)
    {                                                                // we need to get the lower 6 bits, and that with 0xBF. then get the upper 5 bits, and and it with 0xDF
        unsigned int mask1 = (utfInt & 0b111111) | 0b10000000;       // first get the lower 6 bits by ANDing the whole thing with 0b111111. Then add a 0b10 to the front of it by ORing it with 0b10000000
        unsigned int mask2 = ((utfInt >> 6) & 0b11111) | 0b11000000; // get the upper 5 bits by right shifting by 6, then extract the upper 5 bits by ANDing with 0b11111. Then add a 0b110 to the front by Oring with 0b11000000
        utfInt = (mask2 << 8) | mask1;                               // get the final number by shifting mask2 by 8, adn then ORing mask2 and mask1.
        /*
            std::cout << "Second row hex!" << std::endl;
            unsigned int mask1 = 0b000000;
            unsigned int mask2 = 0x00000;
            unsigned int finalInt = 0b1100000010000000;
            mask1 = utfInt | mask1;             // get the lower 6 bits by ORing the lower 8 bits with 000000
            mask1 = mask1 | 0b10000000;         // add a 10 to the front of it by doing that
            mask2 = (utfInt >> 6) | mask2;      // get the upper 5 bits by chopping off the lower 6 (really just right shifting them), and ORing with 0b00000
            mask2 = mask2 | 11000000;           // add a 110 to the front of it
            finalInt = finalInt | (mask2 << 8); // left shift mask2 by 8, or append 8 zeros to the end of it, and or it with the final mask
            finalInt = finalInt | mask1;        // or the lower mask, effectivly givving us the final utf8 encoded character
            utfInt = finalInt;                  // set it equal to the utfInt so we can convert it back to string.
            */
    }

    else if (utfInt >= 0x00000800 && utfInt <= 0x0000ffff)
    {
        // std::cout << "Third row hex!" << std::endl;
        unsigned int mask1 = (utfInt & 0b111111) | 0b10000000;
        unsigned int mask2 = ((utfInt >> 6) & 0b111111) | 0b10000000;
        unsigned int mask3 = ((utfInt >> 12) & 0b1111) | 0b11100000;
        utfInt = (mask3 << 16) | (mask2 << 8) | mask1;
    }

    else if (utfInt >= 0x00010000 && utfInt <= 0x0010FFFF)
    {
        // std::cout << "Fourth row hex!" << std::endl;
        unsigned int mask1 = (utfInt & 0b111111) | 0b10000000;
        unsigned int mask2 = ((utfInt >> 6) & 0b111111) | 0b10000000;
        unsigned int mask3 = ((utfInt >> 12) & 0b111111) | 0b10000000;
        unsigned int mask4 = ((utfInt >> 18) & 0b111) | 0b11110000;
        utfInt = (mask4 << 24) | (mask3 << 16) | (mask2 << 8) | mask1;
    }

    else
    {
        std::cout << "Either too big or unknown!" << std::endl;
    }
    return utfInt;
}

Error lex_init(const char *src_code)
{
    Error er;
    er.line = src_line_no;
    er.col = src_col_no;
    int i = buffer_init(src_code);
    er.error = i;
    return er;
}

Error get_token(Token &t)
{
    Error er;
    char c;

    int i = buffer_get_next_char(c); // get the next character

    if (i == NCC_EOF)
    {
        t.id = TOKEN_EOF;
        // t.line = src_line_no;
        // t.column = src_col_no;
        t.value = "End of file";
    }

    else if (i == NCC_OK)
    {
        if (c == '+')
        {
            t.id = TOKEN_PLUS;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "+";
        }

        else if (c == '-')
        {
            t.id = TOKEN_MINUS;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "-";
        }

        else if (c == '*')
        {
            t.id = TOKEN_MULT;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "*";
        }

        else if (c == '/')
        {
            t.id = TOKEN_DIV;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "/";
        }

        else if (c == '^')
        {
            t.id = TOKEN_EXP;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "^";
        }

        else if (isalpha(c) || c == '_')
        {
            std::string identifier;
            while (true)
            {
                identifier += c;
                i = buffer_get_next_char(c);
                if (i != NCC_OK)
                {
                    er.line = src_line_no;
                    er.col = src_col_no;
                    er.error = i;
                    return er;
                }
                else if (c != '_' && !isalpha(c) && !isdigit(c))
                {
                    i = buffer_back_char();
                    if (i != NCC_OK)
                    {
                        er.line = src_line_no;
                        er.col = src_col_no;
                        er.error = i;
                        return er;
                    }
                    break;
                }
            }

            t.id = TOKEN_IDENT;
            t.value = identifier;
        }

        else if (c == '<') // if its the < sign we need to check for the <= sign
        {
            i = buffer_get_next_char(c);
            if (c == '=')
            {
                t.id = TOKEN_LESS_EQ;
                t.value = "<=";
            }

            else if (c == '-')
            {
                t.id = TOKEN_ASSIGN;
                t.value = "<-";
            }

            else if (c == '<')
            { // it could be the start of a block comment
                i = buffer_get_next_char(c);
                if (c == '-')
                { // means we found the start of a block comment
                    std::string lineComment;
                    while (true)
                    {
                        i = buffer_get_next_char(c); // get the next character

                        if (i != NCC_OK)
                        {
                            // std::cout << "error!!!! i is : " << i << std::endl;
                            er.line = src_line_no;
                            er.col = src_col_no;
                            er.error = NCC_UNEXPECTEDENDOFFILE; // this will be the error if the - is never found
                            return er;
                        }

                        if (c == '-')
                        {                                // if the character we got was a -, it could be the start of a line comment
                            i = buffer_get_next_char(c); // get the next character
                            if (c == '>')                // if it is a >, then get the next charactyer again
                            {
                                i = buffer_get_next_char(c);
                                if (c == '>') // if it was a >, then we have the end of the line comment, so we need to set the value and return it
                                {
                                    t.id = TOKEN_BLOCKCOMMENT;
                                    t.value = lineComment;
                                    er.line = src_line_no;
                                    er.col = src_col_no;
                                    er.error = i;
                                    return er;
                                }
                                else
                                {
                                    lineComment += "->";
                                }
                            }
                            else // if it was not a > then add the - to the string because it was skipped over
                            {
                                lineComment += "-";
                            }
                        }

                        if (i == NCC_OK)
                        {
                            lineComment += c;
                        }
                    }
                }
                else // if a - was not found we need to move back by two and set the token to a <
                {
                    i = buffer_back_char();
                    i = buffer_back_char();
                    t.id = TOKEN_LESS;
                    t.value = '<';
                }
            }

            else // if a < or a - was not detected, then we need to move back by one
            {
                i = buffer_back_char();
                t.id = TOKEN_LESS;
                t.value = '<';
            }
        }

        else if (c == '>')
        {

            // std::cout << " a > character was found! " << std::endl;
            i = buffer_get_next_char(c); // get the next char, if its a = then we have a >= sign
            if (i == NCC_OK)
            {
                if (c == '=')
                {
                    t.id = TOKEN_GREATER_EQ;
                    // t.line = src_line_no;
                    // t.column = src_col_no;
                    t.value = ">=";
                }
                else // if its not back up and set the token to just a > sign
                {
                    // std::cout << " not a >= sign, its was a: " << c << " backing up..." << std::endl;
                    i = buffer_back_char();
                    if (i == NCC_OK)
                    {
                        t.id = TOKEN_GREATER;
                        t.line = src_line_no;
                        t.column = src_col_no;
                        t.value = ">";
                    }
                    else
                    {
                        // std::cout << "something went wrong backing up" << std::endl;
                        er.line = src_line_no;
                        er.col = src_col_no;
                        er.error = i;
                        return er;
                    }
                }
            }
            else
            {
                er.line = src_line_no;
                er.col = src_col_no;
                er.error = i;
                return er;
            }
        }

        else if (c == '=')
        {

            t.id = TOKEN_EQUAL;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "=";
        }

        else if (c == '~')
        {
            i = buffer_get_next_char(c);
            if (c == '=')
            {
                t.id = TOKEN_NOT_EQUAL;
                t.value = "~=";
            }

            else
            {
                t.id = TOKEN_UNKNOWN;
                t.value = "~";
                i = buffer_back_char();
            }
        }

        else if (c == '!')
        {
            t.id = TOKEN_NOT;
            t.value = "!";
        }

        else if (c == '(')
        {
            t.id = TOKEN_LPAREN;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "(";
        }

        else if (c == ')')
        {
            t.id = TOKEN_RPAREN;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = ")";
        }

        else if (c == '{')
        {
            t.id = TOKEN_LBRACE;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "{";
        }

        else if (c == '}')
        {
            t.id = TOKEN_RBRACE;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "}";
        }

        else if (c == '[')
        {
            t.id = TOKEN_LBRACKET;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "[";
        }

        else if (c == ']')
        {
            t.id = TOKEN_RBRACKET;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "]";
        }

        else if (c == '&')
        {
            t.id = TOKEN_AND;
            t.value = "&";
        }

        else if (c == '|')
        {
            t.id = TOKEN_OR;
            t.value = "|";
        }

        else if (c == '.')
        {
            t.id = TOKEN_DOT;
            t.value = ".";
        }

        else if (c == '@')
        {
            t.id = TOKEN_AT;
            t.value = "@";
        }

        else if (isdigit(c)) // is a digit is encountered
        {
            std::string number;   // make a string to hold the digit
            number += c;          // append c to the string first before getting the character
            bool decimal = false; // to keep track of the .,E, and sign. Cant have more than one in a number so we need to keep track
            bool e = false;
            // bool sign = false;
            while (true) // keep geting characters until c is not a digit and the float conditions are ok
            {
                i = buffer_get_next_char(c);
                if (i != NCC_OK) // if something went wrong, return
                {
                    er.line = src_line_no;
                    er.col = src_col_no;
                    er.error = i;
                    return er;
                }
                if (isdigit(c))
                {
                    number += c;
                }

                else if (c == 'e' && e == false && c == 'E')
                {
                    e = true;
                    number += c;
                }

                else if (c == '.' && decimal == false)
                {
                    decimal = true;
                    number += c;
                }

                else
                {
                    break;
                }
            }

            i = buffer_back_char(); // once c is not a digit, back up one character
            if (i == NCC_OK)        // if everything went ok in backing up, set the token to the values
            {
                t.id = TOKEN_INTEGER;
                // t.line = src_line_no;
                // t.column = src_col_no;
                t.value = number;
            }
            else
            {
                er.line = src_line_no;
                er.col = src_col_no;
                er.error = i;
                return er;
            }
        }

        else if (c == '"')
        { // means we found a string
            std::string word;
            while (true)
            {
                i = buffer_get_next_char(c);
                if (c == '\\')
                { // if a back slash is detected.
                    i = buffer_get_next_char(c);
                    if (c == '"')
                    { // if we have a \"", add a " to the string
                        word += static_cast<char>(34);
                    }

                    else if (c == 'n')
                    { // if a \n is detced, add ascii 10
                        word += static_cast<char>(10);
                    }

                    else if (c == 't')
                    {
                        word += static_cast<char>(9);
                    }

                    else if (c == 'r')
                    {
                        word += static_cast<char>(13);
                    }

                    else if (c == '\\')
                    {
                        word += static_cast<char>(92);
                    }

                    else if (c == 'a')
                    {
                        word += static_cast<char>(7);
                    }

                    else if (c == 'b')
                    {
                        word += static_cast<char>(8);
                    }

                    else if (c == '\n')
                    { // if \ newline, add nothing
                        word += "";
                    }

                    else if (c == 'u')
                    { // instructions say it will be 6 hex digits, so get the next 6 hex digits
                        std::string utf;
                        for (size_t j = 0; j <= 5; j++)
                        {
                            i = buffer_get_next_char(c);
                            utf += c;
                        }
                        // std::cout << "UTF ENCOUNTERED: " << utf << std::endl;
                        unsigned int converted = utf_encoding(utf);
                        // std::cout << std::hex << converted << std::endl;
                        word += static_cast<char32_t>(converted); // i dont think this is working
                    }
                }

                else if (i != NCC_OK || c == '"') // stop if i is bad or another quotation mark is encountered
                    break;

                else
                {
                    word += c;
                }
            }

            if (i != NCC_OK) // if error occurs before finding closing quote
            {
                er.line = src_line_no;
                er.col = src_col_no;
                er.error = i;
                return er;
            }

            // std::cout << "read string: " << word << std::endl;

            t.id = TOKEN_STRING;
            t.value = word;
        }

        else if (c == ':')
        {
            t.id = TOKEN_COLON;
            t.value = ":";
        }

        else if (c == ';')
        {
            t.id = TOKEN_SEMICOLON;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = ";";
        }

        else if (c == '#') // means we have a comment
        {
            // std::cout << "found a comment! " << std::endl;
            std::string comment;
            while (true)
            {
                i = buffer_get_next_char(c);
                // std::cout << "adding char: " << c << std::endl;
                if (i == NCC_EOF || c == '\n')
                { // stop at the newline or eof
                    // std::cout << "a new line was detected, stopping loop." << std::endl;
                    break;
                }
                comment += c;
            }
            // std::cout << "comment token value is: " << comment << std::endl;
            t.id = TOKEN_COMMENT;
            t.value = comment;
        }

        else if (c == '\n')
        {
            t.id = TOKEN_NEWLINE;
            // t.line = src_line_no;
            // t.column = src_col_no;
            t.value = "newline";
        }

        else if (isspace(c))
        {
            t.id = TOKEN_WHITESPACE;
            t.value = "space";
        }

        else if (c == ',')
        {
            t.id = TOKEN_COMMA;
            t.value = ",";
        }
        else
        {
            t.id = TOKEN_UNKNOWN;
            // t.line = src_line_no;
            // t.column = src_col_no;
            if (c == '\n')
            {
                t.value = "newline";
            }
            else
            {
                t.value = c;
            }
        }
    }
    else
    {
        t.id = -1;
        t.line = -1;
        t.column = -1;
        t.value = "Something went wrong :(";
    }

    t.line = src_line_no;
    t.column = src_col_no;
    er.line = src_line_no;
    er.col = src_col_no;
    er.error = i;
    return er;
}

bool lex_eof(void)
{
    char c;
    int i = buffer_get_next_char(c);
    if (i == NCC_EOF)
    {
        return true;
    }
    return false;
}

void lex_cleanup()
{
    int i = buffer_cleanup();
    if (i == NCC_OK)
    {
        std::cout << "buffer was erased!" << std::endl;
    }
    else
    {
        std::cout << "something went wring with buffer cleanup" << std::endl;
    }
}