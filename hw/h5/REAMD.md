# Non-programming exercises.

# Simple questions

1. No. At lease one process can get two resources so it can be finished. 
2. n: 1, 2, 3, 4, 5 are OK.
3. 35/50 + 20/100 + 10/200 + x/250 < 1: x<12.5
4. It will be allocated with more CPU time. Maybe it's because it is of higher priority or more CPU time will bring better performance.
5. For source code, the extensive use of CPU-intensive operations like loops, calculations, and data processing, algorithms with high computational complexity, and minimal interaction with I/O devices or external resources indicates CPU-bound. Frequent file operations (read/write), database interactions, or network communication indicates I/O-bound. At runtime, we can use system log or monitoring tools like `perf` to determine it.

# Deadlocks
1. Requested Matrix:
```text
743
122
600
11
431
```
2. Yes, since p2 or p4 can still run.
3. Yes.Order: p4, p2, p5, p3, p1.

# read-write

1. read-lock:
```C
void READ_lock() {
    down(count_lock);
    count++;
    if(count==1) down(db_lock);
    up(count_lock);
}

void READ_unlock() {
    down(count_lock);
    count--;
    if(count==0) up(db_lock);
    up(count_lock);
}
```
2. They will try to increase `count`. Once `count` is available, they will call `READ_lock`. Since the `READ_lock` will up the `count_lock` at last, they can read at the same time.
3. Implementation is below.
```C
void write_lock() {
    down(read_lock);
    down(db_lock);
}

void write_unlock() {
    up(read_lock);
    up(db_lock);
}

void READ_lock() {
    down(read_lock);
    down(count_lock);
    count++;
    if(count==1) down(db_lock);
    up(count_lock);
    up(read_lock);
}

void READ_unlock() {\
    // Since it only blocks new reader, the unlock does not need to touch read_lock
    down(count_lock);
    count--;
    if(count==0) up(db_lock);
    up(count_lock);
}
```
4. Yes. Write is given higher priority than read. Thus it cannot be considered as solved.
