#ifndef __LEXER_H
#define __LEXER_H

typedef struct token{
    enum {
        ERROR,
        ENDEXPR,
        WHITESPACE,
        NUMBER,
        HEXNUMBER,
        STRING,
        OPERATOR,
        IDENTIFIER,
        COMMA,
        LPAREN,
        RPAREN,
        LBRACKET,
        RBRACKET,
        LCURLY,
        RCURLY
    } type;
    unsigned int size;
} token_t;

static const char *token_as_str[] = {
    "ERROR",
    "ENDEXPR",
    "WHITESPACE",
    "NUMBER",
    "HEXNUMBER",
    "STRING",
    "OPERATOR",
    "IDENTIFIER",
    "COMMA",
    "LPAREN",
    "RPAREN",
    "LBRACKET",
    "RBRACKET",
    "LCURLY",
    "RCURLY"
};

token_t scan(char *p);
#endif
