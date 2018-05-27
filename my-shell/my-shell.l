%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>


int yylex();
#define YY_INPUT(buf, result, max_size) result = mygetinput(buf, max_size);

int pathSize = 500;
char *currentDir = (char *) malloc(pathSize * sizeof(char));
char *prompt = (char *)malloc(pathSize * 2 * sizeof(char));

static int mygetinput(char *buf, int size) {
    char *line;
    if(feof(yyin)) return YY_NULL;
    getcwd(currentDir, pathSize);
    snprintf(prompt, 1000, "my-shell:%s > ", currentDir);
    line = readline(prompt);
    if(!line)
        return YY_NULL;
    if(strlen(line) > size-2) {
        fprintf(stderr, "input line too long\n");
        return YY_NULL;
    }
    add_history(line);
    strcpy(buf, line);
    int len = strlen(line);
    buf[len] = '\n';
    free(line);
    return len + 1;
}
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