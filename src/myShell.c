/**
 * @file myShell.c
 * @authors Tobias Wondwossen, Jayden Mingle
 * 
 * @date 2023-10-20 
 */
#include "myShell.h"

extern char **environ;
char prevPath[256];
char prompt[256];
extern int shellPid;

const char potentialCommands[100][50] = {"ls", "cd", "pwd", "mkdir", "rmdir", "rm", "cat", "cp", "mv", "chmod", "chown", "chgrp", "ln", "touch", "grep", "wc", "sort", "whoami", "tty", "uname", "date", "cal", "bc", "clear", "exit"};


/**
 * @brief Handles command line input and executes commands accordingly
 *
 * Details: This function takes a command line structure as input, which contains information
 * about the commands to be executed, their arguments, and any associated input/output redirection
 * or piping. It then forks child processes to execute each command in sequence, handling any necessary
 * redirection or piping. If a command is meant to be run in the background, it does not wait for the
 * child process to finish before returning.
 *
 * @param[in] currentCommandLine A structure containing the parsed command line input
 */
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
        if (currentCommandLine.commandCount == 1)
        {
            return;
        }
        else
        {
            start = 1;
        }
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

/**
 * Executes a program with the given pathname and arguments.
 * If the program is not found in /bin or /usr/bin, it tries to execute it with the given pathname.
 * If the program is not found, it prints an error message and returns -1.
 *
 * @param pathname The pathname of the program to execute.
 * @param argv An array of arguments to pass to the program.
 * @return 0 if the program is executed successfully, -1 otherwise.
 */
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
                    c_write("Command '", STDOUT_FILENO, RED);
                    c_write(pathname, 1, RED);
                    c_write("' not found\n", STDOUT_FILENO, RED);
                    return -1;
                }
            }
        }
    }

    return 0;
}

/**
 * @brief Handles the state of a child process based on whether it should run in the background or not
 *
 * This function takes a process ID (pid) and a flag indicating whether the process should run in the background.
 * If the process is not meant to run in the background, it waits for the child process to finish before returning.
 * If the process is meant to run in the background, it adds the child to a list of background processes and immediately returns.
 *
 * @param[in] pid The process ID of the child process
 * @param[in] isBackground A flag indicating whether the process should run in the background (non-zero) or not (zero)
 * @return The status of the child process
 */
int pidStateHandler(int pid, int isBackground)
{
    int childStatus;
    if (isBackground == 0)
    {
        waitpid(pid, &childStatus, 0);
    }
    else
    {
        c_write("Added child to background process list\n", STDOUT_FILENO, NULL);
        c_write("Child PID: ", STDOUT_FILENO, NULL);
        c_write(intToString(pid), STDOUT_FILENO, GREEN);
        c_write("\n", STDOUT_FILENO, GREEN);
    }
    return childStatus;
}

/**
 * @brief Executes a pipeline of commands
 *
 * Details: This function takes a command line structure and a start index as input. It then creates a pipeline of commands
 * starting from the command at the start index. Each command in the pipeline is executed in a new child process.
 * The output of each command is piped to the input of the next command in the pipeline. If a command is meant to
 * be run in the background, it does not wait for the child process to finish before moving on to the next command.
 *
 * @param[in] currentCommandLine A structure containing the parsed command line input
 * @param[in] start The index of the first command in the pipeline
 * @return The file descriptor for reading from the pipe connected to the output of the last command in the pipeline
 */
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

/**
 * @brief Reads a command line input and parses it into a COMMAND_LINE structure
 *
 * Details: This function takes a pointer to a COMMAND_LINE structure and an experimentalFeatures
 * flag as input. It reads a line of command line input from the user, tokenizes it, and parses it
 * into the COMMAND_LINE structure. The structure contains information about the commands to be
 * executed, their arguments, and any associated input/output redirection or piping. If the experimental
 * Features flag is set, it uses an experimental method to read user input - which allows for tabed auto
 * complete (basic functionality) and command history (basic and very bugy functionality).
 *
 * @param[in,out] currentCommandLine A pointer to a COMMAND_LINE structure where the parsed command line input will be stored
 * @param[in] experimentalFeatures A flag indicating whether to use an experimental method to read user input (non-zero) or not (zero)
 */
