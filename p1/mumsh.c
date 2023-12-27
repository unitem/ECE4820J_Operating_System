#include "mumsh.h"

int killSignal = 0;

int running_process[MAX_INPUT_SIZE];
int running_process_size = 0;
bool doublePrompt = false;


void inthandler_child(int sig){
    if (sig != SIGINT) return;
    // fprintf(stderr, "Kill signal %d captured.\n", sig); // for debug
    fflush(stderr); // for debug
    exit(0);
}

void inthandler_parent(int sig){
    if (sig != SIGINT) return;
    prompt("\nmumsh $ ");
    doublePrompt = true;
}

/*
void inthandler(int sig)
{
    if (sig != SIGINT) return;
    killSignal = 1;
    // fprintf(stderr, "Kill signal %d captured.\n", sig); // for debug
    // fflush(stderr); // for debug
    printf("\n");
    // printf("\nmumsh $ ");
    fflush(stdout);
    for (int i = running_process_size - 1; i >= 0; i--)
    {
        fprintf(stderr, "Kill process %d.\n", running_process[i]); // for debug
        fflush(stderr); // for debug
        if (running_process[i] != 0)
            kill(running_process[i], SIGKILL);
        running_process[i] = 0;
        running_process_size--;
    }
    killSignal = 0;
}
*/

int read_c(char *input)
// EFFECTS: read the input from input
{
    if (!doublePrompt)
    {
        prompt("mumsh $ ");
        doublePrompt = false;
    }
    // read the input
    // fprintf(stderr, "waiting for input..."); // for debug
    // fflush(stderr); // for debug
    // fgets(input, MAX_INPUT_SIZE, stdin);
    int i = 0;
    while (true)
    {
        int c = getchar();
        // fprintf(stderr, "c: %d\n", c); // for debug
        // fflush(stderr); // for debug
        if (c == EOF)
        {
            if (feof(stdin)) {
                if (i == 0)
                {
                    prompt("exit\n");
                    exit(0);
                }
                else
                {
                    continue;
                }
            }
            else if (ferror(stdin))
            {
                return -1;
            }
                }
        if (c == '\n')
        {
            input[i] = '\0';
            break;
        }
        input[i] = (char)c;
        ++i;
    }

    if (strlen(input) == 0)
    {
        return -1;
    }
    return 0;
}

int parse_c(char *input, DynamicStringArray *args, int stdout_fd)
// EFFECTS: parse the input and store the result in args
// return 0 if the command is executed successfully, to be executed
// return -1 if the command is not executed, to be ignored
{
    // parse the input
    initDSA(args, 2);
    for (char *temp = strtok(input, " \n"); temp != NULL; temp = strtok(NULL, " \n"))
    {
        addWithSpace(args, temp);
    }
    // printDSA(args); // for debug
    // return -1; // for debug
    // check if enter is pressed
    if (args->size == 0)
    {
        return -1;
    }
    addDSA(args, NULL);
    while (1)
    {
        int ans = parseArgs(args);
        if (ans < 0)
        {
            freeDSA(args);
            return -1;
        }
        else if (ans == PIPE_STATUS)
        {
            // pipeline file descriptor
            int pipefd[2];
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                return -1;
            }

            args->data[pipeIndex] = NULL;
            pid_t pid = fork();
            running_process[running_process_size++] = pid;
            // fprintf(stderr, "Process generated for command before pipe: %d\n", pid); // for debug
            // fflush(stderr); // for debug

            if (pid == -1)
            {
                perror("fork");
                return -1;
            }
            else if (pid == 0)
            {
                // Child process
                signal(SIGINT, inthandler_child);
                // Redirect stdout to the pipe
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    return -1;
                }
                // Close the pipe
                if (close(pipefd[0]) == -1)
                {
                    perror("close");
                    return -1;
                }
                // Execute the command
                reset_flags();
                in_pipe = true;
                continue;
            }
            else
            {
                // Parent process
                // Redirect stdin to the pipe
                if (dup2(pipefd[0], STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    return -1;
                }
                // Close the pipe
                if (close(pipefd[1]) == -1)
                {
                    perror("close");
                    return -1;
                }
                // Wait for the child process
                int child_status;
                if (waitpid(pid, &child_status, WNOHANG) == -1) // WNOHANG: return immediately if no child has exited
                {
                    perror("waitpid");
                    return -1;
                }
                if (WEXITSTATUS(child_status) != 0)
                {
                    // fprintf(stderr, "Command exited with status %d\n", child_status);
                    freeDSA(args);
                    return -1;
                }
                // Execute the command after the pipe
                // fprintf(stderr, "The first command executed.\n"); // for debug
                // fflush(stderr); // for debug
                in_pipe = false;
                out_pipe = true;
                dup2(stdout_fd, STDOUT_FILENO);
                deleteFrontDSA(args, (size_t)pipeIndex + 1);
                reset_flags();
                continue;
            }
        }
        else
        {
            break;
        }
    }

    // form new args
    delRedirection(args);
    reset_flags();
    return 0;
}

