#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int myrand(){
    return rand();
}

int main()
{
    srand(time(0));
    for (int i=0;i<10;i++){
        // printf("%d\n",rand());
        printf("%d\n",myrand());
    }
    return 0;
}