void readCommandLine(COMMAND_LINE *currentCommandLine, int experimentalFeatures)
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

    if (experimentalFeatures == 1)
    {
        if (readUserInputExp(line) == -1)
        {
            currentCommandLine->commandCount = 0;
            return;
        }
    }
    else
    {
        if (readUserInput(line) == -1)
        {
            currentCommandLine->commandCount = 0;
            return;
        }
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

/**
 * @brief Initializes a COMMAND_LINE structure
 *
 * @param[in,out] currentCommandLine A pointer to a COMMAND_LINE structure to be initialized
 */
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
    for (int i = 0; i < MAX_ARGS; i++)
    {
        currentCommandLine->commands[0].argv[i] = NULL;
    }
}

/**
 * @brief Checks for a background process token and sets a flag if found
 *
 * Details: This function takes a pointer to a token and a pointer to a COMMAND_LINE structure as input.
 * If the last character of the token is '&', it removes the '&' from the token and sets the isBackground
 * field of the COMMAND_LINE structure to 1.
 *
 * @param[in,out] nextToken A pointer to the token to be checked
 * @param[in,out] currentCommandLine A pointer to a COMMAND_LINE structure where the isBackground field will be set if necessary
 */
void handleBackgroundProcessToken(char **nextToken, COMMAND_LINE *currentCommandLine)
{
    if ((*nextToken)[c_strlen(*nextToken) - 1] == '&')
    {
        // remove & from token
        (*nextToken)[c_strlen(*nextToken) - 1] = '\0';
        currentCommandLine->isBackground = 1;
    }
}

/**
 * @brief Processes a command token and stores it in a COMMAND structure
 *
 * Details: This function takes a pointer to a token, a pointer to a COMMAND structure, and a pointer to an argument count as input.
 * It copies the token into the argv array of the COMMAND structure and increments the argument count.
 * If the token is 'ls', it also adds '--color=auto' to the argv array and increments the argument count again.
 * Finally, it sets the argc field of the COMMAND structure to the argument count.
 *
 * @param[in,out] token A pointer to the token to be processed
 * @param[in,out] currentCommand A pointer to a COMMAND structure where the processed token will be stored
 * @param[in,out] argCount A pointer to an integer that keeps track of the number of arguments in the command
 */
void processCommandToken(char **token, COMMAND *currentCommand, int *argCount)
{
    currentCommand->argv[*argCount] = alloc((c_strlen(*token) + 1) * sizeof(char));
    c_strcpy(currentCommand->argv[*argCount], *token);

    // if token is ls add --color=autor
    if (c_strcmp(*token, "ls") == 0 || c_strcmp(*token, "grep") == 0)
    {
        currentCommand->argv[*argCount + 1] = alloc((c_strlen("--color=auto") + 1) * sizeof(char));
        c_strcpy(currentCommand->argv[*argCount + 1], "--color=auto");
        (*argCount)++;
    }

    (*argCount)++;
    currentCommand->argc = *argCount;
}

/**
 * @brief Processes the output redirection token in a command line.
 *
 * Details: This function handles the output redirection token in a command line. It first checks if the command
 * is meant to be run in the background, and then allocates memory for and copies the output file name from the next token.
 *
 * @param[in,out] nextToken A pointer to the next token in the command line. This function will update it to point to the output file name.
 * @param[out] currentCommandLine A pointer to the current command line structure. This function will update its 'outputFile' field with the output file name.
 */
void processOutputRedirToken(char **nextToken, COMMAND_LINE *currentCommandLine)
{
    handleBackgroundProcessToken(nextToken, currentCommandLine);
    currentCommandLine->outputFile = alloc((c_strlen(*nextToken) + 1) * sizeof(char));
    c_strcpy(currentCommandLine->outputFile, *nextToken);
}

