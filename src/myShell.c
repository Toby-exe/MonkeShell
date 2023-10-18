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

void readCommandLine(COMMAND_LINE *);
void initCommandLine(COMMAND_LINE *);
int execPipe(COMMAND_LINE, int);
void execFileRedir(COMMAND_LINE *);
char *tokenize(char *, char *);
void cmdHandler(COMMAND_LINE);
int pidStateHandler(int, int);
void execProg(char *, char **);

int showDebug = 0;

int main()
{
    // COMMAND command;
    COMMAND_LINE currentCommandLine;
    printf("Welcome to MonkeShell!\n");

    readCommandLine(&currentCommandLine);

    while (strcmp(currentCommandLine.commands[0].pathname, "exit") != 0)
    {
        cmdHandler(currentCommandLine);

        readCommandLine(&currentCommandLine);
        while (currentCommandLine.commandCount == 0)
        {
            printf("No command entered\n");
            readCommandLine(&currentCommandLine);
        }
    }
    return 0;
}

void cmdHandler(COMMAND_LINE currentCommandLine)
{
    int file;

    int pid;
    int i = 0;
    int in, fd[2];
    int childStatus;
    COMMAND *currCMD;
    int start = 0;
    in = 0;
    int lastCommand = currentCommandLine.commandCount - 1;

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
            // execve(currentCommandLine.commands[0].pathname, currentCommandLine.commands[0].argv, NULL);
            execProg(currentCommandLine.commands[0].pathname, currentCommandLine.commands[0].argv);
        }

        pidStateHandler(pid, currentCommandLine.isBackground);
        // wait for child process to finish

        start = 1;
    }

    if (currentCommandLine.hasPipe == 1)
    {
        in = execPipe(currentCommandLine, start);
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

        // execve(currentCommandLine.commands[lastCommand].pathname, currentCommandLine.commands[lastCommand].argv, NULL);
        execProg(currentCommandLine.commands[lastCommand].pathname, currentCommandLine.commands[lastCommand].argv);
    }

    // wait for last command to finish
    pidStateHandler(pid, currentCommandLine.isBackground);
}

void execProg(char *pathname, char **argv)
{
    //check default paths first by concatenating pathname with each path 
    //if not found then check current directory
    printf("trying to execute: %s\n", pathname);
    char testPath1[100] = "/bin/";
    char testPath2[100] = "/usr/bin/";

    while(execve(testPath1, argv, NULL) == -1)
    {
        printf("trying to execute: %s\n", testPath1);
        strcat(testPath1, pathname);
        printf("trying to execute: %s\n", testPath1);
        if(execve(testPath1, argv, NULL) == -1)
        {
            printf("trying to execute: %s\n", testPath2);
            strcat(testPath2, pathname);
            printf("trying to execute: %s\n", testPath2);
            if(execve(testPath2, argv, NULL) == -1)
            {
                printf("trying to execute: %s\n", pathname);
                if(execve(pathname, argv, NULL) == -1)
                {
                    printf("Command not found\n");
                    return;
                }
            }
        }
    }
}

int pidStateHandler(int pid, int isBackground)
{
    int childStatus;
    if (isBackground == 0)
    {
        waitpid(pid, &childStatus, 0);
    }
    else
    {
        // add child to background process list
        printf("Added child to background process list\n");
        printf("Child PID: %d\n", pid);
    }
    if (WIFEXITED(childStatus))
    {
        printf("Child exited with status: %d\n", WEXITSTATUS(childStatus));
    }
    else if (WIFSIGNALED(childStatus))
    {
        printf("Child exited due to signal: %d\n", WTERMSIG(childStatus));
    }
    else if (WIFSTOPPED(childStatus))
    {
        printf("Child stopped due to signal: %d\n", WSTOPSIG(childStatus));
    }
    else if (WIFCONTINUED(childStatus))
    {
        printf("Child continued\n");
    }
    return childStatus;
}

int execPipe(COMMAND_LINE currentCommandLine, int start)
{
    COMMAND *currCMD;
    int fd[2];
    pid_t pid;
    int in = READ_END;

    for (int i = start; i < currentCommandLine.pipeCount; i++)
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
            // execve(currCMD->pathname, currCMD->argv, NULL);
            execProg(currCMD->pathname, currCMD->argv);
        }

        // close write end of pipe
        close(fd[WRITE_END]);

        // wait for child process to finish
        pidStateHandler(pid, currentCommandLine.isBackground);

        // set input for next command to read end of pipe
        in = fd[READ_END];
    }

    return in;
}

void readCommandLine(COMMAND_LINE *currentCommandLine)
{
    int commandCount = 1;
    int argCount = 0;
    char *token;
    char line[1024];
    int skipToken = 0;

    // init all COMMAND_LINE values sshould be function later
    initCommandLine(currentCommandLine);

    write(1, "Enter a command: ", 18);

    _fgets(line, 1024, 0);
    // check to see if the user entered a command
    if (line[0] == '\n')
    {
        // no command entered
        currentCommandLine->commandCount = 0;
        return;
    }

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
            if (nextToken[strlen(nextToken) - 1] == '&')
            {
                // remove & from token
                nextToken[strlen(nextToken) - 1] = '\0';
                currentCommandLine->isBackground = 1;
            }
            currentCommandLine->outputFile = alloc((strlen(nextToken) + 1) * sizeof(char));
            strcpy(currentCommandLine->outputFile, nextToken);
            skipToken = 1;
        }
        else if (_strncmp(token, "<", 1) == 0)
        {
            // the next token will be the name of the input file
            if (nextToken[strlen(nextToken) - 1] == '&')
            {
                // remove & from token
                nextToken[strlen(nextToken) - 1] = '\0';
                currentCommandLine->isBackground = 1;
            }
            currentCommandLine->inputFile = alloc((strlen(nextToken) + 1) * sizeof(char));
            strcpy(currentCommandLine->inputFile, nextToken);
            skipToken = 1;
        }
        else if (_strncmp(token, "&", 1) == 0)
        {
            // the next token will be the name of the input file
            currentCommandLine->isBackground = 1;
        }
        else
        {
            if (token[strlen(token) - 1] == '&')
            {
                printf("& is attachde to token: %s\n", token);
                // remove & from token
                token[strlen(token) - 1] = '\0';
                printf("token after removing &: %s\n", token);
                currentCommandLine->isBackground = 1;
            }

            printf("skip token ? %s\n", skipToken == 1 ? "yes" : "no");

            if (skipToken == 0)
            {
                COMMAND *currentCommand = &currentCommandLine->commands[commandCount - 1];
                // init all COMMAND values sshould be function later
                currentCommand->argv[argCount] = alloc((strlen(token) + 1) * sizeof(char));
                printf("**** adding token: %s *****\n", token);
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
    for (int i = 0; i < commandCount; i++)
    {
        COMMAND *currentCommand = &currentCommandLine->commands[i];
        currentCommand->pathname = currentCommand->argv[0];
        // null terminate each command
        currentCommand->argv[currentCommand->argc] = NULL;
    }
    // set command count
    currentCommandLine->commandCount = commandCount;

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
    for (int i = 0; i < commandCount; i++)
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
