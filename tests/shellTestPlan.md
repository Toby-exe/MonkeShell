# Interactive Test plan for Shell

## Getting Started

To ensure consistency when testing the shell interactively should begin in the testEnv directory after the program has been compiled.

```bash
cd MonkeShell/tests/testEnv
```
Ensure `testEnv` contains the following files and directories:

```bash
foo numbers.txt random.c
```

Ensure foo contains the following files and directories:

```bash
foo.c
```

Ensure numbers.txt contains the following numbers:

```bash
1 34 5 2 3 5 6 75 456 334 2232 34 1 33 56 223 2 23 8 9 42 352 23 4 5334 322
```

## Basic Shell Functionality

| Command | Expected Output | Reason for Test |
| ------- | --------------- | --------------- |
| `ls` | `foo numbers.txt random.c` | Test if the `ls` command correctly lists the files and directories in the current directory. |
| `/usr/bin/ls` | `foo numbers.txt random.c` | Test if the `ls` command correctly lists the files and directories in the current directory when given the full path. |
| `cat numbers.txt` | `1 34 5 2 3 5 6 75 456 334 2232 34 1 33 56 223 2 23 8 9 42 352 23 4 5334 322` | Test if the `cat` command correctly displays the content of 'numbers.txt'. |
| `gcc -o random random.c` and `./random` | Varies (output of random.c) | Test if the `gcc` command correctly compiles 'random.c' and if the resulting executable runs correctly. |
| `clear` | Clears the terminal | Test if the `clear` command correctly clears the terminal and ensures the enviroment vars are correct. |

## Shell Functionality with Input Redirection

| Command | Expected Output | Reason for Test |
| ------- | --------------- | --------------- |
| `sort < numbers.txt` | `1 1 2 2 3 3 4 5 5 6 8 9 23 23 34 34 42 56 75 223 2232 322 334 352 456 5334` | Test if the `sort` command correctly sorts the numbers in 'numbers.txt' when used with input redirection. |
| `wc -l < numbers.txt` | `26` | Test if the `wc -l` command correctly counts the number of lines in 'numbers.txt' when used with input redirection. |
| `grep "34" < numbers.txt` | `34 334 34 5334` | Test if the `grep` command correctly finds the lines containing "34" in 'numbers.txt' when used with input redirection. |

## Shell Functionality with Output Redirection

| Command | Expected Output (In output file) | Reason for Test |
| ------- | --------------- | --------------- |
| `sort numbers.txt > sorted.txt` | `1 1 2 2 3 3 4 5 5 6 8 9 23 23 34 34 42 56 75 223 2232 322 334 352 456 5334` | Test if the `sort` command correctly sorts the numbers in 'numbers.txt' and redirects the output to 'sorted.txt'. |
| `wc -l numbers.txt > linecount.txt`  | `26` | Test if the `wc -l` command correctly counts the number of lines in 'numbers.txt' and redirects the output to 'linecount.txt'. |
| `grep "34" numbers.txt > grep_results.txt` |`34 334 34 5334` | Test if the `grep` command correctly finds the lines containing "34" in 'numbers.txt' and redirects the output to 'grep_results.txt'. |

## Shell Functionality with Pipes

| Command | Expected Output | Reason for Test |
| ------- | --------------- | --------------- |
| `cat numbers.txt \| sort \| uniq` | `1 2 3 4 5 6 8 9 23 34 42 56 75 223 2232 322 334 352 456 5334` | Test if the `cat`, `sort`, and `uniq` commands correctly display, sort, and remove duplicates from 'numbers.txt' when used with pipes. |
| `cat numbers.txt \| wc -l` | `26` | Test if the `cat` and `wc -l` commands correctly display and count the number of lines in 'numbers.txt' when used with pipes. |
| `cat numbers.txt \| grep "34"` | `34 334 34 5334` | Test if the `cat` and `grep` commands correctly display and find the lines containing "34" in 'numbers.txt' when used with pipes. |
| `ls \| wc -l` | `3` | Test if the `ls` and `wc -l` commands correctly display and count the number of files and directories in the current directory when used with pipes. |

## Shell Functionality with Background Processes

