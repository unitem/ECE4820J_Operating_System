#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char command[256];

    while (1) {
        printf("Enter a command (or 'exit' to quit): ");
        fgets(command, sizeof(command), stdin);

        // Remove newline character at the end of the input
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Fork a new process to execute the command
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            int result = system(command);
            if (result == -1) {
                perror("system");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 0) {
                    printf("Command executed successfully.\n");
                } else {
                    fprintf(stderr, "Command exited with status %d\n", exit_status);
                }
            } else {
                fprintf(stderr, "Command did not exit normally.\n");
            }
        }
    }

    printf("Exiting the shell.\n");
    return 0;
}
