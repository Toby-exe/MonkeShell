#ifndef MY_SHELL_H
#define MY_SHELL_H

#define MAX_ARGS 1024
#define MAX_COMMANDS 128

#define READ_END  0
#define WRITE_END 1

typedef struct
{
    char *pathname;
    char *argv[MAX_ARGS];
    int argc;
} COMMAND;


typedef struct {
    COMMAND commands[MAX_COMMANDS];
    int commandCount;
    int hasPipe;
    int pipeCount;
    char *inputFile;
    char *outputFile;
    int isBackground;
} COMMAND_LINE;

typedef struct process {
    pid_t pid;
    char *name;
    struct process *next;
} PROCESS;
#endif