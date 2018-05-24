%{
    int yylineno;
%}

%%

^(.*)\n     printf("%4d\t%s\n", yylineno, yytext);

%%

int main(int argc, char *argv[]) {
    yyin = fopen(argv[1], "r");
    yylex();
    fclose(yyin);
}

int yywrap(void) {
    return 1;
}