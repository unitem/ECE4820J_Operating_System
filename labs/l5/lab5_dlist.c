#include "lab5_dlist.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node_t
{
    char *key;
    dlistValue value;
    struct Node_t *next;
} Node;

typedef struct List_t
{
    Node *head;
    dlistValueType type;
    int length;
} List;

dlist createDlist(dlistValueType type)
{
    if (type == DLIST_UNKOWN)
        return NULL;
    dlist list = (List *)malloc(sizeof(List));
    ((List *)list)->head = NULL;
    ((List *)list)->type = type;
    ((List *)list)->length = 0;
    return list;
}

int dlistIsEmpty(dlist_const this)
{
    return (((List *)this)->length == 0);
}

void dlistAppend(dlist this, const char *key, dlistValue value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->next = NULL;

    node->key = (char *)malloc(strlen(key) + 1);
    strcpy(node->key, key);
    if (((List *)this)->type == DLIST_INT)
    {
        node->value.intValue = value.intValue;
    }
    else if (((List *)this)->type == DLIST_DOUBLE)
    {
        node->value.doubleValue = value.doubleValue;
    }
    else if (((List *)this)->type == DLIST_STR)
    {
        node->value.strValue = (char *)malloc(strlen(value.strValue) + 1);
        strcpy(node->value.strValue, value.strValue);
    }
    if (dlistIsEmpty(this))
    {
        ((List *)this)->head = node;
    }
    else
    {
        Node *current = ((List *)this)->head;
        while (current->next != NULL)
            current = current->next;
        current->next = node;
    }
    ((List *)this)->length++;
}

static int rand_comp(const void *a, const void *b)
{
    if (a == NULL || b == NULL)
    {
        printf("NULL pointer in rand_comp\n");
        exit(1);
    }
    return rand() % 2 * 2 - 1;
}

int compareInt(int a, int b)
// Comparison function for searching integers
{
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

int compareDouble(double a, double b)
// Comparison function for searching doubles
{
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

static int int_inc(const void *a, const void *b)
{
    return compareInt((((Node *)a)->value).intValue, (((Node *)b)->value).intValue);
}
static int int_dec(const void *a, const void *b)
{
    return compareInt((((Node *)b)->value).intValue, (((Node *)a)->value).intValue);
}
static int double_inc(const void *a, const void *b)
{
    return compareDouble((((Node *)a)->value).doubleValue, (((Node *)b)->value).doubleValue);
}
static int double_dec(const void *a, const void *b)
{
    return compareDouble((((Node *)b)->value).doubleValue, (((Node *)a)->value).doubleValue);
}
static int char_inc(const void *a, const void *b)
{
    return strcmp((((Node *)a)->value).strValue, (((Node *)b)->value).strValue);
}
static int char_dec(const void *a, const void *b)
{
    return strcmp((((Node *)b)->value).strValue, (((Node *)a)->value).strValue);
}

int (*comp_map[3][3])(const void *, const void *) = {
    {rand_comp, int_inc, int_dec},
    {rand_comp, char_inc, char_dec},
    {rand_comp, double_inc, double_dec}};

void deepCopyNode(Node *dst, Node *src, dlistValueType type)
{
    dst->key = (char *)malloc(strlen(src->key) + 1);
    strcpy(dst->key, src->key);
    if (type == DLIST_INT || type == DLIST_DOUBLE)
    {
        dst->value = src->value;
    }
    else
    {
        dst->value.strValue = (char *)malloc(strlen((src->value).strValue) + 1);
        strcpy(dst->value.strValue, src->value.strValue);
    }
    dst->next = src->next;
}

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

void freeNode(Node *node, dlistValueType type)
{
    if (type == DLIST_STR)
        free((node->value).strValue);
    free(node->key);
    free(node);
}

void dlistRemove(dlist this, Node* node)
{
    if (dlistIsEmpty(this))
        return;
    if (((List *)this)->head == node)
    {
        ((List *)this)->head = node->next;
        freeNode(node, ((List *)this)->type);
        ((List *)this)->length--;
        return;
    }
    Node *current = ((List *)this)->head;
    while (current->next != node)
        current = current->next;
    current->next = node->next;
    freeNode(node, ((List *)this)->type);
    ((List *)this)->length--;
}

Node *dlistFirst(dlist_const this)
{
    return ((List *)this)->head;
}

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method)
{
    if (dlistIsEmpty(this) || method == DLIST_SORT_UNKOWN || listDst == NULL || ((List *)listDst)->type != ((List *)this)->type)
        return;
    while (!dlistIsEmpty(listDst))
    {
        dlistRemove(listDst, dlistFirst(listDst));
    }
    // Dynamically allocate memory for the src array
    Node **src = (Node **)malloc((size_t)((List *)this)->length * sizeof(Node *));
    if (src == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    Node *current = ((List *)this)->head;
    int index = 0;
    while (current != NULL)
    {
        src[index] = (Node *)malloc(sizeof(Node));
        deepCopyNode(src[index], current, ((List *)this)->type);
        index++;
        current = current->next;
    }
    int j;
    for (int i = 1; i < ((List *)this)->length; i++)
    {
        current = src[i];
        j = i - 1;
        while (j >= 0)
        {
            int compResult = comp_map[((List *)this)->type - 1][method - 1](src[j], current);
            if (compResult == 0)
            {
                if (isIntegerKey(src[j]->key) && isIntegerKey(current->key))
                {
                    compResult = atoi(src[j]->key) - atoi(current->key);
                }
                else
                {
                    compResult = strcmp(src[j]->key, current->key);
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
        src[j + 1] = current;
    }

    ((List *)listDst)->head = src[0];
    for (int i = 0; i < ((List *)this)->length - 1; i++)
        src[i]->next = src[i + 1];
    src[((List *)this)->length - 1]->next = NULL;
    ((List *)listDst)->length = ((List *)this)->length;

    // Free the dynamically allocated memory for the src array
    free(src);
}

void dlistPrint(dlist_const this)
{
    if (((List *)this)->length == 0)
        return;
    Node *current;
    for (current = ((List *)this)->head; current != NULL; current = current->next)
    {
        if (((List *)this)->type == DLIST_INT)
            printf("%s=%d\n", current->key, (current->value).intValue);
        else if (((List *)this)->type == DLIST_DOUBLE)
            printf("%s=%lf\n", current->key, (current->value).doubleValue);
        else
            printf("%s=%s\n", current->key, (current->value).strValue);
    }
}

void dlistFree(dlist this)
{
    if (((List *)this)->length == 0)
    {
        free(this);
        return;
    }
    Node *current = ((List *)this)->head;
    Node *tmp;
    while (current != NULL)
    {
        tmp = current;
        current = current->next;
        freeNode(tmp, ((List *)this)->type);
    }
    ((List *)this)->length = 0;
    ((List *)this)->head = NULL;
    free(this);
}
