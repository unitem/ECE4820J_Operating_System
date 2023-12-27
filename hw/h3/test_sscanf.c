#include <stdio.h>

int main() {
    char str[100];
    int somedata = 0;
    int n = sscanf("int=123", "%[^=]=%d", str, &somedata);
    printf("%d\n", n);
    printf("%s\n", str);
    printf("%d\n", somedata);
    void *pv = &somedata;
    printf("%d\n", *(int *)pv);
    return 0;
}