#include "ui.h"

void createFile(struct Node **head, const char *sortType, const char *dataType)
{
    char newfilename[128];
    snprintf(newfilename, 128, "%s_%s.txt", sortType, dataType);
    writeFile(head, newfilename);
}

void menu_interface()
{
    printf("Welcome to ECE4820JFA23 Lab5: Layer Programming!\n");
    int flag_exit = 0;
    while (!flag_exit)
    {
        printf(
            "Usage: [Input File Path] [Sorting Type]\n"
            "This program will sort the input file and output the result to a new file.\n"
            "Input file name should follow this format: <xxx>_<data_type>.txt\n"
            "The content in input file should follow \"key=value\" format, where value follows the <data_type> in the filename\n"
            "supported sorting type: rand, inc, dec\n"
            "supported data_type: int, double\n"
            "For example: rand_int.txt dec\n This will sort the content in rand_int.txt and output the result to dec_int.txt\n"
            "-----\nPlease input your operation or quit the program with the command exit.\n>>> ");

        char *key[3];
        for (int i = 0; i < 3; i++)
            key[i] = (char *)malloc(sizeof(char) * 128);
        char tmp[128];
        if (fgets(tmp, 128, stdin) == NULL)
            printf("Error: fgets fail.\n");
        char *token;
        token = strtok(tmp, " \n");
        if (token == NULL)
            printf("Error: strtok fails.\n");
        strcpy(key[1], token);
        if (!strcmp(key[1], "exit"))
        {
            flag_exit = 1;
        }
        else
        {
            token = strtok(NULL, " \n");
            if (token == NULL)
                printf("Error: strtok fails.\n");
            strcpy(key[2], token);
            cmd_interface(key[1], key[2]);
            printf("Opeartion succeeds.\n---------------\n");
        }
        for (int i = 0; i < 3; i++)
            free(key[i]);
    }
}

void cmd_interface(char *filename, char *sortType)
{
    srand((unsigned)time(0));
    struct Node *tmp = NULL;
    struct Node **head = &tmp;
    int Type = readFile(head, filename);
    if (Type == -1)
    {
        printf("Failed to read file %s\n", filename);
        return;
    }

    // For debug
    // printf("Type: %d\n", Type);
    // displayList(head, Type);
    // printf("dispaly over\n");

    char *dataType = NULL;
    if (Type == 0)
    {
        dataType = "int";
    }
    else if (Type == 1)
    {
        dataType = "double";
    }
    else
    {
        dataType = "string";
    }
    if (strcmp(sortType, "rand") == 0)
    {
        sortList(head, Type, 0);
        createFile(head, "rand", dataType);
        return;
    }
    else if (strcmp(sortType, "inc") == 0)
    {
        sortList(head, Type, 1);
        createFile(head, "inc", dataType);
        return;
    }
    else if (strcmp(sortType, "dec") == 0)
    {
        sortList(head, Type, 2);
        createFile(head, "dec", dataType);
        return;
    }
    else
    {
        printf("Invalid sort type %s\n", sortType);
        return;
    }
    freeList(head);
    return;
}
