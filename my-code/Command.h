#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>                 // for malloc, realloc
#include <stdio.h>
#include "SimpleCommand.cpp"


class Command {

    private:
        unsigned int numberOfAvailabelSimpleCommands;
        unsigned int numberOfSimpleCommands;
        SimpleCommand **simpleCommands;
        char *outFile;
        char *inFile;

    public:
        Command();
        void insertSimpleCommand(SimpleCommand *simpleCommand);
        void setInFile(char *inFileName);
        void setOutFile(char *outFileName);
        void prompt();
        void print();
        void execute();
        void clear();
};
    
#endif