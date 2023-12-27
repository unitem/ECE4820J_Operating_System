#ifndef BASIC_IO_H
#define BASIC_IO_H
#include <stdio.h>

void prompt(const char *message);
// EFFECTS: print the message to stdout

void promptn(const char *message);
// EFFECTS: print the message to stdout with a newline
// MODIFIES: stdout

void cmd_error(const char *cmd);
// no such command

void syntax_near_error(const char *message);
// wrong syntax

void dup_out_red_error(void);
// more than 1 redirected output

void dup_in_red_error(void);
// moe than 1 redirected input

void oldpwd_error(void);
// cd - but no last path

void cd_error(const char *dir);
// cd to somewhere not exist

void cd_null_error(void);
// cd to null

void file_nonexist_error(const char *file);
// use a file that does not exist

void permission_error(const char *file);
// no permission to execute on such file

void miss_prog_error(void);
// miss executable command

void cd_arg_error(void);
// more than 1 args passed to cd

#endif