int execute_c(DynamicStringArray *args)
// EFFECTS: execute the command
{
    // exit commands
    const char *cmd = args->data[0];

    // fprintf(stderr, "Command: %s\n", cmd); // for debug
    // fflush(stderr); // for debug

    if (cmd == NULL)
    {
        miss_prog_error();
        return 1;
    }
    if (strcmp(cmd, "exit") == 0)
    {
        prompt("exit\n");
        return 0;
    }
    // pwd
    if (strcmp(cmd, "pwd") == 0)
    {
        mypwd();
        return 1;
    }
    if (strcmp(cmd, "cd") == 0)
    {
        mycd(args);
        return 1;
    }

    // execute the command
    // Fork a new process
    pid_t pid = fork();
    running_process[running_process_size++] = pid;
    // fprintf(stderr, "Process generated: %d\n", pid); // for debug
    // fflush(stderr); // for debug

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        signal(SIGINT, inthandler_child);
        // fprintf(stderr, "Child process: %d\n", getpid()); // for debug
        // printDSA(args); // for debug
        // fflush(stderr); // for debug
        int status = execvp(cmd, args->data);
        signal(SIGINT, inthandler_child);
        if (status == -1)
        {
            cmd_error(cmd);
            exit(EXIT_FAILURE);
        }
        // fprintf(stderr, "Exit from the execution.\n"); // for debug
        // fflush(stderr); // for debug
        exit(0);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (in_pipe)
        {
            // fprintf(stderr, "Exit from the first command.\n"); // for debug
            // fflush(stderr); // for debug
            exit(WEXITSTATUS(status));
        }
    }
    return 1;
}

void loop(void)
// EFFECTS: run the loop
{
    // fprintf(stderr, "Current process: %d\n", getpid()); // for debug
    // fflush(stderr); // for debug

    // define the variables
    DynamicStringArray DSA = {NULL, 0, 0};
    DynamicStringArray *args = &DSA;
    char input[MAX_INPUT_SIZE];
    int stdin_fd = dup(STDIN_FILENO);
    int stdout_fd = dup(STDOUT_FILENO);
    int status = 1;
    while (status)
    {
        signal(SIGINT, inthandler_parent);
        memset(input, 0, MAX_INPUT_SIZE);
        killSignal = 0;
        reset_flags();
        in_pipe = false;
        out_pipe = false;
        freeDSA(args);
        int read_status = read_c(input);
        if (read_status < 0)
        {
            // fprintf(stderr, "Error command. Ignored.\n"); // for debug
            // fflush(stderr); // for debug
            status = 1;
            continue;
        }
        int parse_status = parse_c(input, args, stdout_fd);
        
        if (parse_status < 0)
        {
            // fprintf(stderr, "Error command. Ignored.\n"); // for debug
            // fflush(stderr); // for debug
            status = 1;
            dup2(stdout_fd, STDOUT_FILENO);
            dup2(stdin_fd, STDIN_FILENO);
            continue;
        }
        status = execute_c(args);
        dup2(stdout_fd, STDOUT_FILENO);
        dup2(stdin_fd, STDIN_FILENO);
    }
    freeDSA(args);
    exit(0);
}

int main(void)
{
    loop();
    return 0;
}
