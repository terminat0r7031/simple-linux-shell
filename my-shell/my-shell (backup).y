%{

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "Command.cpp"

void yyerror(const char *s);
int yylex(void);


Command cmdLine;
SimpleCommand *simpleCmd;
%}


%union {
    char *string_val;
}
%token <string_val> WORD
%token <string_val> STRING
%token NOTOKEN NEWLINE GREAT LESS PIPE AMPERSAND

%%

goal:   NEWLINE {
            cmdLine.prompt();
        }
        |goal NEWLINE {
            cmdLine.prompt();
        }
        | goal command_line
        | command_line        
        ;
    
command_line:   pipe_list io_list background_opt NEWLINE {
                    // cmdLine.print();
                    cmdLine.execute1();
                    cmdLine.prompt();
                    cmdLine.clear();
                }
                ;

pipe_list:  pipe_list PIPE cmd_and_args
            | cmd_and_args
            ;
    
cmd_and_args:   command_word arg_list {
                    cmdLine.insertSimpleCommand(simpleCmd);
                }
                ;

command_word:   WORD {
                    simpleCmd = new SimpleCommand();
                    simpleCmd->insertArgument($1);
                }
                ;

arg_list:   arg_list WORD {
                simpleCmd->insertArgument($2);
            }
            |
            ;

io_list:    io_list io_cmd
            |
            ;

io_cmd:    GREAT WORD {
                    cmdLine.setOutFile($2);
                }
                | LESS WORD {
                    cmdLine.setInFile($2);
                }
                ;

background_opt: AMPERSAND {
                    cmdLine.setBackgroundOpt(1);
                }
                |
                ;

%%

sig_atomic_t sigFlag = 0;

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

void catch_int(int sig_num) {
    sigFlag = 1;
}

int main(void) {
    // clear screen
    signal(SIGINT, catch_int);
    system("@cls|clear");
    cmdLine.prompt();
    yyparse();
}