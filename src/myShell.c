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

// char heap[HEAP_LEN];
// int heapOffset = 0;

// void *alloc(size_t block_sz)
// {
//     void *ptr = &heap[heapOffset];
//     heapOffset += block_sz;
//     return ptr;
// }

// void free_all()
// {
//     // memset(heap, '\0', 4096);
//     heapOffset = 0;
// }

// void printHeap()
// {
//     printf("Entire Heap: ");
//     for (int i = 0; i < HEAP_LEN; i++)
//     {
//         if (heap[i] == '\0')
//             printf("0");
//         else
//             printf("%c", heap[i]);
//     }

//     printf("\n");
// }

void readCommandLine(COMMAND_LINE *);
void initCommandLine(COMMAND_LINE *);
char *tokenize(char *, char *);

int showDebug = 0;

int main()
{
    // COMMAND command;
    COMMAND_LINE currentCommandLine;
    printf("Welcome to MonkeShell!\n");

    readCommandLine(&currentCommandLine);
    // printHeap();

    while (strcmp(currentCommandLine.commands[0].pathname, "exit") != 0)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            // Child process
            currentCommandLine.commands[0].argv[currentCommandLine.commands[0].argc] = NULL;

            /* add path name to */
            if (execvp(currentCommandLine.commands[0].pathname, currentCommandLine.commands[0].argv) == -1)
            {
                printf("Error: Command not found\n");
                // exit(1);
            }
            // exit(0);
        }

        if (currentCommandLine.isBackground == 0)
        {
            // we are in the parent process
            waitpid(pid, NULL, 0);
        }
        else
        {
            // we are in the parent process
            printf("Background process started with pid: %d\n", pid);
        }

        readCommandLine(&currentCommandLine);
    }

    return 0;
}

void readCommandLine(COMMAND_LINE *currentCommandLine)
{
    int commandCount = 0;
    int argCount = 0;
    char *token;
    char line[1024];

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
            if (nextToken != NULL)
            {
                commandCount++;
                argCount = 0;
            }
        }
        else if (_strncmp(token, ">", 1) == 0)
        {
            // set output file
        }
        else if (_strncmp(token, "<", 1) == 0)
        {
            // set input file
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

            COMMAND *currentCommand = &currentCommandLine->commands[commandCount];
            // init all COMMAND values sshould be function later
            currentCommand->argv[argCount] = alloc((strlen(token) + 1) * sizeof(char));
            strcpy(currentCommand->argv[argCount], token);
            // printf("Allocated memory for argv[%d]: %s\n", argCount, currentCommand->argv[argCount]);
            argCount++;
            currentCommand->argc = argCount;

            token = nextToken;
        }

        // set the namess of each command pathname
        for (int i = 0; i <= commandCount; i++)
        {
            COMMAND *currentCommand = &currentCommandLine->commands[i];
            currentCommand->pathname = currentCommand->argv[0];
        }

        printf("\nCommand Count: %d\n", commandCount);
        printf("Pipe Count: %d\n", currentCommandLine->pipeCount);
        printf("is background: %d\n\n", currentCommandLine->isBackground);

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
}

void initCommandLine(COMMAND_LINE *currentCommandLine) {
    currentCommandLine->hasPipe = 0;
    currentCommandLine->pipeCount = 0;
    currentCommandLine->commandCount = 0;
    currentCommandLine->isBackground = 0;
    currentCommandLine->hasInputRedirection = 0;
    currentCommandLine->hasOutputRedirection = 0;
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
