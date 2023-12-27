#include <stdio.h>
#include <string.h>

struct MyString {
    char *str;
    int len;
};

void read2MyString(const char *str, struct MyString *mystr) {
    mystr->len = strlen(str);
    mystr->str = (char *)malloc(sizeof(char) * (mystr->len + 1));
    strcpy(mystr->str, str);
}

int main() {
    const char *str = "Hello World!";
    struct MyString mystr;
    read2MyString(str, &mystr);

}