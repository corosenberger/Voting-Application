#ifndef _LIST_GARBAGE_H
#define _LIST_GARBAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Garbage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct List {
    var** items;
    int size;
    int spaceAvailable;
} List;

List* createList();
void* listget(List* lt, int idx);
int listset(List* lt, int idx, char type, void* pval);
void listresize(List* lt);
void listappend(List* lt, char type, void* pval);
void listextend(List* lt, char type, void* pvals, int vsize);
void listextendlist(List* lt, List* other);
void freeList(List* lt);

#define LIST_INIT_SPACE_AVAILABLE 10
#define LIST_RESIZING_FACTOR 2

#ifdef __cplusplus
}
#endif

#endif