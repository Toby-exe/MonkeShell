#ifndef MY_SHELL_H
#define MY_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "malloc.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "utils/str.h"
#include "utils/mem.h"
#include "signal.h"
#include "utils/io.h"
#include "termios.h"
#include <dirent.h>


#define MAX_ARGS 1024
#define MAX_COMMANDS 128
#define MAX_ARG_LENGTH 256

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

typedef struct {
    unsigned int experimentalFeatures;
} SHELL_OPTIONS;

void readCommandLine(COMMAND_LINE *, int);
void initCommandLine(COMMAND_LINE *);
int execPipe(COMMAND_LINE, int);
void execFileRedir(COMMAND_LINE *);
char *tokenize(char *, char *);
void cmdHandler(COMMAND_LINE);
int pidStateHandler(int, int);
int execProg(char *, char **);
void processInputRedirToken(char **nextToken, COMMAND_LINE *currentCommandLine);
void processOutputRedirToken(char **nextToken, COMMAND_LINE *currentCommandLine);
void handleBackgroundProcessToken(char **nextToken, COMMAND_LINE *currentCommandLine);
void processCommandToken(char **token, COMMAND *currentCommand, int *argCount);
void processPipeToken(char **nextToken, COMMAND_LINE *currentCommandLine, int *commandCount, int *argCount);
void setCommandLineConstants(COMMAND_LINE *currentCommandLine, int commandCount);
void printWelcomeMessage();
void printPrompt();
int readUserInput(char *);
char* generateSuggestions(char *);
void addCommandToHistory(char *);
char* readCommandHistory(int );
int readUserInputExp(char *line);

#endif