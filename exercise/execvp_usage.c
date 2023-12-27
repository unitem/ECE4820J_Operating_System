#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char **newargv = (char **) malloc(sizeof(char *) * (size_t)(argc + 1));
    for(int i = 0; i < argc; i++) {
        newargv[i] = argv[i + 1];
    }
    newargv[argc] = NULL;
    printf("Executing %s...\n", argv[1]);
    int a = execvp(argv[1], newargv);
    printf("%d\n", a);
    if (errno == ENOENT) {
        printf("No file or directory matches the specified pathname.\n");
    } else if (errno == EACCES) {
        printf("Permission denied, or the file isn't an executable.\n");
    } else {
        printf("Error executing command.\n");
    }
    execvp("./hello", newargv);
    // perror("execvp");
    // exit(EXIT_FAILURE);
}