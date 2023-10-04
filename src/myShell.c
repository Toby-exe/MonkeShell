#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "malloc.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define HEAP_LEN 4096
#define MAX_ARGS 1024

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
    //memset(heap, '\0', 4096);
    heapOffset = 0;
}

void printHeap()
{
    printf("Entire Heap: ");
    for(int i = 0; i < HEAP_LEN; i++)
    {
        if(heap[i] == '\0')
            printf("0");
        else
            printf("%c", heap[i]);
    }
        
    printf("\n");
}
char *_fgets(char *buffer, int size, int fd)
{
    int bytesRead = 0;
    char ch;

    if (size <= 0 || buffer == NULL) {
        return NULL;
    }

    while (bytesRead < size - 1) {
        ssize_t n = read(fd, &ch, 1);

        if (n < 0) {
            perror("read");
            return NULL;
        }

        if (n == 0) {
            // End of file reached
            if (bytesRead == 0) {
                return NULL; // Nothing read
            }
            break;
        }

        buffer[bytesRead++] = ch;

        if (ch == '\n') {
            break; // Stop reading at newline
        }
    }

    buffer[bytesRead] = '\0'; // Null-terminate the string

    return buffer;
}  

typedef struct
{
    char *pathname;
    char *argv[MAX_ARGS];
    int argc;
} COMMAND;

char *getCommand(COMMAND *);
char *tokenize(char *, char *);

int showDebug = 0;

int main()
{
    COMMAND command;
    printf("Welcome to MonkeShell!\n");

    getCommand(&command);
    printHeap();

    while (strcmp(command.pathname, "exit") != 0)
    {
        if (showDebug == 1)
        {
            printf("pathname: %s\n", command.pathname);
            printf("argc: %d\n", command.argc);

            for (int i = 0; i < command.argc; i++)
            {
                printf("argv[%d]: %s\n", i, command.argv[i]);
            }
        }

        pid_t pid = fork();

        if (pid == 0)
        {
            if (showDebug == 1)
            {
                printf("IN CHILD PROC\n");
                printf("Executing in CHILD %s\n", command.pathname);
                printf("argc in CHILD: %d\n", command.argc);
                printf("argv[0] in CHILD: %s\n", command.argv[0]);
            }

            command.argv[command.argc] = NULL;

            /* add path name to */
            if (execvp(command.pathname, command.argv) == -1)
            {
                perror("execvp");
                exit(1);
            }
            exit(0);
        }
        else if (pid > 0)
        {
            // Parent process
            waitpid(pid, NULL, 0);
        }
        else
        {
            // Error
            perror("fork");
            exit(1);
        }
        getCommand(&command);
        printHeap();
    }

    return 0;
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

    //printf("Enter a command: ");
    write(1, "Enter a command: ", 18);
    
    _fgets(line, 1024, 0);
    //read(0, line, 1024); /* max length that a command line can be in bash */
    

    line[strlen(line) - 1] = '\0';
    token = tokenize(line, " ");
    /*allocate space for the argument vector */
    command->argv[0] = alloc((strlen(token) + 1) * sizeof(char));
    //command->argv[0] = token;
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
            //command->argv[command->argc] = token;
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
