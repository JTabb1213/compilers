#ifndef NCC_TOKEN_H
#define NCC_TOKEN_H
#include <string>
/*******     Token  id  values   ******/
#define TOKEN_NULL 0
#define TOKEN_EOF 1
#define TOKEN_PLUS 2
#define TOKEN_MINUS 3
#define TOKEN_MULT 4
#define TOKEN_DIV 5
#define TOKEN_EXP 6
#define TOKEN_LESS 7
#define TOKEN_LESS_EQ 8
#define TOKEN_GREATER 9
#define TOKEN_GREATER_EQ 10
#define TOKEN_EQUAL 11
#define TOKEN_NOT_EQUAL 12
#define TOKEN_ASSIGN 13
#define TOKEN_NOT 14
#define TOKEN_LPAREN 15
#define TOKEN_RPAREN 16
#define TOKEN_LBRACE 17
#define TOKEN_RBRACE 18
#define TOKEN_LBRACKET 19
#define TOKEN_RBRACKET 20
#define TOKEN_AND 21
#define TOKEN_OR 22
#define TOKEN_DOT 23
#define TOKEN_AT 24
#define TOKEN_COLON 25
#define TOKEN_SEMICOLON 26
#define TOKEN_COMMA 27
#define TOKEN_INTEGER 28
#define TOKEN_REAL 29
#define TOKEN_STRING 30
#define TOKEN_COMMENT 32
#define TOKEN_ERROR 33
#define TOKEN_BITWISE_NOT 34
#define TOKEN_BITWISE_OR 35
#define TOKEN_BITWISE_AND 36
#define TOKEN_UNKNOWN 37
#define TOKEN_POUND 38
#define TOKEN_SHIFT 39
#define TOKEN_LINECOMMENT 40
#define TOKEN_WHITESPACE 41
#define TOKEN_NEWLINE 42
#define TOKEN_BLOCKCOMMENT 1000
#define TOKEN_IDENT 43
// etc. etc.

struct Token
{
    int id;
    int line;
    int column;
    std::string value;
    // and anything else that might be needed.
};

void print_token(const Token &t);

#endif /* NCC_TOKEN_H */