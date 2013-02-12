#include <stdlib.h>
#include "parser.h"

iomessage_t* parse(char *code){
    token_t currtoken;
    iomessage_t *message = NULL;
    do {
        currtoken = scan(code);
        switch(currtoken.type) {
        }
        code += currtoken.size;
    } while ( currtoken.type != ERROR && currtoken.type != END);
    
    return message;
}
