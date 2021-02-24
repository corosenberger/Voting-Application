#ifndef _GARBAGE_GARBAGE_H
#define _GARBAGE_GARBAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "GarbageBin.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct var {
    union {void* p; char c; int i; long l; long long L; 
        float f; double d; long double D;} data;
    char type;
} var;

void passCollectionBlock();
void unblockCollection();
void initCollection();
void* mallocg(int space);
void* callocg(int space);
void freeg(void* item);
void freegAll();
var* createvar(void* mem, char type);
int varcmp(var* v1, var* v2);
int varsize(char type);

#ifdef __cplusplus
}
#endif

#endif