/**
 * @brief Processes the input redirection token in a command line.
 *
 * Details: This function handles the input redirection token in a command line. It first checks if the command is meant to
 * be run in the background, and then allocates memory for and copies the input file name from the next token.
 *
 * @param[in,out] nextToken A pointer to the next token in the command line. This function will update it to point to the input file name.
 * @param[out] currentCommandLine A pointer to the current command line structure. This function will update its 'inputFile' field with the input file name.
 */
void processInputRedirToken(char **nextToken, COMMAND_LINE *currentCommandLine)
{
    handleBackgroundProcessToken(nextToken, currentCommandLine);
    currentCommandLine->inputFile = alloc((c_strlen(*nextToken) + 1) * sizeof(char));
    c_strcpy(currentCommandLine->inputFile, *nextToken);
}

/**
 * @brief Processes the pipe token in a command line.
 *
 * Details: This function handles the pipe token in a command line. It increments the pipe count, sets the 'hasPipe' flag,
 * and if there is a next token, increments the command count and resets the argument count.
 *
 * @param[in,out] nextToken A pointer to the next token in the command line. This function will update it to point to the next command after the pipe.
 * @param[out] currentCommandLine A pointer to the current command line structure. This function will update its 'pipeCount' and 'hasPipe' fields.
 * @param[in,out] commandCount A pointer to the current command count. This function will increment it if there is a next token.
 * @param[out] argCount A pointer to the current argument count. This function will reset it to 0 if there is a next token.
 */
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

/**
 * @brief Sets the command line constants for each command in a command line.
 *
 * Details: This function sets the pathname for each command in a command line, null terminates each command, and 
 * sets the command count in the command line structure.
 *
 * @param[out] currentCommandLine A pointer to the current command line structure. This function will update its 'commandCount' 
 *                                field and the 'pathname' and 'argv' fields of each command.
 * @param[in] commandCount The number of commands in the command line. This function will use it to iterate over each command 
 *                         and set it as the 'commandCount' field of the command line structure.
 */
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
 * Details: This function takes a string and a delimiter as input. It tokenizes the string based on the delimiter and
 * returns the next token. If the string is NULL, it resets the input string to the beginning of the string. If the
 * string is empty, it returns NULL.
 *
 * @param[in] s The string to be tokenized.
 * @param[in] delm The delimiter to be used for tokenization.
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

/**
 * @brief Reads user input into a line.
 *
 * Details: This function reads user input into a line using the `c_fgets` function. If the user did not enter 
 * a command (i.e., only pressed enter), the function returns -1. Otherwise, it replaces the newline character 
 * at the end of the line with a null character.
 * 
 * @param[out] line A pointer to the line where the user input will be stored. This function will update it with the user input.
 * @return int Returns -1 if no command was entered by the user; otherwise, it does not return a value.
 */
int readUserInput(char *line)
{
    c_fgets(line, MAX_ARGS, STDIN_FILENO);

    // check to see if the user entered a command
    if (line[0] == '\n')
    {
        // no command entered
        return -1;
    }

    line[c_strlen(line) - 1] = '\0';
}

/**
 * @brief Changes the current working directory.
 *
 * Details: This function changes the current working directory based on the given directory string. It handles three cases: 
 * changing to the home directory, changing to the previous working directory, and changing to a specified directory.
 * 
 * @param[in] dir A pointer to a string representing the directory to change to. If this string is NULL or empty, 
 *                the function changes to the home directory. If this string is "-", the function changes to the 
 *                previous working directory. Otherwise, it changes to the specified directory.
 */
