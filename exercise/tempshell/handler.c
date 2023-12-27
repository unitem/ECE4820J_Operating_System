#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "p1.h"

extern int command_empty;

void INThandler_child(int sig){
    if (sig != SIGINT) return;
    exit(0);
}

void INThandler_parent(int sig){
    if (sig != SIGINT) return;
    printf("\nmumsh $ ");
    command_empty = 1;
}


void error_handler(int error, const char *command, const char *file_name) {
    switch (error) {
        case non_exist_file_error:
            fprintf(stderr, "%s: No such file or directory\n", file_name);
            break;
        case permission_error:
            fprintf(stderr, "%s: Permission denied\n", file_name);
            break;
        case command_error:
            fprintf(stderr, "%s: command not found\n", command);
            break;
        case fork_error:
            fprintf(stderr, "can't create child process.\n");
            break;
        case cd_error:
            if(strcmp(file_name, "-")==0) fprintf(stderr, "-bash: cd: OLDPWD not set\n");
            else fprintf(stderr, "%s: No such file or directory\n", file_name);
            break;
        case dup_input:
            fprintf(stderr, "error: duplicated input redirection\n");
            break;
        case dup_output:
            fprintf(stderr, "error: duplicated output redirection\n");
            break;
        case missing_program:
            fprintf(stderr, "error: missing program\n");
            break;
        case syntax_error:
            fprintf(stderr, "syntax error near unexpected token `%s\'\n", command);
            break;
        default:
            break;
    }
    if(error != cd_error) exit(0);
}
