#include <iostream>
#include <vector>
#include "lex.h"
#include "tree.h"
#include "makeInstructions.h"

void printVector(const std::vector<Token> &vec)
{
    std::cout << "Vector contents: ";
    for (const Token &token : vec)
    {
        std::cout << "[" << token.id << ": " << token.value << "] ";
    }
    std::cout << std::endl;
}

void processTree(std::vector<Token> t)
{

    for (size_t i = t.size(); i > 0; i--)
    { // erase all the nelinw tokens they were causing trouble
        if (t[i - 1].id == TOKEN_NEWLINE)
        {
            t.erase(t.begin() + (i - 1));
        }
    }
    // printVector(t);
    TreeNode *root = E(t); // call the parse expression, passing the entire expression (which is the vector of all the tokens).

    if (root)
    {
        std::cout << "Parsing successful! Tree constructed." << std::endl;
        printTree(root, "");
        std::cout << std::endl;
        generateIntelInstructions(root);
        std::cout << std::endl;
        std::cout << std::endl;
        deleteTree(root);
        std::cout << "Tree was erased :)" << std::endl;
        // std::cout << "HI" << std::endl;
    }
    else
    {
        std::cerr << "Parsing failed!" << std::endl;
    }
}
int main()
{
    int lastID = 0; // for checking if there are two of the same token types in a row
    const char *source_code;
    std::string file;
    std::cout << "Enter the file you want to use: ";
    std::cin >> file;
    source_code = file.c_str();

    Error err = lex_init(source_code);
    if (err.error != NCC_OK)
    {
        std::cerr << "Lexical analysis initialization failed!" << std::endl;
        return 1;
    }

    Token token;
    std::vector<Token> t;
    while (true)
    {
        err = get_token(token);

        if (err.error == NCC_EOF)
        {
            print_token(token);
            processTree(t);
            break;
        } // break from loop when the eof token is read

        if (err.error != NCC_OK && err.error != NCC_EOF)
        {
            print_error(err);
            std::cout << "Error occured while reading: " << token.value << std::endl;
            lex_cleanup();
            return 1;
        }

        // print_token(token);
        if (!t.empty())
        {
            lastID = t.back().id;
        }

        if (token.id != TOKEN_WHITESPACE && token.id != TOKEN_LINECOMMENT && token.id != TOKEN_COMMENT && token.id != TOKEN_UNKNOWN /*&& token.id != TOKEN_NEWLINE*/)
        {
            if (lastID == TOKEN_INTEGER && token.id == TOKEN_INTEGER)
            {
                std::cout << "TWO" << std::endl;
                processTree(t);
                t.clear();
            }
            else if (lastID == TOKEN_RPAREN && token.id == TOKEN_LPAREN)
            {
                std::cout << "TWO" << std::endl;
                processTree(t);
                t.clear();
            }
            else if (lastID == TOKEN_RPAREN && token.id == TOKEN_INTEGER)
            {
                std::cout << "TWO" << std::endl;
                processTree(t);
                t.clear();
            }

            else if (lastID == TOKEN_NEWLINE && token.id == TOKEN_NEWLINE)
            { //
                std::cout << "NL" << std::endl;
                processTree(t);
                t.clear();
            }

            else if (token.id == TOKEN_INTEGER && lastID == TOKEN_NEWLINE && t[t.size() - 2].id == TOKEN_INTEGER)
            {
                /*
                std::cout << "Here2" << std::endl;
                std::cout << "INTERGER, NEWLINE, INTEGER" << std::endl;
                std::cout << "Here1" << std::endl;
                if (t.empty())
                {
                    std::cout << "3" << std::endl;
                }
                else
                {
                    std::cout << "4" << std::endl;
                }
                */
                processTree(t);
                // std::cout << "Here5" << std::endl;
                t.clear();
            }

            else if (token.id == TOKEN_LPAREN && lastID == TOKEN_NEWLINE && t[t.size() - 2].id == TOKEN_INTEGER)
            {
                std::cout << "LP, NEWLINE, INTEGER" << std::endl;
                processTree(t);
                t.clear();
            }
            t.push_back(token); // add the token to the vector, excluding whitespace, EOF, comment, unknown, and newline
        }
    }

    lex_cleanup(); // delete the lex analizer

    // processTree(t);
    return 0;
}
