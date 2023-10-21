/**
 * @file myShell.h
 * @brief A library for a shell program
 * @authors Tobias Wondwossen, Jayden Mingle
 * 
 * Details: 
 * - This library provides the necessary data structures and function prototypes for a shell program. It includes the definitions of COMMAND, 
 *   COMMAND_LINE, and SHELL_OPTIONS structures.
 * - Function prototypes for reading and executing command lines, handling signals, changing directories, and managing shell options are provided.
 * 
 * Structures:
 * - COMMAND: Represents a single command with its pathname, arguments (argv), and argument count (argc).
 * - COMMAND_LINE: Represents a command line input by the user. It contains:
 *   - An array of COMMANDs
 *   - Command count
 *   - Pipe count
 *   - Input/output file paths for redirection
 *   - A flag indicating whether the command should run in the background
 * - SHELL_OPTIONS: Contains flags for experimental features.
 * 
 * Function Prototypes:
 * - Reading and initializing command lines
 * - Executing commands with or without pipes
 * - Tokenizing strings
 * - Handling process IDs
 * - Processing input/output redirection tokens
 * - Handling background process tokens
 * - Processing command tokens
 * - Setting command line constants
 * - Printing welcome messages and prompts
 * - Reading user inputs
 * - Generating suggestions / auto complete via tab (experimental)
 * - Adding commands to history (experimental)
 * - Reading command history (experimental)
 * - Handling signals (SIGINT and SIGCHLD)
 * - Changing directories
 * - Managing shell options
 *
 * Assumptions/Limitations: 
 * - This library assumes that the maximum number of arguments per command is 1024, the maximum number of commands per line is 128, and the maximum length 
 *   of an argument is 256 characters.
 * - It also assumes that the read end of a pipe is 0 and the write end is 1.
 * - It does not handle cases where these limits are exceeded.
 * - For commands such as grep where a string is taken as an argument, the string must NOT be surrounded by quotes. For example, grep "hello" is not supported. 
 *   Instead, grep hello is supported.
 *
 * Valid Command: 
 * `command [arg1] [arg2] ... [argn] [< input_file] | command [arg1] [arg2] ... [argn] [> output_file] | ... | command [arg1] [arg2] ... [argn] [&]`
 *
 * & can be at the end of the command line or at the end of a command to run the command in the background.
 *
 * There can be any number of spaces separating the command and its arguments and or Commands and other Commands.
 *
 * This shell does not have exhaustive error handling. In the event of a misused command or invalid input that is one of the many that are not handled, 
 * the shell will re-prompt the user.
 * 
 * @date 2023-10-20
 */

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
int readUserInputExp(char *);
void handleSigChld(int );
void handleSigInt(int );
void changeDir(const char *);
void shellOptions(SHELL_OPTIONS *);

void handleSigInt(int );
void handleSigChld(int );


#endif