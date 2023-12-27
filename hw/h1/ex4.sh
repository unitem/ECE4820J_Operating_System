#!usr/bin/awk -f
# ex4
# ex4.1
useradd myname
passwd myname
# ex4.2
ps ax
# ex4.3
cat /proc/cpuinfo
cat /proc/meminfo
# ex4.4
# Here "echo 1" stands for some random output
echo 1 | tee 1.txt 2.txt
# ex4.5
cat 1.txt 2.txt > 3.txt
# ex4.6
od -x 3.txt
# ex4.7
find /usr/src -type f -name '*semaphore*' -exec grep -l 'ddekit_sem_down' {} \;
# /usr/src/include/ddekit/semaphore.h
# /usr/src/lib/libddekit/src/semaphore.c