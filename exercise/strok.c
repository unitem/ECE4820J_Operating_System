#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char input[1024];
    fgets(input, 1024, stdin);
    for (char *temp = strtok(input, " \n"); temp != NULL; temp = strtok(NULL, " \n"))
    {
        printf("%s\n", temp);
    }
}