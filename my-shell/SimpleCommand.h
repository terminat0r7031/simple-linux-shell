#ifndef SIMPLECOMMAND_H
#define SIMPLECOMMAND_H

#include <stdlib.h>

class SimpleCommand {
    private:
        unsigned int numberOfAvailabelArguments;
        unsigned int numberOfArguments;
        char **arguments;
    public:
        SimpleCommand() {
            numberOfAvailabelArguments = 5;
            numberOfArguments = 0;
            arguments = (char **) malloc(numberOfAvailabelArguments * sizeof(char *));
        }
        void insertArgument(char *argument) {
            // no more space
            // need to extend space for arguments
            // exec() system call need arguments array end with NULL character
            if(numberOfAvailabelArguments == numberOfArguments + 1) {
                // extend to 5 element
                numberOfAvailabelArguments += 5;
                arguments = (char **)realloc(arguments, numberOfAvailabelArguments * sizeof(char *));
            }

            arguments[numberOfArguments] = argument;
            
            // add NULL character at the end of arguments array
            arguments[numberOfArguments+1] = NULL;
            numberOfArguments++;
        }

        unsigned int getNumberOfArguments() {
            return numberOfArguments;
        }
        char * getArgument(int index) {
            if(index >= 0) {
                return arguments[index];
            }
            else 
                return NULL;
        }

        char **getAllArguments() {
            return arguments;
        }
};

#endif