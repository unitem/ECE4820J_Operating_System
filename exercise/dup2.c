#include<stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // file control options
#include <unistd.h>     // UNIX Standard
#include <signal.h>     // signal handling
#include <errno.h>      // error number
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   
#include <sys/stat.h>
#include <pwd.h>        // current directory

int main() {
    int fd = open("1.txt", O_RDWR | O_CREAT, 0666);
    dup2(fd, STDOUT_FILENO);
    printf("hello world\n");
    close(fd);
    printf("hello world\n");
    return 0;
}