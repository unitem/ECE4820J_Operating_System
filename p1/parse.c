#include "parse.h"

bool inputRedirectionFlag = false, outputRedirectionFlag = false, outputAppendFlag = false, pipeFlag = false, in_pipe = false, out_pipe = false;
int inputRedirectionIndex = -1, outputRedirectionIndex = -1, outputAppendIndex = -1, pipeIndex = -1;

int fd = -2;

//---------------------------------------- reset_flags ----------------------------------------

void reset_flags(void)
{
    inputRedirectionFlag = false;
    outputRedirectionFlag = false;
    outputAppendFlag = false;
    pipeFlag = false;
    inputRedirectionIndex = -1;
    outputRedirectionIndex = -1;
    outputAppendIndex = -1;
    pipeIndex = -1;
}

//---------------------------------------- addWithSpace ----------------------------------------

void addWithSpace(DynamicStringArray *args, char *temp)
{
    for (int i = 0; i < (int)strlen(temp); i++)
    {
        if (temp[i] == '<' || temp[i] == '>' || temp[i] == '|')
        {
            if (temp[i] == '>' && (temp[i + 1] == '>'))
            {
                char tmp[3] = {temp[i], temp[i], '\0'}; // ">>" will not work
                temp[i] = '\0';
                if (strlen(temp) != 0)
                {
                    addDSA(args, temp);
                }
                addDSA(args, tmp);
                temp = temp + i + 2;
                i = -1;
            }
            else
            {
                char tmp[2] = {temp[i], '\0'};
                temp[i] = '\0';
                if (strlen(temp) != 0)
                {
                    addDSA(args, temp);
                }
                addDSA(args, tmp);
                temp = temp + i + 1;
                i = -1;
            }
        }
    }
    if (strlen(temp) != 0)
    {
        addDSA(args, temp);
    }
}

//---------------------------------------- remove redirection with files ----------------------------------------

void removeOutputRedirection(DynamicStringArray *args)
{
    if (outputRedirectionFlag)
    {
        removeDSA(args, (size_t)outputRedirectionIndex);
        removeDSA(args, (size_t)outputRedirectionIndex);
    }
}
void removeOutputAppend(DynamicStringArray *args)
{
    if (outputAppendFlag)
    {
        removeDSA(args, (size_t)outputAppendIndex);
        removeDSA(args, (size_t)outputAppendIndex);
    }
}
void removeInputRedirection(DynamicStringArray *args)
{
    if (inputRedirectionFlag)
    {
        removeDSA(args, (size_t)inputRedirectionIndex);
        removeDSA(args, (size_t)inputRedirectionIndex);
    }
}

void delRedirection(DynamicStringArray *args)
// EFFECTS: delete the redirection and pipe symbols
{
    // Ignore pipe for now. Todo: add pipe support
    if (outputAppendFlag && inputRedirectionFlag)
    {
        if (outputAppendIndex > inputRedirectionIndex)
        {
            removeOutputAppend(args);
            removeInputRedirection(args);
        }
        else
        {
            removeInputRedirection(args);
            removeOutputAppend(args);
        }
    }
    else if (outputRedirectionFlag && inputRedirectionFlag)
    {
        if (outputRedirectionIndex > inputRedirectionIndex)
        {
            removeOutputRedirection(args);
            removeInputRedirection(args);
        }
        else
        {
            removeInputRedirection(args);
            removeOutputRedirection(args);
        }
    }
    else
    {
        removeOutputRedirection(args);
        removeOutputAppend(args);
        removeInputRedirection(args);
    }

    if (pipeFlag)
    {
        for (size_t i = 0; i < (size_t)pipeIndex + 1; i++)
        {
            removeDSA(args, 0);
        }
    }
}

