#ifndef __PARSER_H
#define __PARSER_H
#include "lexer.h"
typedef struct _iomessage *iomessage_t;
typedef struct _iomessage_suite *iomessage_suite_t;
typedef struct _iomessage_arguments *iomessage_arguments_t;

typedef struct _iomessage{
    enum {
        STRING,
        INTEGER,
        SYMBOL,
    } type;
    union {
        char *string;
        int integer;
        char *symbol;
    } value;
    iomessage_arguments_t arguments;
    iomessage_t next;
}  *iomessage_t;

typedef struct _iomessage_suite {
    iomessage_t chain;
    iomessage_suite_t next;
} *iomessage_suite_t;

typedef struct _iomessage_arguments {
        iomessage_suite_t argument;
        iomessage_arguments_t next;
} *iomessage_arguments_t;


typedef struct _ioparser_state{
    int flags;
    struct _argstack {
        enum typeofparens{
           BASE,
           BRACKET,
           CURLY,
           PARENS,
        } type;
        iomessage_t curr_msg;
        iomessage_suite_t curr_suite;
        iomessage_arguments_t curr_arg;
        struct _argstack *previous;
    } *stack;
} *ioparser_state_t;

int parse_token(ioparser_state_t*, token_t);
#endif