| Command | Expected Output | Reason for Test |
| ------- | --------------- | --------------- |
| `ls &` | `Added child to background process list` <br> `Child PID: <pid_here>` <br> foo numbers.txt random.c` | Test if the `ls` command correctly lists the files and directories in the current directory when run in the background and `&`flag is seperate from `ls`command. |
|`ls&` | `Added child to background process list` <br> `Child PID: <pid_here>` <br> foo numbers.txt random.c` | Test if the `ls` command correctly lists the files and directories in the current directory when run in the background but `&` flag is part of the `ls`command. | 
| `cat numbers.txt &` | `Added child to background process list` <br> `Child PID: <pid_here>` <br> `1 34 5 2 3 5 6 75 456 334 2232 34 1 33 56 223 2 23 8 9 42 352 23 4 5334 322` | Test if the `cat` command correctly displays the content of 'numbers.txt' when run in the background. |

## Shell Functionality with Built-in Commands

| Command | Expected Output | Reason for Test |
| ------- | --------------- | --------------- |
| `cd foo` | `/home/twond799/monkeShell/MonkeShell/tests/testEnv/foo` | Test if the `cd` command correctly changes the current directory to 'foo'. |
| `cd ..` | `/home/twond799/monkeShell/MonkeShell/tests` | Test if the `cd` command correctly changes the current directory to the parent directory. |
| `pwd` | `/home/twond799/monkeShell/MonkeShell/tests` | Test if the `pwd` command correctly displays the current directory. |
| `cd` | `/home/twond799` | Test if the `cd` command correctly changes the current directory to the home directory. |
| `cd -` | `/home/twond799/monkeShell/MonkeShell/tests` | Test if the `cd` command correctly changes the current directory to the previous directory. |
| `cd ~` | `/home/twond799` | Test if the `cd` command correctly changes the current directory to the home directory. |
| `cd ~/twond799/monkeShell` | `/home/twond799/monkeShell` | Test if the `cd` command correctly changes the current directory to the specified directory. |
| `exit` | `Exiting shell` | Test if the `exit` command correctly exits the shell. |

## Signal Handling

| Command | Expected Output | Reason for Test |
| ------- | --------------- | --------------- |
| `sleep 10s` | `sleep 10s` | Test if the `sleep` command correctly sleeps for 10 seconds. |
| `ls &` then `ps` | `Added child to background process list` <br> `Child PID: <pid_here>` <br> then <br> `    PID TTY          TIME CMD` <br>`62084 pts/0    00:00:00 bash` <br>  ` 63116 pts/0    00:00:00 monkeSh` <br>  ` 63118 pts/0    00:00:00 ps`| Test if the `ls` command when run in the background is removed from the proccess list when it is "done". | 
| `sleep 10s &` then `ps` | `Added child to background process list` <br> `Child PID: <pid_here>` <br> then <br> `    PID TTY          TIME CMD` <br>`62084 pts/0    00:00:00 bash` <br>  ` 63116 pts/0    00:00:00 monkeSh` <br>  ` 63117 pts/0    00:00:00 sleep` <br>  ` 63118 pts/0    00:00:00 ps` <br> after 10 seconds <br> `Child process terminated` <br> `Child PID: 63117` | Test if the `sleep` command correctly sleeps for 10 seconds when run in the background. |

## Composite Commands


| Command | Expected Output | Reason for Test |
| ------- | --------------- | --------------- |
| `cat numbers.txt \| sort \| uniq \| sed s/^/Number:/ > write.txt&` and `cat write.txt` | Sorted unique numbers from 'numbers.txt' each prepended with "Number:" | Test if the `sort`, `uniq`, `sed`, and output redirection commands correctly sort, remove duplicates, prepend each number with "Number:", and write the output to 'write.txt' as a background process. |
| `cat numbers.txt \| sort \| uniq \| sed s/^/Number:/` | `Number:1 Number:2 Number:3 Number:4 Number:5 Number:6 Number:8 Number:9 Number:23 Number:34 Number:42 Number:56 Number:75 Number:223 Number:2232 Number:322 Number:334 Number:352 Number:456 Number:5334` | Test if the `cat`, `sort`, `uniq`, and `sed` commands correctly display, sort, remove duplicates from 'numbers.txt', and prepend each number with "Number:" when used together. |
| `cat numbers.txt \| sort -n \| uniq -c \| sort -nr` | Frequency of each number in 'numbers.txt' sorted in descending order | Test if the `cat`, `sort -n`, `uniq -c`, and `sort -nr` commands correctly display, sort, count the frequency, and sort the frequency of numbers in 'numbers.txt' when used together. |
| `cat numbers.txt \| grep "2" \| wc -l` | Number of lines containing "2" in 'numbers.txt' | Test if the `cat`, `grep "2"`, and `wc -l` commands correctly display, find the lines containing "2", and count these lines in 'numbers.txt' when used together. |
