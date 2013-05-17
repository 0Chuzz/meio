#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argn, char *argv[]){
    int i;
    token_t token;
    char *cur;
    for(i=1; i< argn; i++){
        cur = argv[i];
        do {
            token = scan(cur);
            printf("%s %d\n", TOKEN_ASSTRING(token), token.size);
            cur += token.size;
        } while (token.type != ERROR && token.type != END);
    }
    return 0;
}


