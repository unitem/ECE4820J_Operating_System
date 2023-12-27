#ifndef PARSE_H
#define PARSE_H

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "argslist.h"
#include "basic_io.h"
#define PIPE_STATUS 2

void addWithSpace(DynamicStringArray *args, char *temp);
// add the string to args, and split the string if it contains '<', '>', '|'
int parseArgs(DynamicStringArray *args);
// Parse the arguments and store them in args.
// return -2 when meet syntax error, -1 when system call error, PIPE_STATUS when pipe detected, 0 otherwise.
extern int fd;
extern bool inputRedirectionFlag, outputRedirectionFlag, outputAppendFlag, pipeFlag, in_pipe, out_pipe;
extern int inputRedirectionIndex, outputRedirectionIndex, outputAppendIndex, pipeIndex;
void reset_flags(void);
void delRedirection(DynamicStringArray *args);


#endif
