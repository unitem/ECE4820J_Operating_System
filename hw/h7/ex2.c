#include <unistd.h>
#include <stdio.h>
#include "mproc.h"
/* Global error number set for failed system calls. */
#define OK 0

int getnchpid(int n, pid_t *childpid) {
    register struct mproc *rmc; /* pointer to child */
    if (childpid == NULL) return -1;
    if (n > NR_PROCS) return -1;
    rmc = &mproc[n];
    if (rmc->mp_parent != who_p) return -1;
    *childpid = rmc->mp_pid;
    return OK;
}

int getchpids(int n, pid_t *childpid) {
    int i;
    for (i = 0; i < n; i++) {
        if (getnchpid(i, childpid + i) != OK) return -1;
    }
    return i;
}

int main() {
    pid_t childpid[10];
    int childNum = 10;
    pid_t childpidRef[10];
    
    for(int i = 0; i < childNum; i++){
        pid_t pid = fork();
        if (pid == 0) break;
        childpidRef[i] = pid;
    }
    
    getchpids(childNum, childpid);
    
    for(int i = 0; i < childNum; i++) {
        if(childpidRef[i] != childpid[i]) {
            printf("%dth pid is wrong!",i);
            return -1;
        }
    }

    return 0;
}
```