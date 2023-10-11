#ifndef MY_SHELL_H
#define MY_SHELL_H

#define HEAP_LEN 12000
#define MAX_ARGS 1024
#define MAX_COMMANDS 128

typedef struct
{
    char *pathname;
    char *argv[MAX_ARGS];
    int argc;
} COMMAND;


typedef struct {
    COMMAND *commands[MAX_COMMANDS];
    int commandCount;
    int hasInputRedirection;
    int hasOutputRedirection;
    int pipeCount;
    int hasBackground;
} COMMAND_LINE;


#endif