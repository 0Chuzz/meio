#ifndef __PARSER_H
#define __PARSER_H
#include "lexer.h"

typedef struct _iomessage{
    char *name;
    struct _iomessage **arguments;
    struct _iomessage *next;
}  iomessage_t;


iomessage_t *parse(char *code);
#endif
