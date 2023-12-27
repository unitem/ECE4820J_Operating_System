# Non-programming Exercises

## Ex.1 Simple Questions

1. 
    First fit: 
        (i) 20KB 
        (ii) 10KB
        (iii) 18KB

    Best fit:
        (i) 12KB
        (ii) 10KB
        (iii) 9KB

    Quick fit:
        (i) 12KB
        (ii) 10KB
        (iii) 9KB

2. 10 + n/k (nsec)

3. The four counters are:

    01101110

    01001001
    
    00110111
    
    10001011

## Ex.2 Page tables

1. Inverted Page Tables: there is only a single global page table for all processes. Instead of each entry representing a virtual page (as in traditional page tables), each entry in an inverted page table corresponds to a physical frame in memory. Each entry contains information about which virtual page (and which process) is currently occupying that physical frame. The table size is reduced, and thus the memory overhead is reduced. However, without TLB, the lookup time is still expensive, because the entire table must be searched to find a mapping.
2. Multilevel Page Tables: the page table is broken into multiple smaller page tables. The outer page table contains pointers to the inner page tables. The inner page tables contain the actual mappings. The outer page table is indexed by the most significant bits of the virtual address, and the inner page table is indexed by the least significant bits of the virtual address. The outer page table is much smaller than the inner page tables, so the memory overhead is reduced. The lookup time is also reduced, because the outer page table is much smaller than the inner page tables, so the outer page table can be searched much faster.

## Ex.3

The majority of vulnerabilities in C programming are attributable to buffer overflows and string manipulation issues. Typically, these issues lead to segmentation faults; however, if malicious input is specifically designed to exploit the architecture and environment, it can result in the execution of arbitrary code. Presented below is a compilation of the most prevalent errors in this context, accompanied by recommended remediations and solutions. [1](https://security.web.cern.ch/recommendations/en/codetools/c.shtml)

1. gets

Reason: The stdio gets() function does not check for buffer length and always results in a vulnerability.

Vulnerable code:
```C
#include <stdio.h>
int main () {
    char username[8];
    int allow = 0;
    printf("Enter your username, please: ");
    gets(username); // user inputs "malicious"
    
    // do other things

    return 0;
}
```
Mitigation: Prefer using fgets (and dynamically allocated memory!):
```C
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 8
int main () {
    char* username, *nlptr;
    int allow = 0;
 
    username = malloc(LENGTH * sizeof(*username));
    if (!username)
        return EXIT_FAILURE;
    printf external link("Enter your username, please: ");
    fgets(username,LENGTH, stdin);
    // fgets stops after LENGTH-1 characters or at a newline character, which ever comes first.
    // but it considers \n a valid character, so you might want to remove it:
    nlptr = strchr(username, '\n');
    if (nlptr) *nlptr = '\0';
    
    // do other things
 
    free(username);
    return 0;
}
```
2. strcpy

Reason: The strcpy built-in function does not check buffer lengths and may very well overwrite memory zone contiguous to the intended destination. In fact, the whole family of functions is similarly vulnerable: strcpy, strcat and strcmp.

Vulnerable code:
```C
char str1[10];
char str2[]="abcdefghijklmn";
strcpy(str1,str2);
```
Mitigation
The best way to mitigate this issue is to use strlcpy if it is readily available (which is only the case on BSD systems). However, it is very simple to define it yourself, as shown below:
```C
#include <stdio.h>
 
#ifndef strlcpy
#define strlcpy(dst,src,sz) snprintf((dst), (sz), "%s", (src))
#endif
 
enum { BUFFER_SIZE = 10 };
 
int main() {
    char dst[BUFFER_SIZE];
    char src[] = "abcdefghijk";
 
    int buffer_length = strlcpy(dst, src, BUFFER_SIZE);
 
    if (buffer_length >= BUFFER_SIZE) {
        printf("String too long: %d (%d expected)\n", buffer_length, BUFFER_SIZE-1);
    }
 
    printf("String copied: %s\n", dst);
 
    return 
    0;
}
```
Another and may be slightly less convenient way is to use strncpy, which prevents buffer overflows, but does not guarantee '\0'-termination.
```C
enum { BUFFER_SIZE = 10 };
char str1[BUFFER_SIZE];
char str2[]="abcdefghijklmn";
 
strncpy(str1,str2, BUFFER_SIZE); /* limit number of characters to be copied */
// We need to set the limit to BUFFER_SIZE, so that all characters in the buffer
// are set to '\0'. If the source buffer is longer than BUFFER_SIZE, all the '\0'
// characters will be overwritten and the copy will be truncated.
 
if (str1[BUFFER_SIZE-1] != '\0') {
    /* buffer was truncated, handle error? */
}
```
For the other functions in the family, the *n* variants exist as well: strncpm and strncat

3. sprintf

Reason: Just as the previous functions, sprintf does not check the buffer boundaries and is vulnerable to overflows. Actually I met this problem before when I was doing the lab 5.
Vulnerable code:
```C
#include <stdio.h>
#include <stdlib.h>
 
enum { BUFFER_SIZE = 10 };
 
int main() {
    char buffer[BUFFER_SIZE];
    int check = 0;
 
    sprintf(buffer, "%s", "This string is too long!");
 
    printf external link("check: %d", check); /* This will not print 0! */
 
    return EXIT_SUCCESS;
}
```
Mitigation: Prefer using snprintf, which has the double advantage of preventing buffers overflows and returning the minimal size of buffer needed to fit the whole formatted string.
```C
#include <stdio.h>
#include <stdlib.h>
 
enum { BUFFER_SIZE = 10 };
 
int main() {
    char buffer[BUFFER_SIZE];
 
    int length = snprintf(buffer, BUFFER_SIZE, "%s%s", "long-name", "suffix");
 
    if (length >= BUFFER_SIZE) {
        /* handle string truncation! */
    }
 
    return EXIT_SUCCESS;
}
```

## Thrashing
See ./thrashing.c. It will run extremely slow and the CPU usage will be very high. Note that my virtual machine has only 4 GB of memory, so I set the memory size to 1 GB. If you have more memory, you can change the size to see the thrashing effect more clearly.
