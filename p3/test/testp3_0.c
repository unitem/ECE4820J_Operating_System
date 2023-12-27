#include "time.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    pid_t pid[10];
    int i;
    nice(-15);
    printf("Test start\n");

    for (i = 0; i < 10; i++)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            // Child process code
            int iterations = 1000000000;
            int result = 0;

            nice(15 - 3 * i);

            // Measure time before computation
            int begin, end, runningTime;
            begin = time(NULL);

            // Perform a simple mathematical calculation
            for (int j = 1; j <= iterations; j++)
            {
                result += j * j;
            }

            // Measure time after computation
            end = time(NULL);
            runningTime = (end - begin);

            printf("Child %d: Result = %d, Time = %d seconds\n", i, result, runningTime);
            return 0;
        }
    }

    for (i = 0; i < 10; i++)
    {
        waitpid(pid[i], NULL, 0);
    }

    printf("All child processes have finished.\n");
    return 0;
}
