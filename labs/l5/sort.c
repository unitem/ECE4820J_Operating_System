#include "sort.h"

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
