#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>                 // for malloc, realloc
#include <stdio.h>
#include <unistd.h>                 // for fork(), execvp()
#include <sys/types.h>              
#include <sys/wait.h>               // for waitpid()
#include <sys/stat.h>
#include <fcntl.h>                  // for open()
#include <string.h>
#include <locale.h>
#include <X11/Xlib.h>               // for title() build-in function
#include <X11/Xmu/WinUtil.h>

#include "SimpleCommand.h"


class Command {

    private:
        unsigned int numberOfAvailabelSimpleCommands;
        unsigned int numberOfSimpleCommands;
        SimpleCommand **simpleCommands;
        char *outFile;
        char *inFile;
        unsigned int background;
        
        char *currentDir ;
        unsigned long pathSize;

        char const *buildInCommands[3] = {"cd", "exit", "owbtitle"};
        unsigned int numberOfAvailabelBuildInCommands = 3;

        int searching = 0;
        Display *openDisplay();
        void setOnTop(Display *display, Window window);
        void searchTitle(Display *display, Window window, char *title);
        

    public:
        Command();
        void insertSimpleCommand(SimpleCommand *simpleCommand);
        void setInFile(char *inFileName);
        void setOutFile(char *outFileName);
        void setBackgroundOpt(unsigned int opt);

        void prompt();
        void print();
        void execute();
        void execute1();
        void clear();

        int isBuildInCommand(char *cmd);
        void insertBuildInCommand(char * cmd);

        // build in command
        void cdFunc(char *path);
        void titleFunc(char *title);
};
    
#endif