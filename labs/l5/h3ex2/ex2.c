#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// --- Basic Funcions ---

struct Node
// Define the structure for a node in the linked list
{
    char *pch;
    void *pv;
    struct Node *next;
};

struct Node *createNode(char *pch, void *pv)
// Create a new node and return a pointer to it
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->pch = pch;
    newNode->pv = pv;
    newNode->next = NULL;
    return newNode;
}

void displayList(struct Node **head, int dataType)
// Display the contents of the linked list
{
    struct Node *current = *head;
    while (current != NULL)
    {
        printf("String: %s, Data: ", current->pch);
        if (dataType == 0)
        {
            printf("%d\n", *((int *)current->pv));
        }
        else if (dataType == 1)
        {
            printf("%lf\n", *((double *)current->pv));
        }
        else if (dataType == 2)
        {
            printf("%s\n", (char *)current->pv);
        }
        current = current->next;
    }
}

void freeList(struct Node **head)
// Free the memory used by the linked list
{
    while (*head != NULL)
    {
        struct Node *current = *head;
        *head = current->next;
        free(current->pch);
        free(current->pv);
        free(current);
    }
}

// --- Ex 2.1 Append Function ---

void appendNode(struct Node **head, char *pch, void *pv)
// Add a new node at the end of the linked list
{
    struct Node *newNode = createNode(pch, pv);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct Node *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// --- Ex 2.2 Search Functions ---

struct Node *searchValue(struct Node **head, void *target, int (*compare)(void *, void *))
// search for a specific value in the linked list
// returns a pointer to the first node in the linked list that contains the target value
// returns NULL if the target value is not found
// To search int, use compareInt; to search double, use compareDouble; to search char*, use strcmp
{
    struct Node *current = *head;
    while (current != NULL)
    {
        if (compare(current->pch, target) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int compareInt(void *a, void *b)
// Comparison function for searching integers
{
    if (*((int *)a) < *((int *)b))
        return -1;
    if (*((int *)a) > *((int *)b))
        return 1;
    return 0;
}

int compareDouble(void *a, void *b)
// Comparison function for searching doubles
{
    if (*((double *)a) < *((double *)b))
        return -1;
    if (*((double *)a) > *((double *)b))
        return 1;
    return 0;
}

// --- Ex 2.3 File read & write Functions ---

int readFile(struct Node **head, const char *filename)
// Read data from a file and add it to the linked list
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", filename);
        fclose(file);
        return -2;
    }

    printf("reading %s\n", filename);
    int result = -1;
    if (strstr(filename, "_int") != NULL)
    {
        result = 0;
    }
    else if (strstr(filename, "_double") != NULL)
    {
        result = 1;
    }
    else if (strstr(filename, "_char*") != NULL)
    {
        result = 2;
    }
    else
    {
        printf("Invalid file name %s\n", filename);
        fclose(file);
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1)
    {
        char *equalSign = strchr(line, '=');
        if (!equalSign)
        {
            printf("Invalid line format in file %s\n", filename);
            free(line);
            fclose(file);
            return -2;
        }

        size_t keyLen = (unsigned long)(equalSign - line);
        char *tmpstring = malloc(keyLen + 1);
        strncpy(tmpstring, line, keyLen);
        tmpstring[keyLen] = '\0';

        void *somedata = NULL;

        if (result == 0)
        {
            int num;
            if (sscanf(equalSign + 1, "%d", &num) == 1)
            {
                somedata = malloc(sizeof(int));
                *((int *)somedata) = num;
            }
            else
            {
                printf("Failed to read data from file %s\n", filename);
                free(tmpstring);
                free(line);
                fclose(file);
                return -2;
            }
        }
        else if (result == 1)
        {
            double num;
            if (sscanf(equalSign + 1, "%lf", &num) == 1)
            {
                somedata = malloc(sizeof(double));
                *((double *)somedata) = num;
            }
            else
            {
                printf("Failed to read data from file %s\n", filename);
                free(tmpstring);
                free(line);
                fclose(file);
                return -2;
            }
        }
        else if (result == 2)
        {
            somedata = strdup(equalSign + 1);
            char *newline = strchr(somedata, '\n');
            if (newline)
            {
                *newline = '\0'; // Remove newline character
            }
        }

        appendNode(head, tmpstring, somedata);
    }

    free(line);
    fclose(file);
    return result;

    /* OLD version - cannot deal with long keys or char* data
    while (fgets(line, sizeof(line), file))
    {
        char tmpstring[128];
        void *somedata = NULL;
        char *somestring = NULL;

        // Create and add a node to the linked list based on the data type
        if (result == 0)
        {
            int num = 0;
            int n = sscanf(line, "%[^=]=%d", tmpstring, &num);
            if (n == 2)
            {
                somedata = malloc(sizeof(int));
                *((int *)somedata) = num;
            }
            else
            {
                printf("Failed to read data from file %s\n", filename);
                fclose(file);
                return -2;
            }
        }
        else if (result == 1)
        {
            double num = 0;
            int n = sscanf(line, "%[^=]=%lf", tmpstring, &num);
            if (n == 2)
            {
                somedata = malloc(sizeof(double));
                *((double *)somedata) = num;
            }
            else
            {
                printf("Failed to read data from file %s\n", filename);
                fclose(file);
                return -2;
            }
        }
        else if (result == 2)
        {
            char str[256];
            int n = sscanf(line, "%[^=]=%s", tmpstring, str);
            if (n == 2)
            {
                somedata = malloc(sizeof(char) * (strlen(str) + 1));
                strcpy((char *)somedata, str);
            }
            else
            {
                printf("Failed to read data from file %s\n", filename);
                fclose(file);
                return -2;
            }
        }
        somestring = malloc(sizeof(char) * strlen(tmpstring) + 1);
        strcpy(somestring, tmpstring);
        // for debug
        // printf("somestring: %s, somedata: %d\n", somestring, *((int *)somedata));
        appendNode(head, somestring, somedata);
    }
    */
}

void writeFile(struct Node **head, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", filename);
        fclose(file);
        return;
    }

    printf("writing %s\n", filename);
    const struct Node *current = *head;
    while (current != NULL)
    {
        char *somestring = NULL;
        if (strstr(filename, "_int.txt") != NULL)
        {
            asprintf(&somestring, "%s=%d\n", current->pch, *((int *)current->pv));
        }
        else if (strstr(filename, "_double.txt") != NULL)
        {
            asprintf(&somestring, "%s=%lf\n", current->pch, *((double *)current->pv));
        }
        else if (strstr(filename, "_char*.txt") != NULL)
        {
            asprintf(&somestring, "%s=%s\n", current->pch, (char *)current->pv);
        }

        if (somestring)
        {
            fputs(somestring, file);
            free(somestring);
        }

        current = current->next;

        /*
        char somestring[128];
        if (strstr(filename, "_int.txt") != NULL)
        {
            snprintf(somestring, 128, "%s=%d\n", current->pch, *((int *)current->pv));
        }
        else if (strstr(filename, "_double.txt") != NULL)
        {
            snprintf(somestring, 128, "%s=%lf\n", current->pch, *((double *)current->pv));
        }
        else if (strstr(filename, "_char*.txt") != NULL)
        {
            snprintf(somestring, 128, "%s=%s\n", current->pch, (char *)current->pv);
        }
        fputs(somestring, file);
        */
    }

    fclose(file);
}

// --- Comparison Functions ---

int isIntegerKey(const char *str)
{
    if (*str == '-' || *str == '+')
        str++;
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

static int rand_comp(const void *a, const void *b)
// return randomly 1 or -1
{
    if (a == NULL || b == NULL)
    {
        printf("NULL pointer in rand_comp\n");
        exit(1);
    }
    // for debug
    // printf("seed: %d; rand: %d\n", (unsigned)time(0), rand_num);
    return rand() % 2 * 2 - 1;
}
static int int_inc(const void *a, const void *b)
{
    return compareInt(((struct Node *)a)->pv, ((struct Node *)b)->pv);
}
static int int_dec(const void *a, const void *b)
{
    return compareInt(((struct Node *)b)->pv, ((struct Node *)a)->pv);
}
static int double_inc(const void *a, const void *b)
{
    return compareDouble(((struct Node *)a)->pv, ((struct Node *)b)->pv);
}
static int double_dec(const void *a, const void *b)
{
    return compareDouble(((struct Node *)b)->pv, ((struct Node *)a)->pv);
}
static int char_inc(const void *a, const void *b)
{
    return strcmp((char *)(((struct Node *)a)->pv), (char *)(((struct Node *)b)->pv));
}
static int char_dec(const void *a, const void *b)
{
    return strcmp((char *)(((struct Node *)b)->pv), (char *)(((struct Node *)a)->pv));
}

int (*comp_map[3][3])(const void *, const void *) = {
    {rand_comp, int_inc, int_dec},
    {rand_comp, double_inc, double_dec},
    {rand_comp, char_inc, char_dec}};

void sortList(struct Node **head, int dataType, int sortType)
{
    if (head == NULL || *head == NULL)
    {
        return;
    }
    printf("sorting elements\n");

    // Calculate the length of the list
    int length = 0;
    struct Node *current = *head;
    while (current != NULL)
    {
        length++;
        current = current->next;
    }

    // Dynamically allocate memory for the src array
    struct Node **src = (struct Node **)malloc((unsigned long)length * sizeof(struct Node *));
    if (src == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Populate the src array with pointers to nodes
    current = *head;
    int index = 0;
    while (current != NULL)
    {
        src[index++] = current;
        current = current->next;
    }

    struct Node *flag;
    int j;
    for (int i = 1; i < index; i++)
    {
        flag = src[i];
        j = i - 1;
        while (j >= 0)
        {
            int compResult = comp_map[dataType][sortType](src[j], flag);
            if (compResult == 0)
            {
                if (isIntegerKey(src[j]->pch) && isIntegerKey(flag->pch))
                {
                    compResult = atoi(src[j]->pch) - atoi(flag->pch);
                }
                else
                {
                    compResult = strcmp(src[j]->pch, flag->pch);
                }
            }
            
            if (compResult > 0)
            {
                src[j + 1] = src[j];
                j--;
            }
            else
            {
                break;
            }
        }
        src[j + 1] = flag;
    }

    *head = src[0];
    for (int i = 0; i < index - 1; i++)
    {
        src[i]->next = src[i + 1];
    }
    src[index - 1]->next = NULL;

    // Free the dynamically allocated memory for the src array
    free(src);
}

void createFile(struct Node **head, const char *sortType, const char *dataType)
{
    char newfilename[128];
    snprintf(newfilename, 128, "%s_%s.txt", sortType, dataType);
    writeFile(head, newfilename);
}

int main(int argc, char **argv)
{
    srand((unsigned)time(0));
    if (argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 0;
    }
    struct Node *tmp = NULL;
    struct Node **head = &tmp;
    int Type = readFile(head, argv[1]);
    if (Type == -1)
    {
        printf("Failed to read file %s\n", argv[1]);
        freeList(head);
        return 0;
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
        dataType = "char*";
    }
    if (strcmp(argv[2], "rand") == 0)
    {
        sortList(head, Type, 0);
        createFile(head, "rand", dataType);
    }
    else if (strcmp(argv[2], "inc") == 0)
    {
        sortList(head, Type, 1);
        createFile(head, "inc", dataType);
    }
    else if (strcmp(argv[2], "dec") == 0)
    {
        sortList(head, Type, 2);
        createFile(head, "dec", dataType);
    }
    else
    {
        printf("Invalid sort type %s\n", argv[2]);
    }
    freeList(head);
    return 0;
}
