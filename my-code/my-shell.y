%{

extern "C" int yylex();
#define yylex yylex
#include <stdio.h>
#include "Command.cpp"

Command cmd;
SimpleCommand *SimpleCommand;

%}

%union {
    char *string_val;
}
%token <string_val> WORD
%token NOTOKEN NEWLINE GREAT LESS PIPE

%%

command_line:   pipe_list io_modifier NEWLINE {
                    cmdLine.print();
                    cmdLine.prompt();
                }
                | NEWLINE
                | error NEWLINE { yyerrok; }
                ;

pipe_list:  pipe_list PIPE cmd_and_args
            | cmd_and_args
            ;
    
cmd_and_args:   command_word arg_list {
                    cmd.insertSimpleCommand(simpleCmd);
                }
                ;

command_word:   WORD {
                    simpleCmd = new SimpleCommand();
                    simpleCmd.insertArgument($1);
                }
                ;

arg_list:   arg_list WORD {
                simpleCmd.insertArgument($2);
            }
            |
            ;

io_modifier:    GREAT WORD {
                    cmd.setInFile($2);
                }
                | LESS WORD {
                    cmd.setOutFile($2);
                }
                |
                ;


%%

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    yyparse();
}