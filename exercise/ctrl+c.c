#include<stdio.h>
#include<signal.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile int keepRunning = 1;

void reset(void) {
    printf("\n Resetting...\n");
}

void intHandler(int dummy) {
    reset();
    keepRunning = 0;
}

int main() {
    signal(SIGINT, intHandler);

    while(keepRunning) {
        printf("Hello World!\n");
        sleep(1);
    }
}