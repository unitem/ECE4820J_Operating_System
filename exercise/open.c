#include <fcntl.h>
#include <stdio.h>
 
int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open error");
        return 1;
    } 
    return 0;
}