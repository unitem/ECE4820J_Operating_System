#include "ui.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error: arguments not match!\n");
        return -1;
    }
    srand((unsigned)time(NULL));
    cmd_interface(argv[1], argv[2]);

    return 0;
}
