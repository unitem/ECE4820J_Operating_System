#include "basic_io.h"

void prompt(const char *message)
// EFFECTS: print the message to stdout
// MODIFIES: stdout
{
    printf("%s", message);
    fflush(stdout);
}

void promptn(const char *message)
// EFFECTS: print the message to stdout
// MODIFIES: stdout
{
    printf("%s\n", message);
    fflush(stdout);
}

void cmd_error(const char *cmd)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "%s: command not found\n", cmd);
    fflush(stderr);
}

void syntax_near_error(const char *message) 
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "syntax error near unexpected token `%s'\n", message);
    fflush(stderr);
}

void dup_out_red_error(void)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "error: duplicated output redirection\n");
    fflush(stderr);
}

void dup_in_red_error(void)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "error: duplicated input redirection\n");
    fflush(stderr);
}

void oldpwd_error(void)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "error: OLDPWD not set\n");
    fflush(stderr);
}

void cd_error(const char *dir)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "%s: No such file or directory\n", dir);
    fflush(stderr);
}

void cd_null_error(void)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "NULL: No such file or directory\n");
    fflush(stderr);
}

void file_nonexist_error(const char *file)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "%s: No such file or directory\n", file);
    fflush(stderr);
}

void permission_error(const char *file)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "%s: Permission denied\n", file);
    fflush(stderr);
}

void miss_prog_error(void)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "error: missing program\n");
    fflush(stderr);
}

void cd_arg_error(void)
// EFFECTS: print the error message to stderr
// MODIFIES: stderr
{
    fflush(stdout);
    fprintf(stderr, "cd: too many arguments\n");
    fflush(stderr);
}
