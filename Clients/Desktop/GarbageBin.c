#include "GarbageBin.h"

GarbageBin* createGarbageBin() {
	GarbageBin* hs = calloc(sizeof(GarbageBin),1);
	hs->spaceAvailable = GARBAGEBIN_INIT_SPACE_AVAILABLE;
	hs->maxLF = GARBAGEBIN_MAX_LOAD_FACTOR;
	hs->size = 0;
	hs->items = calloc(hs->spaceAvailable*sizeof(GarbageBinEntry*),1);
	return hs;
}

int garbagebinhashcode(int sa, void* item) {
	int x = (int)item;
	long hc = ((((x >> 7) ^ x) ^ 0x5555555555555555) << 11) ^ x;
	return (hc >0 ? hc:0-hc) % sa;
}

int garbagebincontains(GarbageBin* hs, void* item) {
	int hc = garbagebinhashcode(hs->spaceAvailable, item);

	GarbageBinEntry* e = hs->items[hc];
	if(e == NULL) {
		return 0;
	} else {
		while(e != NULL) {
			if(e->val == item) return 1;
			e = e->next;
		}
	}
	return 0;
}

int garbagebinadd(GarbageBin* hs, void* item) {
	if(garbagebincontains(hs,item)) return 0;

	if(hs->maxLF <= (hs->size / (double)hs->spaceAvailable)) garbagebinrehash(hs);
	GarbageBinEntry* e = calloc(1,sizeof(GarbageBinEntry));
	e->val = item;
	return garbagebinaddHelper(hs,e);
}

int garbagebinaddHelper(GarbageBin* hs, GarbageBinEntry* e) {
	int hc = garbagebinhashcode(hs->spaceAvailable, e->val);
	e->next = hs->items[hc];
	hs->items[hc] = e;
	hs->size++;
	return 1;
}

int garbagebinremove(GarbageBin* hs, void* item) {
	int hc = garbagebinhashcode(hs->spaceAvailable, item);

	GarbageBinEntry* e = hs->items[hc];
	GarbageBinEntry* temp;
	if(e == NULL) {
		return 0;
	} else if(e->val == item) {
		temp = hs->items[hc];
		hs->items[hc] = hs->items[hc]->next;
	} else {
		int removed = 0;
		while(!removed) {
			if(e->next == NULL) return 0;
			
			if(e->next->val == item) {
				temp = e->next;
				e->next = e->next->next;
				removed = 1;
			}
			e = e->next;
		}
	}
	free(temp);
	hs->size--;
	return 1;
}

void garbagebinrehash(GarbageBin* hs) {
	GarbageBin* newhs = calloc(1,sizeof(GarbageBin));
	newhs->spaceAvailable = GARBAGEBIN_RESIZING_FACTOR * hs->spaceAvailable;
	newhs->items = calloc(newhs->spaceAvailable,sizeof(GarbageBinEntry*));

	int i;
	for(i = 0; i < hs->spaceAvailable; i++) {
		while(hs->items[i] != NULL) {
			GarbageBinEntry* temp = hs->items[i];
			hs->items[i] = hs->items[i]->next;
			temp->next = NULL;
			garbagebinaddHelper(newhs,temp);
		}
	}
	
	free(hs->items);
	hs->items = newhs->items;
	hs->spaceAvailable = newhs->spaceAvailable;
	free(newhs);
}

void freeGarbageBin(GarbageBin* hs) {
	int i;
	for(i = 0; i < hs->spaceAvailable; i++) {
		while(hs->items[i] != NULL) {
			GarbageBinEntry* temp = hs->items[i];
			hs->items[i] = hs->items[i]->next;
			free(temp->val);
			free(temp);
		}
	}
	free(hs->items);
	free(hs);
}