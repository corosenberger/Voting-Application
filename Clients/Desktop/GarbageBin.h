#ifndef _GARBAGEBIN_GARBAGE_H
#define _GARBAGEBIN_GARBAGE_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GarbageBinEntry {
	void* val;
	struct GarbageBinEntry* next;
} GarbageBinEntry;

typedef struct GarbageBin {
	GarbageBinEntry** items;
	int size;
	int spaceAvailable;
	double maxLF;
} GarbageBin;

GarbageBin* createGarbageBin();
int garbagebinhashcode(int sa, void* item);
int garbagebincontains(GarbageBin* hs, void* item);
int garbagebinadd(GarbageBin* hs, void* item);
int garbagebinaddHelper(GarbageBin* hs, GarbageBinEntry* e);
int garbagebinremove(GarbageBin* hs, void* item);
void garbagebinrehash(GarbageBin* hs);
void freeGarbageBin(GarbageBin* hs);

#define GARBAGEBIN_INIT_SPACE_AVAILABLE 10
#define GARBAGEBIN_MAX_LOAD_FACTOR 2
#define GARBAGEBIN_RESIZING_FACTOR 2

#ifdef __cplusplus
}
#endif

#endif