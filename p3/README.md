# This is project 3 - Modifying Minix 3 source code by p3team-02

## Team members

Weiqing Xu 520021910400
Sizhe Sun 521370910097

## Milestone 1

For this milestone, the goal is to implement lottery scheduling.

## 1.1 Process

1. process struct location: p3/p3team-02/kernel/proc.h
2. The default kernel policy is relatively simple and is applied only when the process' `p_scheduler` pointer is set to NULL.
```C
struct proc *p_scheduler;	/* who should get out of quantum msg */
/* test whether the process is scheduled by the kernel's default policy  */
#define proc_kernel_scheduler(p)	((p)->p_scheduler == NULL || \
					(p)->p_scheduler == (p))
```
3. Then it will call `static void notify_scheduler(struct proc *p)` in p3/p3team-02/kernel/proc.c
4. `m_no_quantum` is sent
5. `mini_send` using ipc `p3/p3team-02/kernel/ipc.h`

## 1.2 Where to modify

1. The main structure in `proc.c` can be maintained
2. `pick_proc` in `proc.c` is the function to pick the next process to run
3. Also, `enqueue`, `enqueue_head`, `dequeue` should be modified a little bit.

## 1.3 Strategies

1. The priority of the process is from 0 to 15.
2. The process of priority i can get 2^(15-i) tickets.
3. Randomly pick a ticket and find the process with the ticket.

## 1.4 Test cases

1. We provide a test case `/test/testp3_0.c`. To compile and run it, use the following commands (after compile and build the kernel):
```bash
cd /usr/src/test
clang -o testp3_0 testp3_0.c
./testp3_0
```

In the test case, we create n child processes with different priorities using nice(), and run them to check the order executed and the time needed for each child process.

In the original Minix 3, the output is:
```
Test start
Child 0: Result = 422847872, Time = 0.183333 seconds
Child 1: Result = 422847872, Time = 0.183333 seconds
Child 2: Result = 422847872, Time = 0.166667 seconds
Child 3: Result = 422847872, Time = 0.183333 seconds
Child 4: Result = 422847872, Time = 0.166667 seconds
All child processes have finished.
```
(to be revised since test case changed)

In our implementation, the output is:
```bash
Test start
Child 6: Result = 720795392, Time = 8 seconds
Child 5: Result = 720795392, Time = 11 seconds
Child 7: Result = 720795392, Time = 10 seconds
Child 8: Result = 720795392, Time = 10 seconds
Child 9: Result = 720795392, Time = 9 seconds
Child 4: Result = 720795392, Time = 15 seconds
Child 3: Result = 720795392, Time = 15 seconds
Child 2: Result = 720795392, Time = 9 seconds
Child 1: Result = 720795392, Time = 6 seconds
Child 0: Result = 720795392, Time = 4 seconds
All child processes have finished.
```

We can see that in the original source, although the processes have different priority, their finishing time is almost the same and it's in a order. But in the lottery scheduling, the processes with higher priority (bigger child numbers, lower nice value) will finish earlier and they are in random, not in order.
