#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    char input[256];
    char *command, *arg;
    char *args[256];
    int num_args = 0;

    while (1) {
        printf("Enter a command: ");
        fgets(input, sizeof(input), stdin);

        // Remove newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the input into command and arguments
        command = strtok(input, " ");
        num_args = 0;
        while ((arg = strtok(NULL, " ")) != NULL) {
            args[num_args++] = arg;
        }
        args[num_args] = NULL;

        if (command == NULL) {
            continue; // Empty command, ignore
        }

        // Check if the command exists in the system
        if (access(command, X_OK) == -1) {
            fprintf(stderr, "Error: Command '%s' not found.\n", command);
            continue;
        }

        // Fork a new process to execute the command
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            execvp(command, args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0) {
                    fprintf(stderr, "Command exited with status %d\n", exit_status);
                }
            } else {
                fprintf(stderr, "Command did not exit normally.\n");
            }
        }
    }

    return 0;
}
