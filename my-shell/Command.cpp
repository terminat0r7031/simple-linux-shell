#include "Command.h"

Command::Command() {
    numberOfAvailabelSimpleCommands = 1;
    numberOfSimpleCommands = 0;
    simpleCommands = (SimpleCommand **) malloc(numberOfAvailabelSimpleCommands * sizeof(SimpleCommand *));

    inFile = 0;
    outFile = 0;
    background = 0;

    // pathSize = 500;
    // currentDir = (char *) malloc(pathSize * sizeof(char));
    // getcwd(this->currentDir, pathSize);
}

void Command::insertSimpleCommand(SimpleCommand *simpleCommand) {
    // if no more space for simpleCommands array
    if(numberOfSimpleCommands == numberOfAvailabelSimpleCommands) {
        // extend to 1
        numberOfAvailabelSimpleCommands++;
        simpleCommands = (SimpleCommand **) realloc(simpleCommands, numberOfAvailabelSimpleCommands * sizeof(SimpleCommand *));
    }
    simpleCommands[numberOfSimpleCommands] = simpleCommand;
    numberOfSimpleCommands++;
}

void Command::setInFile(char *inFileName) {
    inFile = inFileName;
}

void Command::setOutFile(char *outFileName) {
    outFile = outFileName;
}

void Command::setBackgroundOpt(unsigned int opt) {
    background = opt;
}

// void Command::prompt() {
//     printf("my-shell:%s > ", currentDir);
//     fflush(stdin);
// }
void Command::print() {
    printf("\n\n");
    printf("                COMMAND TABLE                   ");
    printf("\n");
    printf("  #   Simple Commands\n");
    printf("  --- ------------------------------------------\n");
    for(int i = 0; i < numberOfSimpleCommands; i++) {
        printf("  %-3d ", i);
        for(int j = 0; j < simpleCommands[i]->getNumberOfArguments(); j++)
            printf("\"%s\"\t", simpleCommands[i]->getArgument(j));
        printf("\n");
    }

    printf("\n\n");
    printf("  Output       Input        Background\n");
    printf("  ------------ ------------ ------------\n");
    printf("  %-12s %-12s %-12d\n", outFile?outFile:"stdout", inFile?inFile:"stdin", background);
    printf("\n\n");

}

void Command::clear() {

    // free simpleCommands array's element
    for(int i = 0; i < numberOfSimpleCommands; i++) {
        for(int j = 0; j < simpleCommands[i]->getNumberOfArguments(); j++)
            if(simpleCommands[i]->getArgument(j) != NULL)
                free(simpleCommands[i]->getArgument(j));
    }

    // free simpleCommands array
    free(simpleCommands);
    simpleCommands = (SimpleCommand **) malloc(numberOfAvailabelSimpleCommands * sizeof(SimpleCommand *));
    numberOfSimpleCommands = 0;
    // set in, out file to stdin stdout
    
    inFile = 0;
    outFile = 0;
    background = 0;

}

