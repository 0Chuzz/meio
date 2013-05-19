#ifndef __SHOWAST_H
#define __SHOWAST_H
#include <stdio.h>
#include "parser.h"

void show_message(FILE *stream, iomessage_t msg);
void show_arguments(FILE *stream, iomessage_arguments_t arglist);
void show_suite(FILE *stream, iomessage_suite_t suite);
void show_ast(FILE *stream, ioparser_state_t parser);
#endif
