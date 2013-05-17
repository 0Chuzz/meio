#include"lexer.h"

#define TOKRETURN(T) return ((token_t){.type=(T), .size=(p-start), \
        .symbol=start})


token_t scan(char *p)
{
    char *marker, *start = p;

/*!re2c
        re2c:define:YYCTYPE  = "unsigned char";
        re2c:define:YYCURSOR = p;
        re2c:define:YYMARKER = marker;
        re2c:yyfill:enable   = 0;
        re2c:yych:conversion = 1;
        re2c:indent:top      = 1;
        [0-9]+                          {TOKRETURN(NUMBER);}
        "0x"[0-9a-fA-F]+                {TOKRETURN(HEXNUMBER);}
        [a-zA-Z_\x80-\xff]+             {TOKRETURN(IDENTIFIER);}
        [+\-/^*=><:']+                   {TOKRETURN(OPERATOR);}
        [\n;]                       {TOKRETURN(ENDEXPR);}
        [ \t\r]                         {TOKRETURN(WHITESPACE);}
        '"'([\\][^\000]|[^"\000])*'"'   {TOKRETURN(STRING);}
        ","                             {TOKRETURN(COMMA);}
        "("                             {TOKRETURN(LPAREN);}
        ")"                             {TOKRETURN(RPAREN);}
        "["                             {TOKRETURN(LBRACKET);}
        "]"                             {TOKRETURN(RBRACKET);}
        "{"                             {TOKRETURN(LCURLY);}
        "}"                             {TOKRETURN(RCURLY);}
        [\000]                          {TOKRETURN(END);}
        [^]                             {TOKRETURN(ERROR);}

*/
    TOKRETURN(ERROR);
}
