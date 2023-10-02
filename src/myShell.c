#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "malloc.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 512

typedef struct
{
    char *pathname;
    char *argv[MAX_ARGS];
    int argc;
} COMMAND;

char *getCommand(COMMAND *);
char *tokenize(char *, char *);

int main()
{

    COMMAND command;
    printf("Welcome to MonkeShell!\n");

    getCommand(&command);

    while (strcmp(command.pathname, "exit") != 0)
    {
        printf("pathname: %s\n", command.pathname);
        printf("argc: %d\n", command.argc);

        for (int i = 0; i < command.argc; i++)
        {
            printf("argv[%d]: %s\n", i, command.argv[i]);
        }

        pid_t pid = fork();

        if (pid == 0)
        {
            printf("IN CHILD PROC\n");
            printf("Executing in CHILD %s\n", command.pathname);
            printf("argc in CHILD: %d\n", command.argc);
            printf("argv[0] in CHILD: %s\n", command.argv[0]);

            command.argv[command.argc] = NULL;
            
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

    printf("Enter a command: ");
    fgets(line, 1024, stdin); 

    line[strlen(line) - 1] = '\0';
    
    token = tokenize(line, " ");
    command->argv[0] = token;
    command->argc = 1;

    while (token != NULL)
    {
        token = tokenize(NULL, " ");
        if (token != NULL)
        {
            command->argv[command->argc] = token;
            command->argc += 1;
        }
    }

    command->pathname = command->argv[0];

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