//---------------------------------------- parseArgs ----------------------------------------
// Parse the input args
// Completed: redirecting
int parseArgs(DynamicStringArray *args)
{
    for (size_t i = 0; i < args->size - 1; ++i)
    {
        char *arg = args->data[i];
        if (arg == NULL)
        {
            break;
        }

        // Redirection and pipe handling
        if (inputRedirectionFlag && (int)i == inputRedirectionIndex + 1)
        {
            // Open the file
            if (strcmp(arg, "<") == 0 || strcmp(arg, ">") == 0 || strcmp(arg, "|") == 0)
            {
                syntax_near_error(arg);
                return -2;
            }
            fd = open(arg, O_RDONLY);
            if (fd == -1)
            {
                if (errno == ENOENT)
                {
                    file_nonexist_error(arg);
                    return -2;
                }
                else if (errno == EACCES)
                {
                    permission_error(arg);
                    return -2;
                }
                else
                {
                    perror("open");
                    return -1;
                }
            }
            // Redirect stdin
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                return -1;
            }
            // close the file
            if (close(fd) == -1)
            {
                perror("close");
                return -1;
            }
            continue;
        }
        else if (outputRedirectionFlag && (int)i == outputRedirectionIndex + 1)
        {
            // Open the file
            if (strcmp(arg, "<") == 0 || strcmp(arg, ">") == 0 || strcmp(arg, "|") == 0)
            {
                syntax_near_error(arg);
                return -2;
            }
            fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd == -1)
            {
                if (errno == ENOENT)
                {
                    file_nonexist_error(arg);
                    return -2;
                }
                else if (errno == EACCES)
                {
                    permission_error(arg);
                    return -2;
                }
                else
                {
                    perror("open");
                    return -1;
                }
            }
            // Redirect stdout
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                return -1;
            }
            // close the file
            if (close(fd) == -1)
            {
                perror("close");
                return -1;
            }
            continue;
        }
        else if (outputAppendFlag && (int)i == outputAppendIndex + 1)
        {
            // Open the file
            if (strcmp(arg, "<") == 0 || strcmp(arg, ">") == 0 || strcmp(arg, "|") == 0)
            {
                syntax_near_error(arg);
                return -2;
            }
            fd = open(arg, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd == -1)
            {
                perror("open");
                return -1;
            }
            // Redirect stdout
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                return -1;
            }
            // close the file
            if (close(fd) == -1)
            {
                perror("close");
                return -1;
            }
            continue;
        }
        else if (pipeFlag)
        {

            if (strcmp(arg, "<") == 0 || strcmp(arg, ">") == 0 || strcmp(arg, "|") == 0)
            {
                syntax_near_error(arg);
                return -2;
            }
            return PIPE_STATUS;
        }

        // Redirection and pipe detection
        if (strcmp(arg, ">") == 0)
        {
            // check if the next arg is legal
            if (i == args->size - 2)
            {
                syntax_near_error("newline");
                return -2;
            }
            else if (outputRedirectionFlag || outputAppendFlag || in_pipe)
            {
                dup_out_red_error();
                return -2;
            }
            else
            {
                outputRedirectionFlag = true;
                outputRedirectionIndex = (int)i;
                continue;
            }
        }
        else if (strcmp(arg, ">>") == 0)
        {
            // check if the next arg is legal
            if (i == args->size - 2)
            {
                syntax_near_error("newline");
                return -2;
            }
            else if (outputRedirectionFlag || outputAppendFlag || in_pipe)
            {
                dup_out_red_error();
                return -2;
            }
            else
            {
                outputAppendFlag = true;
                outputAppendIndex = (int)i;
                continue;
            }
        }
        else if (strcmp(arg, "<") == 0)
        {
            // check if the next arg is legal
            if (i == args->size - 2)
            {
                syntax_near_error("newline");
                return -2;
            }
            else if (inputRedirectionFlag || out_pipe)
            {
                dup_in_red_error();
                return -2;
            }
            else
            {
                inputRedirectionFlag = true;
                inputRedirectionIndex = (int)i;
                continue;
            }
        }
        else if (strcmp(arg, "|") == 0)
        {
            if (i == args->size - 2)
            {
                syntax_near_error("newline");
                return -2;
            }
            if (i == 0)
            {
                miss_prog_error();
                return -2;
            }
            else
            {
                pipeFlag = true;
                pipeIndex = (int)i;
                return PIPE_STATUS;
            }
        }
    }
    return 0;
}
