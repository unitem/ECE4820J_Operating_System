

#ifndef VE482P1_P1_H
#define VE482P1_P1_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

// error enum type
typedef enum error {
    non_exist_file_error,
    permission_error,
    command_error,
    fork_error,
    cd_error,
    dup_input,
    dup_output,
    syntax_error,
    missing_program
} error;

typedef enum redirect {
    normal,
    stdin_r,
    stdout_append,
    stdout_r,
    stdin_stdout,
    stdin_stdout_append
} redirect;

void error_handler(int error, const char *command, const char *file_name);
void INThandler_child(int sig);
void INThandler_parent(int sig);
void elim_quo(char* string_t);
int parse_input(char input[], char* execu[]);
void adjust_parser(char **parser, int begin, int end, int *num);
void pre_process(char* input);
int check_bg(char** parser, int *num_arg);
int check_completeness(char* input);
int redirect_process(char **execu, int *num, char *file_stdin, char *file_stdout, int* pipe_pos);
void redirect_std(int type, const char* file_name_in, const char* file_name_out);
void process_pwd(void);
void process_cd(int num_arg, char** execu, char** last_path);
void job(pid_t** record, int num_bg, const int child_num[]);
void execute_process(char *execu[], int* num_arg, int fd_in, int if_pipe, int if_bg, pid_t** record, int num_bg, int child[], char* last_dir);
void mumsh(void);

#endif //VE482P1_P1_H
