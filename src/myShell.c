#include <stdio.h>
#include "string.h"

char *getCommand(char* );

char getPi();

int main () {

    typedef enum { Running, Terminate } shellState;
    char cmd[60];

    shellState myShellState = Running;

    while(myShellState == Running){
        printf("monkeShell$ ");
        scanf("%s", cmd);
        if(strcmp(getCommand(cmd), "exit") == 0){
            myShellState = Terminate;
        }
    } 

    return 0;
}

char *getCommand(char *cmd){
    char argv[100];

    printf("Command Entered: %s\n", cmd);

    return cmd;
}