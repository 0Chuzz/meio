
typedef struct token{
    enum {
        END,
        ERROR,
        WHITESPACE,
        ENDEXPR,
        NUMBER,
        HEXNUMBER,
        STRING,
        OPERATOR,
        IDENTIFIER,
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
    "END",
    "ERROR",
    "WHITESPACE",
    "ENDEXPR",
    "NUMBER",
    "HEXNUMBER",
    "STRING",
    "OPERATOR",
    "IDENTIFIER",
    "LPAREN",
    "RPAREN",
    "LBRACKET",
    "RBRACKET",
    "LCURLY",
    "RCURLY"
};

token_t scan(char *p);
