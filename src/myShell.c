#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "malloc.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "utils/str.h"
#include "myShell.h"
#include "utils/mem.h"

extern char **environ;

void runCommand(COMMAND *, int);
void readCommandLine(COMMAND_LINE *);
void initCommandLine(COMMAND_LINE *);
void execPipe(COMMAND_LINE , int);
void execFileRedir(COMMAND_LINE *);
char *tokenize(char *, char *);
void cmdHandler(COMMAND_LINE);

int showDebug = 0;

int main()
{
    // COMMAND command;
    COMMAND_LINE currentCommandLine;
    int file;
    printf("Welcome to MonkeShell!\n");

    readCommandLine(&currentCommandLine);

    while (strcmp(currentCommandLine.commands[0].pathname, "exit") != 0)
    {

        // cmdHandler(currentCommandLine);
        int pid;
        int i = 0;
        int in, fd[2];
        int childStatus;
        COMMAND *currCMD;
        int start = 0;
        in = 0;
        int lastCommand = currentCommandLine.commandCount;

        if (currentCommandLine.inputFile != NULL)
        {
            // run input redirection command
            pid = fork();

            if (pid == 0)
            {
                // child process
                file = open(currentCommandLine.inputFile, O_RDONLY);
                dup2(file, STDIN_FILENO);
                close(file);

                // execute command
                execvp(currentCommandLine.commands[0].pathname, currentCommandLine.commands[0].argv);
            }

            // wait for child process to finish
            if (currentCommandLine.isBackground == 0)
            {
                waitpid(pid, NULL, 0);
            }
            start = 1;
        }

        if (currentCommandLine.hasPipe == 1)
        {

            // execPipe(currentCommandLine, start);

            for (i = start; i < currentCommandLine.pipeCount; i++)
            {
                currCMD = &currentCommandLine.commands[i];

                pipe(fd);

                pid = fork();

                if (pid == 0)
                {
                    // child process
                    if (in != READ_END)
                    {
                        // redirect input
                        dup2(in, 0);
                        close(in);
                    }

                    if (fd[WRITE_END] != 1)
                    {
                        // redirect output
                        dup2(fd[WRITE_END], 1);
                        close(fd[WRITE_END]);
                    }

                    // execute command
                    execvp(currCMD->pathname, currCMD->argv);
                }

                // close write end of pipe
                close(fd[WRITE_END]);

                // wait for child process to finish
                if (currentCommandLine.isBackground == 0)
                {
                    waitpid(pid, NULL, 0);
                }

                // set input for next command to read end of pipe
                in = fd[READ_END];
            }
        }

        // execute last command
        pid = fork();

        if (pid == 0)
        {
            if (currentCommandLine.hasPipe == 1)
            {
                // last command
                if (in != 0)
                {
                    // redirect input
                    dup2(in, 0);
                    close(in);
                }
            }

            if (currentCommandLine.outputFile != NULL)
            {
                // redirect output
                file = open(currentCommandLine.outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(file, STDOUT_FILENO);
                close(file);
            }

            execvp(currentCommandLine.commands[i].pathname, currentCommandLine.commands[i].argv);
        }

        // wait for last command to finish
        if (currentCommandLine.isBackground == 0)
        {
            waitpid(pid, NULL, 0);
        }

        readCommandLine(&currentCommandLine);
    }

    return 0;
}

void cmdHandler(COMMAND_LINE cmdLine)
{
}

void execPipe(COMMAND_LINE currentCommandLine, int start)
{
    int pid;
    int fd[2];
    int childStatus;
    COMMAND *currCMD;

    int lastCommand = currentCommandLine.commandCount;
    printf("last command: %d\n", lastCommand);
}

void readCommandLine(COMMAND_LINE *currentCommandLine)
{
    int commandCount = 0;
    int argCount = 0;
    char *token;
    char line[1024];
    int skipToken = 0;

    // init all COMMAND_LINE values sshould be function later
    initCommandLine(currentCommandLine);

    write(1, "Enter a command: ", 18);

    _fgets(line, 1024, 0);

    line[strlen(line) - 1] = '\0';
    token = tokenize(line, " ");

    while (token != NULL)
    {
        char *nextToken = tokenize(NULL, " ");
        if (_strncmp(token, "|", 1) == 0)
        {
            currentCommandLine->pipeCount++;
            currentCommandLine->hasPipe = 1;
            if (nextToken != NULL)
            {
                commandCount++;
                argCount = 0;
            }
        }
        else if (_strncmp(token, ">", 1) == 0)
        {
            // the next token will be the name of the output file
            currentCommandLine->outputFile = alloc((strlen(nextToken) + 1) * sizeof(char));
            strcpy(currentCommandLine->outputFile, nextToken);
            skipToken = 1;
        }
        else if (_strncmp(token, "<", 1) == 0)
        {
            // the next token will be the name of the input file
            currentCommandLine->inputFile = alloc((strlen(nextToken) + 1) * sizeof(char));
            strcpy(currentCommandLine->inputFile, nextToken);
            skipToken = 1;
        }
        else
        {
            if (token[strlen(token) - 1] == '&')
            {
                // If & is at the end of the token, remove it and set background flag
                token[strlen(token) - 1] = '\0';
                currentCommandLine->isBackground = 1;
            }
            else if (_strncmp(token, "&", 1) == 0 && nextToken == NULL)
            {
                // If & is a new token and it's the last one, set background flag
                currentCommandLine->isBackground = 1;
            }

            if (skipToken == 0)
            {

                COMMAND *currentCommand = &currentCommandLine->commands[commandCount];
                // init all COMMAND values sshould be function later
                currentCommand->argv[argCount] = alloc((strlen(token) + 1) * sizeof(char));
                strcpy(currentCommand->argv[argCount], token);
                // printf("Allocated memory for argv[%d]: %s\n", argCount, currentCommand->argv[argCount]);
                argCount++;
                currentCommand->argc = argCount;
            }
            skipToken = 0;
        }
        token = nextToken;
    }

    // set pathname for each command
    for (int i = 0; i <= commandCount; i++)
    {
        COMMAND *currentCommand = &currentCommandLine->commands[i];
        currentCommand->pathname = currentCommand->argv[0];
        //null terminate each command
        currentCommand->argv[currentCommand->argc] = NULL;
    }

    printf("\nCommand Count: %d\n", commandCount);
    printf("Pipe Count: %d\n", currentCommandLine->pipeCount);
    printf("is background: %d\n\n", currentCommandLine->isBackground);
    if (currentCommandLine->inputFile != NULL)
    {
        printf("Input File: %s\n", currentCommandLine->inputFile);
    }
    if (currentCommandLine->outputFile != NULL)
    {
        printf("Output File: %s\n", currentCommandLine->outputFile);
    }

    // formate nicley and print out each command, its pathname, and its args give it some indenting
    for (int i = 0; i <= commandCount; i++)
    {
        COMMAND *currentCommand = &currentCommandLine->commands[i];
        printf("Command %d: %s\n", i, currentCommand->pathname);
        printf("Arg Count: %d\n", currentCommand->argc);
        for (int j = 0; j < currentCommand->argc; j++)
        {
            printf("Arg %d: %s\n", j, currentCommand->argv[j]);
        }
    }
}

void initCommandLine(COMMAND_LINE *currentCommandLine)
{
    currentCommandLine->hasPipe = 0;
    currentCommandLine->pipeCount = 0;
    currentCommandLine->commandCount = 0;
    currentCommandLine->isBackground = 0;
    currentCommandLine->inputFile = NULL;
    currentCommandLine->outputFile = NULL;
    currentCommandLine->commands[0].pathname = NULL;
    currentCommandLine->commands[0].argc = 0;
    for (int i = 0; i < 10; i++)
    {
        currentCommandLine->commands[0].argv[i] = NULL;
    }
}

/**
 * Tokenizes a string based on a given delimiter.
 *
 * @param s The string to be tokenized.
 * @param delm The delimiter to be used for tokenization.
 * @return A pointer to the next token in the string, or NULL if there are no more tokens.
 */
char *tokenize(char *src, char *delm)
{
    static char *input = NULL;
    char *token;

    /* if s is not NULL, then we are tokenizing a new string */
    if (src != NULL)
        input = src;

    /* check to see if we are at the end of the string */
    if (input == NULL)
        return NULL;

    /* skip leading delimiters */
    input += strspn(input, delm);

    /* if we are at the end of the string, return NULL */
    if (*input == '\0')
    {
        return input = NULL;
    }
    else
    {
        /* find the end of the token */
        token = input;
        input += strcspn(input, delm);

        /* if not at the end of the string, terminate the token and set input
        to the next character */
        if (*input != '\0')
            *input++ = '\0';

        return token;
    }
}
