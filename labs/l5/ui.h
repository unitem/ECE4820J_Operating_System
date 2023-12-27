#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.h"
#include "sort.h"

void createFile(struct Node **head, const char *sortType, const char *dataType);
void menu_interface(void);
void cmd_interface(char *filename, char *sortType);

#endif
