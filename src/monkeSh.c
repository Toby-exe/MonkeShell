/**
 * @file monkeSh.c
 * @brief Custom shell implementation
 * @authors Tobias Wondwossen, Jayden Mingle
 * 
 * Details: 
 * - Installs signal handlers for SIGINT and SIGCHLD
 * - Prompts user for a command and executes it.
 * - Supports multiple commands in one line
 * - Supports two built in commands: cd and exit
 * - Commands can by typed in with or without the full path
 * - If command is not entered, shell will prompt user again until a command is entered
 * 
 * @return 0 upon successful completion of the program.
 */
#include "myShell.h"

int shellPid;

int main()
{
  
  shellPid = getpid();
  signal(SIGINT, handleSigInt);
  signal(SIGCHLD, handleSigChld); // detect child termination
  COMMAND_LINE currentCommandLine;
  SHELL_OPTIONS options;

  shellOptions(&options);
  printWelcomeMessage();

  while (1)
  {
    readCommandLine(&currentCommandLine, options.experimentalFeatures);

    while (currentCommandLine.commandCount == 0)
    {
      readCommandLine(&currentCommandLine, options.experimentalFeatures);
    }

    if (c_strcmp(currentCommandLine.commands[0].pathname, "exit") == 0)
    {
      break;
    }

    if (c_strcmp(currentCommandLine.commands[0].pathname, "cd") == 0)
    {
      changeDir(currentCommandLine.commands[0].argv[1]);
      continue;
    }

    cmdHandler(currentCommandLine);
  }
  return 0;

}
