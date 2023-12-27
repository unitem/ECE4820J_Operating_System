# ECE482 p1- Mumsh Project

## Author

Name: Weiqing Xu
Student ID: 520021910400
Email: sjtu_xwq@sjtu.edu.cn

## Compiling and running

Compiling:

```bash
cd /
make
```

Running:
```bash
cd /
./mumsh
```
Then you can interact with my `mumsh`. See the upported features and usage below.

## Project structure

The whole program contains  files, `mumsh.c`, `mumsh.h`, `basic_io.c`, `basic_io.h`, `argslist.c`, `argslist.h`, `other_utils.c`, `other_utils.h`, `parse.c`, `parse.h`.
- `mumsh.c` is the main file, which contains the main function, the read-parse-execute loop as basic logic.
- `basic_io.c` contains the basic prompt and error message print functions.
- `argslist.c` contains the data structure which contains the args and related functions.
- `other_utils.c` contains some other utility functions like `cd` and `pwd` implemented by myself.
- `parse.c` contains the parse function which parse the arglist to solve the input logic.
- `*.h` files are the header files for the corresponding `*.c` files, contains the necessary header files, global variables and the function declarations.

## Supported Features and usage

1. The mumshell will infinitely loop to prompt you `mumsh $` and read command from stdin.
2. Type `exit` to exit the shell.
3. Single command with or without arguments. e.g. `ls` or `ls -l`
4. Bash style redirections, including:
    1. Redirection: `>` and `>>` to redirect stdout to a file, `<` to redirect stdin from a file
    2. Combination of redirections. e.g. `cat 1.txt < 2.txt > 3.txt`. Note that the redirections can be placed anywhere in the command.
    3. Combination with pipeline, e.g. `echo 321 < 1.txt | > 3.txt cat`
    4. Cascaded pipelines like `echo 321 | cat | > 2.txt cat`. 
    5. Pipeline run in parallel. e.g. `sleep 15 | sleep 15` will take 15 seconds to finish.
5. CTRL+D: If a process is running or there exists input in the stdin, it will do nothing. If the command line is empty, it will exit the shell.
6. CTRL+C: It will reset everything and start a new prompt like bash, regardless of the current input. If a process is running, it will kill the process.
9. pwd and cd as self-defined commands.
10. Precise error messages, including `command not found`, `permission denied`, etc. See `basic_io.h` for details. 

