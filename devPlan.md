# How we handled development

## Tech Used For Collaboration And Development

We used `Github` to store our code and to collaborate on it. 

We used `Discord` to communicate with each other and to share our screens when we needed help or want to do some pair programming.

We used `Visual Studio Code` as our IDE via the Microsoft ssh extension to connect to our `Ubuntu` vm's.

## Devlopment Timeline

### Week 1 - Setup Stage

#### Goals 

1. Learn how to use git
2. Learn more about how a shell works at a high level
3. Find an alternative to using emacs, perhaps find a way to use vscode
4. Learn about particular aspects of linux / operating systems such as child processes, files, signals, etc.

#### Achived 
During this week, we successfully set up the git repository and learned how to use git for version control. We also set up virtual machines (VMs) to provide a consistent development environment and learned how to use them effectively. To make our development process more comfortable, we learned how to use SSH and configured it to use Visual Studio Code (VS Code) as our primary code editor. We didn’t make much progress in terms of coding, but we gained a high-level understanding of how a shell works, including concepts like child processes, files, signals, etc.

### Week 2 - Planning Stage 

#### Goals: 

1. Get a high level view of what components make up a shell and what needs to be implemented
2. Decide on what extra features we want to implement
3. Start implementing basic components of the shell such as a tokenizer, a parser, a utils, etc.

#### Achived: 
In the second week, we started planning out the modules of the shell using Miro for visual collaboration. We created a basic program that could run commands in a child process. We also started working on a utility module that would replace standard library functions with our own implementations. This included starting work on a parser that would parse the input (tokenizer). We decided on the extra features we wanted to implement: Signals, N stage pipes, basic command path searching, printing the current directory, changing the current directory, and colors. If time permitted, we planned to implement tab completion, history, and jobs.

### Week 3

#### Goals: 

1. Start implementing bigger components of the shell.
2. Implement compete tokenizer and parser that can handle pipes, redirection, and background processes.
3. Implement basic path searching for commands to use with execve such that we do not have to use the full path to the command.
4. Start resarching how jobs and signals work in linux.

#### Achived: 
During the third week, we made significant progress in implementing the larger components of the shell. We completed the tokenizer that tokenizes based on the space delimiter. This allowed us to handle individual commands and their arguments effectively.

We also completed a utility module that replaced standard library functions with our own implementations. However, we identified some bugs in these custom functions that needed to be addressed.

In addition, we implemented a basic version of the parser that could handle all the commands we wanted to implement. This parser could handle pipes, redirection, and background processes. However, it had some bugs and did not account for some special cases, such as when ‘&’ is at the end of a command (e.g., ‘ls&’ vs ‘ls &’).

Furthermore, we implemented basic path searching for commands to use with execve. This meant that we didn’t have to use the full path to the command, making our shell more user-friendly.

Lastly, we started researching how jobs and signals work in Linux to prepare for implementing these features in our shell.
### Week 4

#### Goals: 

1. Fix bugs in the tokenizer and parser and account for special cases.
2. Implement N stage pipes
3. Implement I/O redirection
4. Implement background processes
5. Implement Signals 

#### Achived: 

During the fourth week, we were able to fix all the known bugs with the tokenizer and parser. This included handling special cases such as when ‘&’ is at the end of a command (e.g., ‘ls&’ vs ‘ls &’).

We also made significant progress in implementing I/O redirection and background processes. This allowed our shell to handle more complex command lines and run commands in the background, making it more versatile and user-friendly.

However, we encountered some challenges with implementing N stage pipes. Despite our best efforts, we were not able to fully implement this feature during this week.

Lastly, we planned to implement signals in our shell, but we did not get to this task this week. We will carry this goal forward to the next week.

### Week 5

#### Goals: 

1. Implement N stage pipes
2. Implement Signals
3. Attempt to implement tab completion and history
4. Attempt to implement jobs
5. Compelete Documentation and other supporting documents
6. Implement colors
7. Implement printing the current directory
8. Implement changing the current directory

#### Achived: 

This week we are able to implement N stage pipes. We also implemented correct managment of background processes via signals. We were also able to complete colors, printing the current directory, changing the current directory. We had been documenting our progress throughout the project, so we were able to complete the documentation and other supporting documents. We did make a great effort to implement jobs, command history, and auto complete but they proved to be too difficult to implement in the time we had left. 

## How We Divided the Work

| Name             | Tasks                                                                 |
|------------------|-----------------------------------------------------------------------|
| `Tobias Wondwossen`| Redirection, N stage pipelines, colors, parser, tokenizer, background processes, path searching, code, testing driver,  organization |
| `Jayden Mingle`    | Custom string library, custom IO library, custom mem library, signals, Changing Directory Command, Printing current Directory,  |
| `All`    | Documentation, Planning, Managment|