void changeDir(const char *dir)
{
    char buf[256];
    // _fputs(dir, 1);
    // _fputs("\n", 1);

    // cd without any arguments: changes the working directory to the home directory
    if (dir == NULL || c_strcmp(dir, "") == 0)
    {
        // save the current directory before changing it
        c_fputs("in empty case\n", STDOUT_FILENO);
        c_strcpy(prevPath, getcwd(buf, 256));
        chdir(getenv("HOME"));
        // chdir(getHomeDir());
    }
    // cd -: change to previous working directory
    else if (strncmp(dir, "-", 1) == 0)
    {
        c_fputs("in prev case\n", STDOUT_FILENO);
        char temp[256];
        c_strcpy(temp, getcwd(buf, 256)); // save current directory to temp
        chdir(prevPath);
        c_fputs(getcwd(buf, 256), 1);
        c_fputs("\n", STDOUT_FILENO);
        c_strcpy(prevPath, temp); // update prevPath with temp
    }
    else
    {
        c_fputs("in normal case\n", STDOUT_FILENO);
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
            // redo using c_write
            c_write("MonkeShell: cd: ", STDOUT_FILENO, RED);
            c_write(dir, STDOUT_FILENO, RED);
            c_write(": No such file or directory\n", STDOUT_FILENO, RED);

            return;
        }
    }

    printf("changed directory\n");
}

/**
 * @brief Handles the SIGCHLD signal.
 *
 * Details: This function is a signal handler for the SIGCHLD signal. It reaps all zombie child processes that have 
 * terminated but not yet been waited for.
 * 
 * @param[in] snum The signal number. This function expects it to be SIGCHLD.
 */
void handleSigChld(int snum)
{
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        // get all the zombie process
    }
}

/**
 * @brief Handles the SIGINT signal.
 *
 * Details: This function is a signal handler for the SIGINT signal. If the process receiving the signal is not 
 * the shell process, it terminates the process. Otherwise, it prompts the user to use 'exit' to terminate the shell.
 * 
 * @param[in] snum The signal number. This function expects it to be SIGINT.
 */
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

/**
 * @brief Prompts the user to turn on experimental features and sets the corresponding option in the SHELL_OPTIONS struct.
 * 
 * @param options A pointer to the SHELL_OPTIONS struct to set the experimentalFeatures option.
 */
void shellOptions(SHELL_OPTIONS *options)
{
    c_write("MonkeShell options:\n", STDOUT_FILENO, CYAN);
    // ask if they want to turn experimental features on
    c_write("Turn experimental features on [y/n] \n", STDOUT_FILENO, CYAN);

    char *input = alloc(56 * sizeof(char));
    c_fgets(input, 56, STDIN_FILENO);

    while (c_strncmp(input, "y", 1) != 0 && c_strncmp(input, "n", 1) != 0)
    {
        c_write("Invalid input, please enter 'y' or 'n'\n", STDOUT_FILENO, RED);
        c_fgets(input, 56, STDIN_FILENO);
    }

    if (c_strncmp(input, "y", 1) == 0)
    {
        options->experimentalFeatures = 1;
    }
    else
    {
        options->experimentalFeatures = 0;
    }
}

/**
 * @brief This function prints a welcome message to the user.
 * It displays the MonkeShell logo in ASCII art and a greeting message.
 */
void printWelcomeMessage()
{
    // clear the screen
    c_write(CLEAR_SCREEN, 1, NULL);
    // move cursor to top left
    c_write(MOVE_PAGE_UP, 1, NULL);

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

    c_write("Welcome to MonkeShell!\n\n", 1, NULL);
}

/**
 * Prints the shell prompt, which consists of the current working directory
 * and a "$" symbol.
 */
void printPrompt()
{
    c_write(getcwd(prompt, 256), 1, CYAN);
    c_write("$ ", 1, PURPLE);
}

