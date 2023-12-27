#include <stdio.h>
#include <string.h>

int main() {
    char *s = "rand_int.txt";
    char *t = "_int.txt";
    char *p = strstr(s, t);
    printf("%s\n", p);
    return 0;
}