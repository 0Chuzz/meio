#include <stdlib.h>
#include "parser.h"

static __thread ioparser_state_t *p_state;

static enum typeofparens token2parens(token_t tok){
    switch(tok.type){
        case LPAREN:
        case RPAREN:
            return PARENS;
        case LBRACKET:
        case RBRACKET:
            return BRACKET;
        case LCURLY:
        case RCURLY:
            return CURLY;
    }
    return BASE;
}

static iomessage_t make_message(token_t tok){
    iomessage_t ret = malloc(sizeof *iomessage_t);
    ret->next = NULL;
    ret->arguments = NULL;
    switch(tok.type){
        case STRING:
            char *str = malloc(tok->len);
            memcpy(str, tok->str, tok->len);
            str[tok->len] = 0;
            ret->value.str = str;
            ret->type = STRING;
            break;
        case HEXNUMBER:
        case NUMBER:
            ret->value.integer = 123;
            ret->type = INTEGER;
            break;
    }
    return ret;
}

static int push_message(iomessage_t message){
    if(p_state->stack->curr_suite->chain == NULL){
        p_state->stack->curr_suite->chain = message;
    } else {
        p_state->stack->curr_msg->next = message;
    }
    p_state->stack->curr_msg = message;
    return 0;
}

static iomessage_suite_t new_suite(){
    iomessage_suite_t ret = malloc(sizeof *iomessage_suite_t);
    ret->next = NULL;
    ret->chain = NULL;
    return ret;
}

static int push_suite(){
    p_state->stack->curr_suite = new_suite();
    p_state->stack->curr_msg = NULL;
    return 0;
}

static iomessage_arguments_t new_arg_list(){
    iomessage_arguments_t arglist = malloc(sizeof *iomessage_arguments_t);
    arglist->next = NULL;
    arglist->argument = NULL;
    return arglist;
}

static void push_new_arguments(token_t tok){
    if(p_state->stack->curr_msg == NULL){
        push_message(make_emptymessage())
    }
    iomessage_arguments_t arglist = new_arg_list();
    p_state->stack->curr_msg->arguments = arglist;
    push_prev_state(p_state);
    p_state->stack->type = token2parens(tok);
    p_state->stack->curr_arg = arglist;
    p_state->stack->curr_msg = NULL;
    p_state->stack->curr_suite = new_suite();
}

static int push_next_argument(){
    if(p_state->stack->type == BASE) return 1;
    iomessage_arguments_t arglist = new_arg_list();
    p_state->stack->curr_arg->next = arglist;
    p_state->stack->curr_arg->argument = 
        p_state->stack->curr_suite;
    p_state->stack->curr_arg = arglist;
    p_state->stack->curr_msg = NULL;
    p_state->stack->curr_suite = new_suite();
    return 0;
}

static int pop_arguments(token_t tok){
    if(p_state->stack->type != token2parens(tok)) return 1;
    push_next_argument();
    struct _argstack *prev = p_state->stack->previous;
    free(p_state->stack);
    p_state->stack = prev;
    return 0;
}

int parse_token( token_t *currtoken){
    iomessage_t *message = NULL;
    char *mname = NULL;
    int eom = 0;
    switch(currtoken->type) {
        case WHITESPACE:
            return 0;
        case NUMBER:
        case HEXNUMBER:
        case STRING:
        case OPERATOR:
        case IDENTIFIER:
            return push_message(make_message(currtoken));
        case LBRACKET:
        case LCURLY:
            if(push_message(make_message(currtoken))) return 1;
        case LPAREN:
            push_new_arguments(currtoken);
            return 0;
        case COMMA:
            return push_next_argument();
        case RPAREN:
        case RCURLY:
        case RBRACKET:
            return pop_arguments(currtoken);
        case ENDEXPR:
            return push_suite();
        case END:
            error_if_parentheses_open();
            return 1;
        case ERROR:
            error_lexer(currtoken);
            break;
    }
    return -1;
}

