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
#include "signal.h"
#include "utils/io.h"

extern char **environ;
int shellPid;
char prevPath[256];
char prompt[256];


void handleSigChld(int snum)
{
    pid_t pid;
    int status;

    pid = wait(&status);
    if (pid == -1)
    {
        // foreground jobs will have been waitpid'd by the time they terminate
        // perror("wait");
    }
    else
    {
        printf("parent: child process pid=%d exited with status %d\n", (int)pid, WEXITSTATUS(status));
    }

    // removeJob(pid);
    signal(SIGCHLD, handleSigChld); /* reset the signal, recall handleSigChld*/
}

void handleSigInt(int snum)
{
    pid_t pid = getpid();
    if (pid != shellPid)
    {
        c_fputs("forcefully terminating the process with ctrl+C\n", 1);
        kill(pid, SIGKILL);
        signal(SIGINT, handleSigInt);
    }
    else
    {
        c_fputs("\nUse 'exit' to terminate MonkeShell\n", 1);
        // signal(SIGINT, SIG_DFL);
        // kill(pid, SIGINT);
    }
}

void cd(const char *dir)
{
    char buf[256];
    // _fputs(dir, 1);
    // _fputs("\n", 1);

    // cd without any arguments: changes the working directory to the home directory
    if (dir == NULL || c_strcmp(dir, "") == 0)
    {
        // save the current directory before changing it
        c_fputs("in empty case\n", 1);
        c_strcpy(prevPath, getcwd(buf, 256));
        chdir(getenv("HOME"));
        // chdir(getHomeDir());
    }
    // cd -: change to previous working directory
    else if (strncmp(dir, "-", 1) == 0)
    {
        c_fputs("in prev case\n", 1);
        char temp[256];
        c_strcpy(temp, getcwd(buf, 256)); // save current directory to temp
        chdir(prevPath);
        c_fputs(getcwd(buf, 256), 1);
        c_fputs("\n", 1);
        c_strcpy(prevPath, temp); // update prevPath with temp
    }
    else
    {
        c_fputs("in normal case\n", 1);
        //~ = home directory
        char new_dir[1024];
        if (dir[0] == '~')
        {
            // sprintf(new_dir, "%s%s", getenv("HOME"), dir + 1);
            // use c_strcat instead
            c_strcpy(new_dir, getenv("HOME"));
            c_strcat(new_dir, dir + 1);

            dir = new_dir;
        }

        // cd <pathname>: change to specified directory (includes "." and "../")
        c_strcpy(prevPath, getcwd(buf, 256));
        if (chdir(dir) != 0)
        {
            printf("MonkeShell: cd: %s: No such file or directory\n", dir);
            return;
        }
    }

    printf("changed directory\n");
}

void printError(char *msg)
{
    c_write(msg, 1, RED);
}

int main()
{
    shellPid = getpid();
    signal(SIGINT, handleSigInt);
    signal(SIGCHLD, handleSigChld); /* detect child termination */
    COMMAND_LINE currentCommandLine;

    printWelcomeMessage();

    while (1)
    {
        readCommandLine(&currentCommandLine);

        while (currentCommandLine.commandCount == 0)
        {
            readCommandLine(&currentCommandLine);
        }

        // Check for 'exit' immediately after reading the command this should be part of a function that checks for built in commands
        if (c_strcmp(currentCommandLine.commands[0].pathname, "exit") == 0)
        {
            break;
        }

        if (c_strcmp(currentCommandLine.commands[0].pathname, "cd") == 0)
        {
            cd(currentCommandLine.commands[0].argv[1]);
            continue;
        }

        cmdHandler(currentCommandLine);
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
    // last command

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
            if (execProg(currentCommandLine.commands[0].pathname, currentCommandLine.commands[0].argv) == -1)
            {
                _exit(0);
            }
        }

        pidStateHandler(pid, currentCommandLine.isBackground);
        // wait for child process to finish

        start = 1;
    }

    if (currentCommandLine.hasPipe == 1)
    {
        in = execPipe(currentCommandLine, start);
        if (in == -1)
        {
            return;
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

        currCMD = &currentCommandLine.commands[lastCommand];
        if (execProg(currCMD->pathname, currCMD->argv) == -1)
        {
            // error
            // make sure to exit child process
            _exit(0);
        }
    }

    // wait for last command to finish
    pidStateHandler(pid, currentCommandLine.isBackground);
}

