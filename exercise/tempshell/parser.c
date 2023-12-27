#include <string.h>
#include <stdlib.h>
#include "p1.h"

void elim_helper(char* string_t, int i){
    int length = (int) strlen(string_t);
    for (int j = i; j < length-1; j++) string_t[j] = string_t[j+1];
    string_t[length-1] = '\0';
}

void elim_quo(char* string_t){
    int check = 0;
    int i = 0;
    while(i<(int)strlen(string_t)){
        if(string_t[i]== '\"'){
            if(check == 0){
                check = 1;
                elim_helper(string_t, i);
            } else if (check == 1){
                check = 0;
                elim_helper(string_t, i);
            } else {
                i++;
            }
        } else if (string_t[i]== '\''){
            if(check == 0){
                check = 2;
                elim_helper(string_t, i);
            } else if (check == 2){
                check = 0;
                elim_helper(string_t, i);
            } else {
                i++;
            }
        } else{
            i++;
        }
    }
}

int parse_input(char input[], char* execu[]) {
    if (strcmp(&input[strlen(input) - 1], "\n") == 0) {
        input[strlen(input) - 1] = '\0';
    }
    if (strlen(input) == 0) return 0; // if there's no input

    int num_of_sec = 0;
    int ignore_flag = 0;
    int record = -1;
    for(int i=0;i<(int)strlen(input);i++){
        if (input[i] == '\"'){
            if (ignore_flag == 0) ignore_flag = 1;
            else if (ignore_flag == 1) ignore_flag = 0;
        } else if (input[i] == '\'') {
            if (ignore_flag == 0) ignore_flag = 2;
            else if (ignore_flag == 2) ignore_flag = 0;
        }

        if(input[i] == ' ' || i == (int)strlen(input)-1){
            if (i-record==1 && i != (int)strlen(input)-1) {
                record = i;
            } else {
                if (i-record==1 && i == (int)strlen(input)-1 && input[i] == ' ') break;
                if(ignore_flag == 0){
                    unsigned int length = 0;
                    if (i== (int)strlen(input)-1 && input[i] != ' ') length = (unsigned) (i-record);
                    else length = (unsigned)(i-record-1);

                    execu[num_of_sec] = (char*) malloc(length+1);
                    bzero(execu[num_of_sec],length+1);
                    strncpy(execu[num_of_sec],input+record+1,length);
                    execu[num_of_sec][length] = '\0';
                    num_of_sec++;
                    record = i;
                }
            }
        }
    }
    execu[num_of_sec] = NULL;

    return num_of_sec;
}

void adjust_parser(char **parser, int begin, int end, int *num) {
    int step = end - begin;
    for(int i=begin;i<end;i++) free(parser[i]);

    for (int i = begin; i < *num - step; i++) {
        parser[i] = parser[i + step];
    }
    for (int i = *num - step; i < *num; i++) {
        parser[i] = NULL;
    }
    *num = *num - step;
}

int check_redirect_symbol(const char *input, int idx) {
    if (input[idx] == '<' || input[idx] == '|' || input[idx] == '&') {
        return 1;
    } else if (input[idx] == '>') {
        if (idx + 1 < (int) strlen(input) && input[idx + 1] == '>') return 2;
        else return 1;
    } else {
        return 0;
    }
}

void pre_process(char* input){
    int i = 0;
    int ignore_flag = 0;
    while (i < (int) strlen(input)) {
        if (input[i] == '\"'){
            if (ignore_flag == 0) ignore_flag = 1;
            else if (ignore_flag == 1) ignore_flag = 0;
            i++;
            continue;
        } else if (input[i] == '\'') {
            if (ignore_flag == 0) ignore_flag = 2;
            else if (ignore_flag == 2) ignore_flag = 0;
            i++;
            continue;
        }

        if(ignore_flag == 0){
            int result = check_redirect_symbol(input, i);
            if (result != 0) {
                // add space before redirection symbol
                int length = (int) strlen(input);
                for (int j = length - 1; j > i - 1; j--) input[j + 1] = input[j];
                input[i] = ' ';
                input[length + 1] = '\0';
                i++;

                // add space after redirection symbol
                if (result == 2) i++;
                length = (int) strlen(input);
                for (int j = length - 1; j > i; j--) input[j + 1] = input[j];
                input[i + 1] = ' ';
                input[length + 1] = '\0';

            }
        }
        i++;
    }
}

int check_bg(char** parser, int *num_arg){
    if(strcmp(parser[*num_arg-1], "&")==0){
        adjust_parser(parser, *num_arg-1, *num_arg, num_arg);
        return 1;
    } else {
        return 0;
    }
}

int check_completeness(char* input){
    int flag = 0;
    for(int i=0;i<(int)strlen(input);i++){
        if(flag == 3 && (input[i] == '|' || input[i] == '>' || input[i] == '<')) return 0;

        if(input[i]!=' ' && input[i]!='\n' && flag == 3) flag = 0;

        if (input[i] == '\"'){
            if (flag == 0) flag = 1;
            else if (flag == 1) flag = 0;
        } else if (input[i] == '\'') {
            if (flag == 0) flag = 2;
            else if (flag == 2) flag = 0;
        } else if (flag == 0 && (input[i] == '|' || input[i] == '>' || input[i] == '<')) {
            flag = 3;
        }
    }
    return flag;
}