void Command::execute() {
    int pid, status;
    for(int i = 0; i < numberOfSimpleCommands; i++) {
        pid = fork();
        if(pid == 0) { //child process
            execvp(simpleCommands[i]->getArgument(0), simpleCommands[i]->getAllArguments());
            perror("execvp() error!");
            abort();
        }
        else if (pid < 0) {
            perror("fork() error!");
            return;
        } else {
            // if background_opt is set, then the shell has to wait to all his childrens
            if(background == 0)
                do {
                    waitpid(pid, &status, WUNTRACED);
                } while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
}
void Command::execute1() {

    // save stdin/stdout
    // dup(int oldfd) function create a copy of the file description oldfd
    int tmpstdin = dup(0);     // 0 -> stdin file descriptor 
    int tmpstdout = dup(1);    // 1 -> stdout file descriptor

    int fdin;
    if(inFile) {
        fdin = open(inFile, O_RDONLY);
    }
    else {
        fdin = dup(tmpstdin);
    }

    int pid;
    int fdout;
    for(int i = 0; i < numberOfSimpleCommands; i++) {

        // redirect input
        dup2(fdin, 0);          // now fdin has file descriptor number = 0
        close(fdin);            // no longer be needed, using 0 instead as fdin

        // setup output
        if(i == numberOfSimpleCommands - 1) {   // the last command
            if(outFile) {
                fdout = open(outFile, O_CREAT | O_WRONLY, S_IWUSR);
            }
            else {
                fdout = dup(tmpstdout);
            }
        }

        else {

            // create pipe
            int fdpipe[2];
            if(pipe(fdpipe) != 0 ) {
                perror("pipe error!");
            }
            fdout = fdpipe[1];
            fdin = fdpipe[0];
        }

        // redirect output
        dup2(fdout, 1);
        close(fdout);

        int index = isBuildInCommand(simpleCommands[i]->getArgument(0));
        if(index != -1) {
            if(strcmp(buildInCommands[index], "cd") == 0) {
                cdFunc(simpleCommands[i]->getArgument(1));
            }
            if(strcmp(buildInCommands[index], "exit") == 0) {
                exit(0);
            }
            if(strcmp(buildInCommands[index], "owbtitle") == 0) {
                titleFunc(simpleCommands[i]->getArgument(1));
            }
            if(strcmp(buildInCommands[index], "word") == 0) {
                officeFunc("Microsoft Word 2010", simpleCommands[i]->getArgument(1));
            }
            if(strcmp(buildInCommands[index], "excel") == 0) {
                officeFunc("Microsoft Excel 2010", simpleCommands[i]->getArgument(1));
            }
            if(strcmp(buildInCommands[index], "ppoint") == 0) {
                officeFunc("Microsoft Powerpoint 2010", simpleCommands[i]->getArgument(1));
            }
        }
        else {
            // create child process
            pid = fork();
            if(pid == 0) {
                execvp(simpleCommands[i]->getArgument(0), simpleCommands[i]->getAllArguments());
                perror("execvp() error!");
                abort();
            }
        }
    }

    dup2(tmpstdin, 0);
    dup2(tmpstdout, 1);
    close(tmpstdin);
    close(tmpstdout);

    if(!background) {
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int Command::isBuildInCommand(char *cmd) {
    for(int i = 0; i < numberOfAvailabelBuildInCommands; i++) {
        if(strcmp(cmd, buildInCommands[i]) == 0)
            return i;
    }
    return -1;
}

void Command::cdFunc(char *path) {
    if(path == NULL) {
        printf("\nmy-shell: expected argument to \"cd\"\n");
    }
    else {
        if(chdir(path) == 0) {      // on success, change the currentDir property to path
            // getcwd(currentDir, pathSize);
        } else {
            printf("cd: %s: No such file or directory\n", path);
        }
    }
}

void Command::titleFunc(char *title) {

    if(title == NULL) {
        printf("owbtitle: window's title is required !\n");
        return;
    }

    setlocale(LC_ALL, "");
    
    Display *display = openDisplay();
    Window root = XDefaultRootWindow(display);

    Window parent;
    Window *children;
    unsigned int nChildren;

    int status = XQueryTree(display, root, &root, &parent, &children, &nChildren);
    if(status == 0) { // query failed
        printf("XQueryTree failed!\n");
        return;
    }
    if (nChildren == 0) {  // no children
        printf("No children\n");
        return;
    }

    searching = 1;
    int i;
    for(i = 0; i < nChildren; i++) {
        searchTitle(display, children[i], title);
        if(searching == 0)
            break;
    }
    if(i == nChildren)
        printf("owbtitle: no such window has title: '%s'\n", title);
}

Display * Command::openDisplay() {
    // connect to X Server
    Display *display = XOpenDisplay(NULL);
    if(display == NULL) {
        printf("owbtitle: cannot connect to X Server !\n");
    }
    return display;
}

void Command::setOnTop(Display *display, Window window) {
    
    XClientMessageEvent xclient;
    memset(&xclient, 0, sizeof(xclient));

    // set data
    xclient.type = ClientMessage;
    xclient.window = window;
    xclient.message_type = XInternAtom(display, "_NET_ACTIVE_WINDOW", True);
    xclient.format = 32;
    xclient.data.l[0] = 1;
    xclient.data.l[1] = CurrentTime;
    xclient.data.l[2] = xclient.data.l[3] = xclient.data.l[4] = 0;

    XSetInputFocus(display, window, None, CurrentTime);
    XSendEvent(display, DefaultRootWindow(display), False, SubstructureNotifyMask|SubstructureRedirectMask, (XEvent *)&xclient);
    XFlush(display);
}

void Command::searchTitle(Display *display, Window window, char *title) {

    window = XmuClientWindow(display, window);
    
    XTextProperty prop;
    Status s;

    s = XGetWMName(display, window, &prop);
    if(s) {
        int count = 0, result;
        char **list = NULL;
        result = XmbTextPropertyToTextList(display, &prop, &list, &count);
        if(result == Success) {
            if(count > 0) {
                XWindowAttributes attr;
                XGetWindowAttributes(display, window, &attr);
                if(attr.map_state == 2) { // window is visible
                    if(strcmp(title, list[0]) == 0) {
                        setOnTop(display, window);
                        searching = 0;
                    }
                }
            }
        }
    }
}

void Command::officeFunc(char const *prog, char *filePath) {
    int numOfParams = 5;
    char **params = (char **)malloc(numOfParams * sizeof(char *));
    for(int i = 0; i < numOfParams; i++)
        params[i] = (char *)malloc(1000 * sizeof(char ));
    strcpy(params[0], "/usr/share/playonlinux/playonlinux");
    strcpy(params[1], "--run");
    strcpy(params[2], prog);
    if(filePath != NULL) {
        strcpy(params[3], filePath);
        params[4] = NULL;
    }
    else {
        params[3] = NULL;
    }

    int pid;
    pid = fork();
    if(pid == 0) { // child
        execvp(params[0], params);
        perror("execvp() error!");
        for(int i = 0; i < numOfParams; i++)
            free(params[i]);
        free(params);
        abort();
    }
    else {
        if(!background) { // if & is not set, parent has to wait for the child
            int status;
            do {
                waitpid(pid, &status, WUNTRACED);
            } while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
}