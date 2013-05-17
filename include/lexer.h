#ifndef __LEXER_H
#define __LEXER_H

typedef struct _token{
    enum {
        ERROR,
        END,
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
    char *symbol;
    unsigned int size;
} token_t;

static const char *_token_names[] = {
    "ERROR",
    "END",
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
#define TOKEN_ASSTRING(token) _token_names[token.type]

token_t scan(char *p);
#endif
