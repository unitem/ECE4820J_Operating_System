## Mumsh shell project

[![Build Status](https://focs.ji.sjtu.edu.cn:2222/api/badges/ECE482-22/PingchuanMa519370910019-p1/status.svg?ref=refs/heads/master)](https://focs.ji.sjtu.edu.cn:2222/ECE482-22/PingchuanMa519370910019-p1)

#### I. Build badge
The whole program contains 6 files, `main.c`, `execute.c`, `redirection.c`, `handler.c`, `parser.c` and header file `p1.h`.
- `main.c`: main program
- `execute.c`: contains all the execution functions and built-in command implementation (e.g. fork to open child process)
- `redirection.c`: contains all the functions processing I/O redirection check and implementation
- `handler.c`: contains error handler and ctrl+C interrupt handler
- `parser.c`: contains all the functions transforming the input string intro parser, which contains segments of strings better for execution.
- `p1.h`: contains all the function declaration, enum declaration

The main code structure and logic are as follows.

(a) The main function `void mumsh(void)` starts, and there's a read-parse-execute loop continuously read the input from users.

(b) Then there're several parser function to process the input string, such as checking & for background process, inserting space before and after >, <, |, checking if the command is completed, finally break it into several segments stored in **parser**.

(c) Then the mumsh function calls function `void execute_process` (written in recursion), which in our father process, check the I/O redirection if any from parser, break the segments if detecting |. And then fork() to create child process, implement I/O redirection, execute the corresponding commands.

(d) Harvest all the child processes, and then continue reading input from users. 

#### II. Build & installation instruction
```shell
$ mkdir cmake-build-debug && cd cmake-build-debug
$ cmake -DCMAKE_C_COMPILER=clang ..
$ make
```

#### III. Running instructions
In your terminal run `./mumsh`, then you can enter the **mumsh** interface.
- single commands without arguments, e.g. `ls`.
- commands with arguments, e.g. `ls -a`.
- File I/O redirection (also support bash style redirection syntax), e.g. `cat < 1.txt 2.txt > 3.txt 4.txt`. 
- Pipe support, also runs in parallel, supports arbitrarily deep "cascade pipes", and can be combined with I/O redirection.
- Support CTRL-D and CTRL-C
- Support built-in commands for `cd` and `pwd`.
- Support quotes and incomplete quotes (will output '>'), e.g. `echo "de'f' ghi" '123"a"bc' a b c`
- Wait for the command to be completed when encountering >, <, or |.
- Support background process and `jobs` commands.


#### IV. Author information
Pingchuan Ma, senior undergraduate student from UM-SJTU JI, email: hensonma@sjtu.edu.cn



