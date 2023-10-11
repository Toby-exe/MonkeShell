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

char heap[HEAP_LEN];
int heapOffset = 0;

void *alloc(size_t block_sz)
{
    void *ptr = &heap[heapOffset];
    heapOffset += block_sz;
    return ptr;
}

void free_all()
{
    // memset(heap, '\0', 4096);
    heapOffset = 0;
}

void printHeap()
{
    printf("Entire Heap: ");
    for (int i = 0; i < HEAP_LEN; i++)
    {
        if (heap[i] == '\0')
            printf("0");
        else
            printf("%c", heap[i]);
    }

    printf("\n");
}

void readCommandLine(COMMAND_LINE *);
char *getCommand(COMMAND *);
char *tokenize(char *, char *);

int showDebug = 0;

int main()
{
    // COMMAND command;
    COMMAND_LINE *currentCommandLine = malloc(sizeof(COMMAND_LINE));
    printf("Welcome to MonkeShell!\n");

    // getCommand(&command);
    // printHeap();
    readCommandLine(currentCommandLine);
    //printHeap();

    // // if command line has at least one pipe run pipe logic else run normal logic
    // while(currentCommandLine->commands[0]->argv[0] != "exit")
    // {
    //     if(currentCommandLine->pipeCount > 0)
    //     {
    //         // run pipe logic
    //     }
    //     else
    //     {
    //         // run normal logic
    //     }
    // }

    return 0;
}

void readCommandLine(COMMAND_LINE *currentComandLine)
{
    int commandCount = 0;
    int argCount = 0;
    char *token;
    char line[1024];

    write(1, "Enter a command: ", 18);

    _fgets(line, 1024, 0);

    line[strlen(line) - 1] = '\0';
    token = tokenize(line, " ");

    currentComandLine->commands[commandCount] = malloc(sizeof(COMMAND));

    currentComandLine->commands[commandCount]->argv[argCount] = malloc((strlen(token) + 1) * sizeof(char));

    strcpy(currentComandLine->commands[commandCount]->argv[argCount], token);

    while (token != NULL)
    {

        token = tokenize(NULL, " ");

        if (token != NULL)
        {
            if (strcmp(token, "|") == 0)
            {
                commandCount++;
                argCount = 0;
                printf("Encountered a pipe\n");
                currentComandLine->commands[commandCount] = malloc(sizeof(COMMAND));
            }
            else
            {
                argCount++;
                currentComandLine->commands[commandCount]->argv[argCount] = malloc((strlen(token) + 1) * sizeof(char));
                strcpy(currentComandLine->commands[commandCount]->argv[argCount], token);
            }
        }
    }

    
}

/**
 * Reads a line of input from stdin and tokenizes it into an array of arguments.
 *
 * @param command A pointer to a COMMAND struct to store the parsed command.
 * @return Returns 0 on success.
 */
char *getCommand(COMMAND *command)
{
    char *token;
    char line[1024];

    // printf("Enter a command: ");
    write(1, "Enter a command: ", 18);

    _fgets(line, 1024, 0);
    // read(0, line, 1024); /* max length that a command line can be in bash */

    line[strlen(line) - 1] = '\0';
    token = tokenize(line, " ");
    /*allocate space for the argument vector */
    command->argv[0] = alloc((strlen(token) + 1) * sizeof(char));
    // command->argv[0] = token;
    strcpy(command->argv[0], token);
    /* print out the alocated memory */
    if (1)
    {
        printf("Allocated memory for argv[0]: %s\n", command->argv[0]);
    }
    command->argc = 1;

    while (token != NULL)
    {
        token = tokenize(NULL, " ");
        if (token != NULL)
        {
            command->argv[command->argc] = alloc((strlen(token) + 1) * sizeof(char));
            // command->argv[command->argc] = token;
            strcpy(command->argv[command->argc], token);
            command->argc += 1;
        }
    }

    command->pathname = command->argv[0];

    /* myFree(line);  ^^^^ ????*/
    return 0;
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
