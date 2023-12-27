#include <stdlib.h>
#include <stdio.h>

typedef struct myString_t {
    char *str;
    int length;
} myString;

int main() {
    myString *str = (myString *)malloc(sizeof(myString));
    str->str = (char *)malloc(sizeof(char) * 10);
    str->length = 10;
    printf("%d\n", str->length);
    free(str->str);
    free(str);
    return 0;
}