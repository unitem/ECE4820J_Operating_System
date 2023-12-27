# Non-programming exercise.

## Ex.1

1. Various problems:

1. Inconsistency in thread management data structures: If a thread is interrupted while it's modifying thread-related data structures, these structures can become inconsistent, leading to unpredictable behavior when the thread resumes.
   
2. Loss of time slice: The operating system's scheduler allocates CPU time slices to processes. If a thread is interrupted while in the run-time system, the time slice meant for the actual computation might be consumed in thread management tasks. This can degrade performance.

3. Nested scheduling: If the runtime system is scheduling threads, and it gets interrupted by the system's clock interrupt, the operating system may end up scheduling a different process, causing nested or redundant scheduling decisions.

Solution: Before performing thread management tasks (or entering critical sections), the run-time system can disable clock interrupts. Once the critical section is exited or the thread management task is complete, the clock interrupts can be re-enabled. 

2. The `select` system call allows a program to monitor multiple file descriptors to see if they are ready for reading, writing, or have an error condition.  It is possible to implement a threads package in user space using the provided alarm mechanism to handle potentially blocking system calls. The alarm interrupts can facilitate context switching between threads. However, there would be challenges related to performance, efficiency, and complexity.

# Ex.2

I have wrote it in `ex2.sh`. The code is also listed below:

```bash
#!/bin/bash

# Specify the file name
FILE="numbers.txt"

# Check if the file exists
if [[ ! -f $FILE ]]; then
    # If the file doesn't exist, create it with the number 1
    echo "1" > $FILE
else
    # Read the last number in the file
    LAST_NUMBER=$(tail -n 1 $FILE)
    
    # Increment the last number
    NEXT_NUMBER=$((LAST_NUMBER + 1))
    
    # Append the result to the file
    echo $NEXT_NUMBER >> $FILE
fi

```

1. By running the command below, I found that the first one is over after just one iteration, and the second one runs forever.

```bash
while true; do ./ex2.sh; done &
while true; do ./ex2.sh; done
```
2. The code is modified and be put in ex2_fix.sh:

```bash
#!/bin/bash

FILE="numbers.txt"
LOCKFILE="${FILE}.lock"

(
    # Wait for lock on the lock file (file descriptor 200). 
    # This ensures that only one instance of the script can modify numbers.txt at a time.
    flock -e 200
    
    # Check if the file exists
    if [[ ! -f $FILE ]]; then
        # If the file doesn't exist, create it with the number 1
        echo "1" > $FILE
    else
        # Read the last number in the file
        LAST_NUMBER=$(tail -n 1 $FILE)
        
        # Increment the last number
        NEXT_NUMBER=$((LAST_NUMBER + 1))
        
        # Append the result to the file
        echo $NEXT_NUMBER >> $FILE
    fi

) 200>$LOCKFILE

```
Run the following code to test:
```bash
./ex2_driver.sh
```

## Ex3

See `cthread.c`.