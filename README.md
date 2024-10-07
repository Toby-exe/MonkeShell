# MonkeShell
Shell program (monkeShell) that provides a command line interface for a Linux user. Provides a subset of the functionality found in common Linux shells such as bash.

## Current Features
- [x] Execute commands with arguments
- [x] Execute commands in the background
- [x] Change directories
- [x] Redirect input and output
- [x] Pipelines
- [x] Signal handling  

## Getting Started

### Option 1:

```bash
MonkeShell$ make
MonkeShell$ ./monke
```

### Option 2:

```bash
MonkeShell$ make all
```

## Usage

### Valid Command Line

```bash
______  ___               ______
___   |/  /______ _______ ___  /_______
__  /|_/ / _  __ \__  __ \__  //_/_  _ \
_  /  / /  / /_/ /_  / / /_  ,<   /  __/
/_/  /_/   \____/ /_/ /_/ /_/|_|  \___/
_____________  ______________________
__  ___/__  / / /__  ____/__  /___  /
_____ \__  /_/ /__  __/  __  / __  /
____/ /_  __  / _  /___  _  /___  /___
/____/ /_/ /_/  /_____/  /_____/_____/

Welcome to MonkeShell!

~/MonkeShell$ command [arg1] [arg2] ... [argn] [< input_file] | command [arg1] [arg2] ... [argn] [> output_file] | ... | command [arg1] [arg2] ... [argn] [&]
```
