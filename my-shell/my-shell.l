%{
#include "y.tab.h"
#include <string.h>
int yylex();
%}

%%

"\n"    {
    return NEWLINE;
}

[ \t]   {
    /* discard spaces and tabs  
     do nothing;
    */
}

">"     {
    return GREAT;
}

"<"     {
    return LESS;
}

"|"     {
    return PIPE;
}

"&" {
    return AMPERSAND;
}

\"([^\"]|\\\")*\" {
    /* string is encapsulated by double quote */
    yylval.string_val = strdup(yytext);
    return STRING;
}

[^ \t\n]+   {
    /* word doesn't contain spaces, tabs, or linefeed */
    yylval.string_val = strdup(yytext);
    return WORD;
}



%%

int yywrap(void) {
    return 1;
}