#include <stdlib.h>
#include <string.h>
#include "parser.h"

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
static iomessage_t new_message(){
    iomessage_t ret = malloc(sizeof *ret);
    ret->next = NULL;
    ret->arguments = NULL;
    return ret;
}

static iomessage_suite_t new_suite(){
    iomessage_suite_t ret = malloc(sizeof *ret);
    ret->next = NULL;
    ret->chain = NULL;
    return ret;
}

static iomessage_arguments_t new_arg_list(iomessage_suite_t arg){
    iomessage_arguments_t arglist = malloc(sizeof *arglist);
    arglist->next = NULL;
    arglist->argument = arg;
    return arglist;
}

static iomessage_t make_symbol(char * name){
    iomessage_t ret = new_message();
    ret->type = SYMBOL;
    ret->value.symbol= name;
    return ret;
}

static void push_stack_frame(ioparser_state_t p_state){
    struct _argstack *newframe = malloc(sizeof *newframe);
    newframe->previous = p_state->stack;
    p_state->stack = newframe;
}

static int push_message(ioparser_state_t p_state, iomessage_t message){
    if(p_state->stack->curr_suite->chain == NULL){
        p_state->stack->curr_suite->chain = message;
    } else {
        p_state->stack->curr_msg->next = message;
    }
    p_state->stack->curr_msg = message;
    return 0;
}

static int push_suite(ioparser_state_t p_state){
    iomessage_suite_t nsuite = new_suite();
    p_state->stack->curr_suite->next = nsuite;
    p_state->stack->curr_suite = nsuite;
    p_state->stack->curr_msg = NULL;
    return 0;
}

static void push_new_arguments(ioparser_state_t p_state, token_t tok){
    if(p_state->stack->curr_msg == NULL){
        push_message(p_state, make_symbol(""));
    }
    iomessage_arguments_t arglist = new_arg_list(new_suite());
    p_state->stack->curr_msg->arguments = arglist;
    push_stack_frame(p_state);
    p_state->stack->type = token2parens(tok);
    p_state->stack->curr_arg = arglist;
    p_state->stack->curr_msg = NULL;
    p_state->stack->curr_suite = arglist->argument;
}

static int push_next_argument(ioparser_state_t p_state){
    if(p_state->stack->type == BASE) return 1;
    iomessage_arguments_t arglist = new_arg_list(new_suite());
    p_state->stack->curr_arg->next = arglist;
    p_state->stack->curr_arg = arglist;
    p_state->stack->curr_msg = NULL;
    p_state->stack->curr_suite = arglist->argument;
    return 0;
}

static int pop_arguments(ioparser_state_t p_state, token_t tok){
    if(p_state->stack->type != token2parens(tok)) return 1;
    push_next_argument(p_state);
    struct _argstack *prev = p_state->stack->previous;
    free(p_state->stack);
    p_state->stack = prev;
    return 0;
}

ioparser_state_t init_parser(){
    ioparser_state_t p_state = malloc(sizeof *p_state);
    p_state->stack = NULL;
    push_stack_frame(p_state);
    p_state->stack->type = BASE;
    p_state->stack->curr_arg = NULL;
    p_state->stack->curr_suite = new_suite();
    p_state->stack->curr_msg = NULL;
    return p_state;
}


int parse_token(ioparser_state_t p_state, token_t *pcurrtoken){
    char *mname = NULL;
    token_t currtoken = *pcurrtoken;
    iomessage_t msg = NULL;
    int eom = 0;
    switch(currtoken.type) {
        case WHITESPACE:
            return 0;
        case NUMBER:
        case HEXNUMBER:
            msg = new_message();
            msg->type = CONSTINT;
            msg->value.integer = 123; //XXX
            return push_message(p_state, msg);
        case STRING:
            msg = new_message();
            msg->type = CONSTSTR;
            msg->value.string = "asdasd"; //XXX
            return push_message(p_state, msg);
        case OPERATOR:
        case IDENTIFIER:
            mname = strndup(currtoken.symbol, currtoken.size);
            msg = make_symbol(mname);
            msg->type = SYMBOL;
            msg->value.symbol = mname;
            return push_message(p_state, msg);
        case LBRACKET:
            msg = make_symbol("bracket");
        case LCURLY:
            if(msg == NULL) msg = make_symbol("curly");
            if(push_message(p_state, msg)) return 1;
        case LPAREN:
            push_new_arguments(p_state, currtoken);
            return 0;
        case COMMA:
            return push_next_argument(p_state);
        case RPAREN:
        case RCURLY:
        case RBRACKET:
            return pop_arguments(p_state, currtoken);
        case ENDEXPR:
            return push_suite(p_state);
        case END:
            return 1;
        case ERROR:
            break;
    }
    return -1;
}