int execProg(char *pathname, char **argv)
{
    // printf("trying to execute: %s\n", pathname);
    char testPath1[100] = "/bin/";
    char testPath2[100] = "/usr/bin/";
    
    while (execve(testPath1, argv, environ) == -1)
    {
        // printf("trying to execute: %s\n", testPath1);
        c_strcat(testPath1, pathname);
        // printf("trying to execute: %s\n", testPath1);
        if (execve(testPath1, argv, environ) == -1)
        {
            // printf("trying to execute: %s\n", testPath2);
            c_strcat(testPath2, pathname);
            // printf("trying to execute: %s\n", testPath2);
            if (execve(testPath2, argv, environ) == -1)
            {
                // printf("trying to execute: %s\n", pathname);
                if (execve(pathname, argv, environ) == -1)
                {
                    c_write("Command '", 1, RED);
                    c_write(pathname, 1, RED);
                    c_write("' not found\n", 1, RED);
                    return -1;
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
            if (execProg(currCMD->pathname, currCMD->argv) == -1)
            {
                _exit(0);
            }
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
    char *token = alloc(MAX_ARG_LENGTH * sizeof(char));
    char *nextToken = alloc(MAX_ARG_LENGTH * sizeof(char));
    char line[1024];
    int skipToken = 0;

    free_all();
    // init all COMMAND_LINE values sshould be function later
    initCommandLine(currentCommandLine);
    
    printPrompt();
    // c_fgets(line, MAX_ARGS, 0);
    if(readUserInput(line) == -1) {
        currentCommandLine->commandCount = 0;
        return;
    }

    token = tokenize(line, " ");

    while (token != NULL)
    {
        nextToken = tokenize(NULL, " ");
        if (c_strncmp(token, "|", 1) == 0)
        {
            processPipeToken(&nextToken, currentCommandLine, &commandCount, &argCount);
        }
        else if (c_strncmp(token, ">", 1) == 0)
        {
            processOutputRedirToken(&nextToken, currentCommandLine);
            skipToken = 1;
        }
        else if (c_strncmp(token, "<", 1) == 0)
        {
            processInputRedirToken(&nextToken, currentCommandLine);
            skipToken = 1;
        }
        else if (c_strncmp(token, "&", 1) == 0)
        {
            currentCommandLine->isBackground = 1;
        }
        else if (skipToken == 0)
        {
            handleBackgroundProcessToken(&token, currentCommandLine);
            COMMAND *currentCommand = &currentCommandLine->commands[commandCount - 1];
            processCommandToken(&token, currentCommand, &argCount);
        }
        token = nextToken;
    }

    setCommandLineConstants(currentCommandLine, commandCount);
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

void handleBackgroundProcessToken(char **nextToken, COMMAND_LINE *currentCommandLine)
{
    if ((*nextToken)[c_strlen(*nextToken) - 1] == '&')
    {
        // remove & from token
        (*nextToken)[c_strlen(*nextToken) - 1] = '\0';
        currentCommandLine->isBackground = 1;
    }
}

void processCommandToken(char **token, COMMAND *currentCommand, int *argCount)
{
    currentCommand->argv[*argCount] = alloc((c_strlen(*token) + 1) * sizeof(char));
    c_strcpy(currentCommand->argv[*argCount], *token);

    // if token is ls add --color=auto
    if (c_strcmp(*token, "ls") == 0)
    {
        currentCommand->argv[*argCount + 1] = alloc((c_strlen("--color=auto") + 1) * sizeof(char));
        c_strcpy(currentCommand->argv[*argCount + 1], "--color=auto");
        (*argCount)++;
    }

    (*argCount)++;
    currentCommand->argc = *argCount;
}

void processOutputRedirToken(char **nextToken, COMMAND_LINE *currentCommandLine)
{
    handleBackgroundProcessToken(nextToken, currentCommandLine);
    currentCommandLine->outputFile = alloc((c_strlen(*nextToken) + 1) * sizeof(char));
    c_strcpy(currentCommandLine->outputFile, *nextToken);
}

void processInputRedirToken(char **nextToken, COMMAND_LINE *currentCommandLine)
{
    handleBackgroundProcessToken(nextToken, currentCommandLine);
    currentCommandLine->inputFile = alloc((c_strlen(*nextToken) + 1) * sizeof(char));
    c_strcpy(currentCommandLine->inputFile, *nextToken);
}

void processPipeToken(char **nextToken, COMMAND_LINE *currentCommandLine, int *commandCount, int *argCount)
{
    currentCommandLine->pipeCount++;
    currentCommandLine->hasPipe = 1;
    if (*nextToken != NULL)
    {
        (*commandCount)++;
        *argCount = 0;
    }
}

void setCommandLineConstants(COMMAND_LINE *currentCommandLine, int commandCount)
{
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
    input += c_strspn(input, delm);

    /* if we are at the end of the string, return NULL */
    if (*input == '\0')
    {
        return input = NULL;
    }
    else
    {
        /* find the end of the token */
        token = input;
        input += c_strcspn(input, delm);

        /* if not at the end of the string, terminate the token and set input
        to the next character */
        if (*input != '\0')
            *input++ = '\0';

        return token;
    }
}

int readUserInput(char *line)
{
    c_fgets(line, MAX_ARGS, 0);

    // check to see if the user entered a command
    if (line[0] == '\n')
    {
        // no command entered
        return -1;
    }

    line[c_strlen(line) - 1] = '\0';
}

/**
 * This function prints a welcome message to the user.
 * It displays the MonkeShell logo in ASCII art and a greeting message.
 *
 * @return void
 */
void printWelcomeMessage()
{
    c_write("______  ___               ______        \n", 1, YELLOW);
    c_write("___   |/  /______ _______ ___  /_______ \n", 1, YELLOW);
    c_write("__  /|_/ / _  __ \\__  __ \\__  //_/_  _ \\\n", 1, YELLOW);
    c_write("_  /  / /  / /_/ /_  / / /_  ,<   /  __/\n", 1, YELLOW);
    c_write("/_/  /_/   \\____/ /_/ /_/ /_/|_|  \\___/ \n", 1, YELLOW);
    c_write("_____________  ______________________ \n", 1, PURPLE);
    c_write("__  ___/__  / / /__  ____/__  /___  / \n", 1, PURPLE);
    c_write("_____ \\__  /_/ /__  __/  __  / __  /  \n", 1, PURPLE);
    c_write("____/ /_  __  / _  /___  _  /___  /___\n", 1, PURPLE);
    c_write("/____/ /_/ /_/  /_____/  /_____/_____/\n", 1, PURPLE);
    c_write("                                         \n", 1, CYAN);

    printf("Welcome to MonkeShell!\n");
}

void printPrompt()
{
    c_write(getcwd(prompt, 256), 1, CYAN);
    c_write("$ ", 1, PURPLE);
}