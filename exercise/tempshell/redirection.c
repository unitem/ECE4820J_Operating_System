#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "p1.h"

int search_for_redirect(const char *sect) {
    int redirect_type = normal;
    if(strcmp(sect, "<")==0){
        redirect_type = stdin_r;
    } else if (strcmp(sect, ">")==0){
        redirect_type = stdout_r;
    } else if (strcmp(sect, ">>")==0){
        redirect_type = stdout_append;
    }
    return redirect_type;
}

int redirect_process(char **execu, int *num, char *file_stdin, char *file_stdout, int* pipe_pos) {
    int redirect_type = normal;
    int i = 0;
    int change = 0;
    while (i < *num) {
        if(strcmp(execu[i], "|") == 0){
            *pipe_pos = i;
            break;
        }

        int new_type = search_for_redirect(execu[i]);
        if (new_type == stdin_r) {
            if (redirect_type == stdout_r){
                redirect_type = stdin_stdout;
                change = 1;
            } else if (redirect_type == stdout_append){
                redirect_type = stdin_stdout_append;
                change = 1;
            } else if (redirect_type == normal){
                redirect_type = stdin_r;
                change = 1;
            }
        } else if (new_type == stdout_r && redirect_type == stdin_r) {
            redirect_type = stdin_stdout;
            change = 1;
        } else if (new_type == stdout_append && redirect_type == stdin_r) {
            redirect_type = stdin_stdout_append;
            change = 1;
        } else if (new_type != normal && redirect_type == normal) {
            redirect_type = new_type;
            change = 1;
        }


        if(change == 1){
            if (new_type == stdin_r){
                strcpy(file_stdin, execu[i + 1]);
                elim_quo(file_stdin);
                adjust_parser(execu, i, i + 2, num);
            } else if (new_type != normal){
                strcpy(file_stdout, execu[i + 1]);
                elim_quo(file_stdout);
                adjust_parser(execu, i, i + 2, num);
            }
        } else {
            i++;
        }
        change = 0;
    }
    return redirect_type;
}

void redirect_std(const int type, const char* file_name_in, const char* file_name_out){
    int fd;
    switch (type) {
        case stdin_r:
            fd = open(file_name_in, O_RDONLY);
            if (fd == -1) error_handler(non_exist_file_error, NULL, file_name_in); // O_CREAT | O_RDWR | O_APPEND, 0644
            dup2(fd, STDIN_FILENO);
            close(fd);
            break;
        case stdout_r:
            fd = open(file_name_out, O_CREAT | O_RDWR | O_TRUNC, 0644);
            if (fd == -1) error_handler(permission_error, NULL, file_name_out);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            break;
        case stdout_append:
            fd = open(file_name_out, O_CREAT | O_RDWR | O_APPEND, 0644);
            if (fd == -1) error_handler(permission_error, NULL, file_name_out);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            break;
        case stdin_stdout:
            fd = open(file_name_in, O_RDONLY);
            if (fd == -1) error_handler(non_exist_file_error, NULL, file_name_in); // O_CREAT | O_RDWR | O_APPEND, 0644
            dup2(fd, STDIN_FILENO);
            close(fd);
            fd = open(file_name_out, O_CREAT | O_RDWR | O_TRUNC, 0644);
            if (fd == -1) error_handler(permission_error, NULL, file_name_out);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            break;
        case stdin_stdout_append:
            fd = open(file_name_in, O_RDONLY);
            if (fd == -1) error_handler(non_exist_file_error, NULL, file_name_in); // O_CREAT | O_RDWR | O_APPEND, 0644
            dup2(fd, STDIN_FILENO);
            close(fd);
            fd = open(file_name_out, O_CREAT | O_RDWR | O_APPEND, 0644);
            if (fd == -1) error_handler(permission_error, NULL, file_name_out);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            break;
        default:
            break;
    }
}
