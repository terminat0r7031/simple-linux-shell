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

[^ \t\n]+   {
    /* word doesn't contain spaces, tabs, or linefeed */
    yylval.string_val = strdup(yytext);
    return WORD;
}

.   {
    return NOTOKEN;
}


%%

int yywrap(void) {
    return 1;
}