/****************************************** EXPIREMENTAL *******************************************/
int readUserInputExp(char *line)
{
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);          // get current terminal I/O settings
    new_tio = old_tio;                          // make new settings same as old settings
    new_tio.c_lflag &= (~ICANON & ~ECHO);       // disable buffered I/O and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio); // apply new settings

    int c;
    int i = 0;
    int hIndex = 0;

    while ((c = getchar()) != '\n')
    {
        if (c == '\t')
        {
            char *suggestion = generateSuggestions(line);
            if (suggestion == NULL)
            {
                // beep
                putchar('\a');
            }
            else
            {
                // replace entire line with suggestion
                for (int j = 0; j < i; j++)
                {
                    putchar('\b'); // move cursor back
                }
                for (int j = 0; j < i; j++)
                {
                    putchar(' '); // overwrite with spaces
                }
                for (int j = 0; j < i; j++)
                {
                    putchar('\b'); // move cursor back again
                }

                strcpy(line, suggestion);
                printf("%s", line);
                i = strlen(line);
            }

            // reset suggestion
            suggestion = NULL;
        }
        else if (c == 127) // ASCII value of backspace on Unix-like systems
        {
            if (i > 0)
            {
                i--;
                putchar('\b'); // move cursor back
                putchar(' ');  // overwrite with space
                // remove from line
                for (int j = i; j < strlen(line); j++)
                {
                    line[j] = line[j + 1];
                }
                putchar('\b'); // move cursor back again
            }
        }
        else if (c == 27)
        {
            // arrow keys
            getchar(); // skip the '[' character
            switch (getchar())
            {         // the third character will indicate which arrow key was pressed
            case 'A': // up arrow
                strcpy(line, readCommandHistory(hIndex));

                for (int j = 0; j < i; j++)
                {
                    putchar('\b');    // move cursor back
                    putchar(' ');     // overwrite with space
                    putchar('\b'); // move cursor back again
                }

                printf("%s", line); // overwrite the line with the command from history

                i = strlen(line);

                hIndex++;

                break;
            case 'B': // down arrow
                hIndex--;

                if (hIndex < 0)
                    hIndex = 0;

                strcpy(line, readCommandHistory(hIndex));

                for (int j = 0; j < i; j++)
                {
                    putchar('\b');    // move cursor back
                    putchar(' ');     // overwrite with space
                    putchar('\b'); // move cursor back again
                }

                printf("%s", line); // overwrite the line with the command from history

                i = strlen(line);

                break;
            case 'C': // right arrow
            case 'D': // left arrow
                break;
            }
        }
        else
        {
            line[i] = c;
            putchar(c); // echo character immediately
            i++;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // restore old settings

    printf("\nline: %s\n", line);

    if (line[0] == '\0')
    {
        return -1;
    }
}

void addCommandToHistory(char *command)
{
    // open the file in append mode
    int fd = open("history.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd != -1)
    {
        write(fd, command, strlen(command)); // write the command to the file
        write(fd, "\n", 1);                  // write a newline character
        close(fd);                           // close the file
    }
    else
    {
        c_write("Failed to open history.txt", 1, RED);
    }
}

char *readCommandHistory(int index)
{

    // read the command at the specified index from the history file
    int fd = open("history.txt", O_RDONLY);
    if (fd != -1)
    {
        char *line = alloc(1024 * sizeof(char));
        int i = 0;
        int j = 0;
        int count = 0;
        char c;

        while (read(fd, &c, 1) > 0)
        {
            if (c == '\n')
            {
                count++;
                if (count == index + 1)
                {
                    line[i] = '\0';
                    break;
                }
                else
                {
                    i = 0;
                }
            }
            else
            {
                line[i] = c;
                i++;
            }
        }

        close(fd);

        return line;
    }
    else
    {
        c_write("Failed to open history.txt", 1, RED);
        return NULL;
    }
}

char *generateSuggestions(char *line)
{
    int i = 0;
    int j = 0;

    // find the last space in the line
    char *last_space = strrchr(line, ' ');
    char *search_term = line;
    if (last_space != NULL)
    {
        search_term = last_space + 1; // start search after the last space
    }

    // search for potential commands first before searching for files

    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < c_strlen(search_term); j++)
        {
            if (search_term[j] != potentialCommands[i][j])
            {
                break;
            }
        }
        if (j == c_strlen(search_term))
        {
            // replace search term in line with potential command
            strcpy(search_term, potentialCommands[i]);
            return line;
        }
    }

    // if no potential commands match, search for files

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            for (j = 0; j < c_strlen(search_term); j++)
            {
                if (search_term[j] != dir->d_name[j])
                {
                    break;
                }
            }
            if (j == c_strlen(search_term))
            {
                // replace search term in line with file/folder name
                strcpy(search_term, dir->d_name);
                closedir(d);
                return line;
            }
        }
        closedir(d);
    }

    return NULL;
}
