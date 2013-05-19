#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

int main(int argn, char *argv[]){
    int i;
    token_t token;
    char *cur;
    ioparser_state_t pstate = init_parser();
    for(i=1; i< argn; i++){
        cur = argv[i];
        do {
            token = scan(cur);
            printf("%s %d\n", TOKEN_ASSTRING(token), token.size);
            cur += token.size;
            parse_token(pstate, &token);
        } while (token.type != ERROR && token.type != END);
    }
    return 0;
}


