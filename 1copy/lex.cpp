#include "lex.h"
#include <iostream>

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
        t.line = src_line_no;
        t.column = src_col_no;
        t.value = "End of file";
    }

    else if (i == NCC_OK)
    {
        if (c == '+')
        {
            t.id = TOKEN_PLUS;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "+";
        }

        else if (c == '-')
        {
            t.id = TOKEN_MINUS;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "-";
        }

        else if (c == '*')
        {
            t.id = TOKEN_MULT;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "*";
        }

        else if (c == '/')
        {
            t.id = TOKEN_DIV;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "/";
        }

        else if (c == '^')
        {
            t.id = TOKEN_EXP;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "^";
        }

        else if (c == '<') // if its the < sign we need to check for the <= sign
        {
            i = buffer_get_next_char(c); // get the next char, if its a = then we have a <= sign
            if (i == NCC_OK)
            {
                if (c == '=')
                {
                    t.id = TOKEN_LESS_EQ;
                    t.line = src_line_no;
                    t.column = src_col_no;
                    t.value = "<=";
                }
                else // if its not back up and set the token to just a < sign
                {
                    i = buffer_back_char();
                    if (i == NCC_OK)
                    {
                        t.id = TOKEN_LESS;
                        t.line = src_line_no;
                        t.column = src_col_no;
                        t.value = "<";
                    }
                    else
                    {
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

        else if (c == '>')
        {

            t.id = TOKEN_GREATER;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = ">";
            /*
            // std::cout << " a > character was found! " << std::endl;
            i = buffer_get_next_char(c); // get the next char, if its a = then we have a >= sign
            if (i == NCC_OK)
            {
                if (c == '=')
                {
                    t.id = TOKEN_GREATER_EQ;
                    t.line = src_line_no;
                    t.column = src_col_no;
                    t.value = ">=";
                }
                else // if its not back up and set the token to just a > sign
                {
                    std::cout << " not a >= sign, its was a: " << c << " backing up..." << std::endl;
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
                        std::cout << "something went wrong backing up" << std::endl;
                        er.line = src_line_no;
                        er.col = src_col_no;
                        er.error = i;
                        return er;
                    }

            }
            else
            {
                er.line = src_line_no;
                er.col = src_col_no;
                er.error = i;
                return er;
            }
            */
        }

        else if (c == '=')
        {
            i = buffer_get_next_char(c);
            if (i == NCC_OK)
            {
                if (c == '=')
                {
                    t.id = TOKEN_EQUAL;
                    t.line = src_line_no;
                    t.column = src_col_no;
                    t.value = "==";
                }
                else
                {
                    buffer_back_char();
                    t.id = TOKEN_ASSIGN;
                    t.line = src_line_no;
                    t.column = src_col_no;
                    t.value = "=";
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

        else if (c == '!')
        {
            i = buffer_get_next_char(c);
            if (i == NCC_OK)
            {
                if (c == '=')
                {
                    t.id = TOKEN_NOT_EQUAL;
                    t.line = src_line_no;
                    t.column = src_col_no;
                    t.value = "!=";
                }
                else
                {
                    i = buffer_back_char();
                    if (i == NCC_OK)
                    {
                        t.id = TOKEN_NOT;
                        t.line = src_line_no;
                        t.column = src_col_no;
                        t.value = "!";
                    }
                    else
                    {
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

        else if (c == '(')
        {
            t.id = TOKEN_LPAREN;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "(";
        }

        else if (c == ')')
        {
            t.id = TOKEN_RPAREN;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = ")";
        }

        else if (c == '{')
        {
            t.id = TOKEN_LBRACE;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "{";
        }

        else if (c == '}')
        {
            t.id = TOKEN_RBRACE;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "}";
        }

        else if (c == '[')
        {
            t.id = TOKEN_LBRACKET;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "[";
        }

        else if (c == ']')
        {
            t.id = TOKEN_RBRACKET;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "]";
        }

        else if (c == '&')
        {
            i = buffer_get_next_char(c);
            if (i == NCC_OK)
            {
                if (c == '&')
                {
                    t.id = TOKEN_AND;
                    t.line = src_line_no;
                    t.column = src_col_no;
                    t.value = "&&";
                }
                else
                {
                    i = buffer_back_char();
                    if (i == NCC_OK)
                    {
                        t.id = TOKEN_BITWISE_AND;
                        t.line = src_line_no;
                        t.column = src_col_no;
                        t.value = "&";
                    }
                    else
                    {
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

        else if (c == '|')
        {

            i = buffer_get_next_char(c);
            if (i == NCC_OK)
            {
                if (c == '|')
                {
                    t.id = TOKEN_OR;
                    t.line = src_line_no;
                    t.column = src_col_no;
                    t.value = "||";
                }
                else
                {
                    i = buffer_back_char();
                    if (i == NCC_OK)
                    {
                        t.id = TOKEN_BITWISE_OR;
                        t.line = src_line_no;
                        t.column = src_col_no;
                        t.value = "|";
                    }
                    else
                    {
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

        else if (c == '.')
        {
            t.id = TOKEN_DOT;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = ".";
        }

        else if (c == '@')
        {
            t.id = TOKEN_AT;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "@";
        }

        else if (isdigit(c)) // is a digit is encountered
        {
            std::string number; // make a string to hold the digit
            while (isdigit(c))  // Keep geting characters until c is not a digit
            {
                number += c; // append c to the string first before getting the character
                i = buffer_get_next_char(c);
                if (i != NCC_OK) // if something went wrong, return
                {
                    er.line = src_line_no;
                    er.col = src_col_no;
                    er.error = i;
                    return er;
                }
            }

            i = buffer_back_char(); // once c is not a digit, back up one character
            if (i == NCC_OK)        // if everything went ok in backing up, set the token to the values
            {
                t.id = TOKEN_INTEGER;
                t.line = src_line_no;
                t.column = src_col_no;
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
                if (i != NCC_OK || c == '"') // Stop at closing quote or error
                    break;
                word += c;
            }

            if (i != NCC_OK) // If error occurs before finding closing quote
            {
                er.line = src_line_no;
                er.col = src_col_no;
                er.error = i;
                return er;
            }

            t.id = TOKEN_STRING;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = word;
        }

        else if (c == ':')
        {
            t.id = TOKEN_COLON;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = ":";
        }

        else if (c == ';')
        {
            t.id = TOKEN_SEMICOLON;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = ";";
        }

        else if (c == '#')
        {
            t.id = TOKEN_POUND;
            t.line = src_line_no;
            t.column = src_col_no;
            t.value = "#";
        }

        else
        {
            t.id = TOKEN_UNKNOWN;
            t.line = src_line_no;
            t.column = src_col_no;
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

    er.line = src_line_no;
    er.col = src_col_no;
    er.error = i;
    return er;
}
