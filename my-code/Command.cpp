#include "Command.h"

Command::Command() {
    numberOfAvailabelSimpleCommands = 1;
    numberOfSimpleCommands = 0;
    simpleCommands = (SimpleCommand **) malloc(numberOfAvailabelSimpleCommands * sizeof(SimpleCommand *));
    inFile = 0;
    outFile = 0;
}

void Command::insertSimpleCommand(SimpleCommand *simpleCommand) {
    // if no more space for simpleCommands array
    if(numberOfSimpleCommands == numberOfAvailabelSimpleCommands) {
        // extend to 1
        numberOfAvailabelSimpleCommands++;
        simpleCommands = (SimpleCommand **) realloc(simpleCommands, numberOfAvailabelSimpleCommands * sizeof(SimpleCommand *));
        simpleCommands[numberOfSimpleCommands] = simpleCommand;
        numberOfSimpleCommands++;
    }
}

void Command::setInFile(char *inFileName) {
    inFile = inFileName;
}

void Command::setOutFile(char *outFileName) {
    outFile = outFileName;
}

void Command::prompt() {
    printf("my-shell>");
    fflush(stdin);
}
void Command::print() {
    printf("\n\n");
    printf("                COMMAND TABLE                   ");
    printf("\n");
    printf("  #   Simple Commands\n");
    printf("  --- ------------------------------------------");
    for(int i = 0; i < numberOfSimpleCommands; i++) {
        printf("  %-3d ", i);
        for(int j = 0; j < simpleCommands[i]->getNumberOfArguments(); j++)
            printf("\"%s\"\t", simpleCommands[i]->getArgument(j));
    }

    printf("\n\n");
    printf("  Output       Input       \n");
    printf("  ------------ ------------\n");
    printf("%-12s %-12s", outFile?outFile:"stdout", inFile?inFile:"stdin");
    printf("\n\n");

}