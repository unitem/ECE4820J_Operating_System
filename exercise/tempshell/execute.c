#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "p1.h"

// global variable
int command_empty = 1;
char buffer_bg[50][1024];

void process_pwd(void){
    char* pwd = NULL;
    pwd = getcwd(pwd, 1024);
    printf("%s\n", pwd);
    free(pwd);
}

void process_cd(int num_arg, char** execu, char** last_path){
    char* dir = NULL;
    if (num_arg==1 || (num_arg==2 && strcmp(execu[1], "~")==0)) {
        dir = getenv("HOME");
    } else if (num_arg==2 && strcmp(execu[1], "-")==0) {
        if(*last_path!=NULL){
            dir = malloc(1024);
            bzero(dir, 1024);
            strcpy(dir, *last_path);
        }
    }
    else dir = execu[1];
    *last_path = getcwd(*last_path, 1024);
    if(dir!=NULL){
        if(chdir(dir)==-1) error_handler(cd_error, NULL, execu[1]);
    }
    else error_handler(cd_error, NULL, execu[1]);

    if(num_arg==2 && strcmp(execu[1], "-")==0 && dir!=NULL) free(dir);
}

void job(pid_t** record, int num_bg, const int child_num[]){
    for(int i=0;i<num_bg;i++){
        int flag = 0;
        for(int j=0;j<child_num[i];j++){
            if(waitpid(record[i][j], NULL, WNOHANG)==0){
                printf("[%d] running %s", i+1, buffer_bg[i]);
                flag = 1;
                break;
            }
        }
        if (flag==0) printf("[%d] done %s", i+1, buffer_bg[i]);
    }
}

void error_check(char** execu, int num_arg, int if_pipe_in, int if_pipe_out, int if_stdin, int if_stdout){
    if(if_pipe_in == 1 && if_stdin == 1) error_handler(dup_input, NULL, NULL);
    if(if_pipe_out == 1 && if_stdout == 1) error_handler(dup_output, NULL, NULL);

    for(int i=0;i<num_arg;i++){
        if(strcmp(execu[i], "<")==0 && if_stdin == 1) error_handler(dup_input, NULL, NULL);
        if((strcmp(execu[i], ">")==0 || strcmp(execu[i], ">>")==0) && if_stdout == 1) error_handler(dup_output, NULL, NULL);
    }
}

void execute_process(char *execu[], int* num_arg, int fd_in, int if_pipe, int if_bg, pid_t** record, int num_bg, int child[], char* last_dir) {
    char file_in[50];
    char file_out[50];
    bzero(file_in, 50);
    bzero(file_out, 50);
    int pipe_pos = -1;
    int redirection = redirect_process(execu, num_arg, file_in, file_out, &pipe_pos);

    int fd[2];
    if (pipe_pos!=-1) pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, INThandler_child);

        if(strcmp(file_in, ">") == 0 || strcmp(file_in, ">>") == 0 || strcmp(file_in, "<") == 0 || strcmp(file_in, "|") == 0){
            error_handler(syntax_error, file_in, NULL);
        } else if (strcmp(file_out, ">") == 0 || strcmp(file_out, ">>") == 0 || strcmp(file_out, "<") == 0 || strcmp(file_out, "|") == 0){
            error_handler(syntax_error, file_out, NULL);
        }

        redirect_std(redirection, file_in, file_out);

        if (pipe_pos!=-1) {
            adjust_parser(execu, pipe_pos, *num_arg, num_arg);
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
        }
        if (if_pipe == 1) dup2(fd_in, STDIN_FILENO);

        if(*num_arg==0) error_handler(missing_program, NULL, NULL);
        error_check(execu, *num_arg, if_pipe, pipe_pos!=-1, file_in[0] != '\0', file_out[0] != '\0');
        for(int i=0;i<*num_arg;i++) elim_quo(execu[i]);

        if(*num_arg == 1 && strcmp(execu[0], "pwd")==0) {
            process_pwd();
            exit(0);
        } else if (*num_arg == 1 && strcmp(execu[0], "exit")==0) {
            exit(255);
        }  else {
            int check = execvp(execu[0], execu);
            if (check == -1) error_handler(command_error, execu[0], NULL);
        }

    } else if (pid < 0) {
        error_handler(fork_error, NULL, NULL);
    } else {
        if(if_bg==1){
            child[num_bg-1]++;
            if(child[num_bg-1] == 1) record[num_bg-1] = malloc(sizeof(pid_t));
            else record[num_bg-1] = realloc(record[num_bg-1], sizeof(pid_t)*(unsigned)child[num_bg-1]);
            record[num_bg-1][child[num_bg-1]-1] = pid;
        }
        if (pipe_pos!=-1){
            close(fd[1]);
            adjust_parser(execu, 0, pipe_pos+1, num_arg);
            execute_process(execu, num_arg, fd[0], 1, if_bg, record, num_bg, child, last_dir);
        }

        int status = 0;
        if(if_bg==0) waitpid(pid, &status, 0);
        if(status >> 8 == 255){
            printf("exit\n");
            for(int i =0;i<*num_arg;i++) free(execu[i]);
            for(int i =0;i<num_bg;i++) free(record[i]);
            if(last_dir!=NULL) free(last_dir);
            exit(0);
        }
    }
}

void mumsh(void){
    int num_bg = 0;
    int if_bg = 0;
    char input[2048];
    char buffer[1024];
    char* last_dir = NULL;
    pid_t* record_bg[50];
    int child_num[50] = {0};
    while (1) {
        char* parser[50];
        signal(SIGINT, INThandler_parent);
        if(if_bg == 1){
            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';
            }
            printf("[%d] %s\n", num_bg, buffer);
        }
        printf("mumsh $ ");
        int complete_flag = -1;
        command_empty = 1;

        bzero(buffer,1024);
        while(complete_flag!=0){
            if(command_empty==0) printf("> ");
            if(!fgets(buffer, 1024, stdin)){
                printf("exit\n");
                for(int i=0;i<num_bg;i++) free(record_bg[i]);
                if(last_dir != NULL) free(last_dir);
                exit(0);
            }
            if(command_empty == 1) bzero(input,2048);
            command_empty = 0;
            strcat(input, buffer);
            complete_flag = check_completeness(input);
            if(complete_flag == 3 && input[strlen(input)-1] == '\n') input[strlen(input)-1] = ' ';
            bzero(buffer,1024);
        }

        signal(SIGINT, SIG_IGN);
        strcpy(buffer, input);
        pre_process(input);
        int num_arg = parse_input(input, parser);

        if(num_arg>=1){
            elim_quo(parser[0]);
            if_bg  = check_bg(parser, &num_arg);
        }

        if(if_bg == 1){
            num_bg++;
            bzero(buffer_bg[num_bg-1], 1024);
            strcpy(buffer_bg[num_bg-1], buffer);
        }
        if (num_arg == 1 && strcmp(parser[0], "exit") == 0) {
            printf("exit\n");
            for(int i =0;i<num_arg;i++) free(parser[i]);
            break;
        } else if(num_arg >=1 && strcmp(parser[0], "cd") == 0) {
            process_cd(num_arg, parser, &last_dir);
        } else if (num_arg == 1 && strcmp(parser[0], "jobs")==0) {
            job(record_bg, num_bg, child_num);
        } else if (num_arg >= 1) {
            execute_process(parser, &num_arg, -1, 0, if_bg, record_bg, num_bg, child_num, last_dir);
        }
        for(int i =0;i<num_arg;i++) free(parser[i]);
    }
    for(int i=0;i<num_bg;i++) free(record_bg[i]);
    if(last_dir != NULL) free(last_dir);
}
