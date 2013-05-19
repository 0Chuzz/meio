#include "showast.h"

void show_message(FILE *stream, iomessage_t msg){
    char *strtopr = NULL;
    switch(msg->type){
        case CONSTINT:
            fprintf(stream, "%d", msg->value.integer);
            break;
        case CONSTSTR: //XXX escape
            fprintf(stream, "\"%s\"", msg->value.string); 
            break;
        case SYMBOL:
            fprintf(stream, "%s", msg->value.symbol); 
            break;
    }
    if (msg->arguments){
        fputc('(', stream);
        show_arguments(stream, msg->arguments);
        fputc(')', stream);
    } 
    if (msg->next){
        fputc(' ', stream);
        show_message(stream, msg->next);
    }
}


void show_arguments(FILE *stream, iomessage_arguments_t arglist){
    show_suite(stream, arglist->argument);
    if (arglist->next){
        fputc(',', stream);
        show_arguments(stream, arglist->next);
    }
}

void show_suite(FILE *stream, iomessage_suite_t suite){
    if (suite->chain) show_message(stream, suite->chain);
    if (suite->next){
        fputc(';', stream);
        show_suite(stream, suite->next);
    }
}

void show_ast(FILE *stream, ioparser_state_t parser){
    show_suite(stream, parser->parsed_code);
}
