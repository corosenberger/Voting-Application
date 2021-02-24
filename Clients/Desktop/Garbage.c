#include "Garbage.h"

volatile int garbageinit;
pthread_mutex_t garbagelock;
volatile int garbageblock;
GarbageBin* mainmem;

void passCollectionBlock() {
	garbageRestart:
	while(garbageblock) {/*pass*/}
	pthread_mutex_lock(&garbagelock);
	if(garbageblock) {pthread_mutex_unlock(&garbagelock); goto garbageRestart;} 
	garbageblock = 1;
	pthread_mutex_unlock(&garbagelock);
}

void unblockCollection() {
	garbageblock = 0;
}

void initCollection() {
	if(!garbageinit) {
		pthread_mutex_init(&garbagelock, NULL);

		passCollectionBlock();
		if(!garbageinit) {
			garbageblock = 0;
			mainmem = createGarbageBin();
			garbageinit = 1;
		}
		unblockCollection();
	}
}

void* mallocg(int space) {
	if(!garbageinit) initCollection();
	void* item = malloc(space);
	passCollectionBlock();
	garbagebinadd(mainmem,item);
	unblockCollection();
	return item;
}

void* callocg(int space) {
	if(!garbageinit) initCollection();
	void* item = calloc(1,space);
	passCollectionBlock();
	garbagebinadd(mainmem,item);
	unblockCollection();
	return item;
}

void freeg(void* item) {
	if(!garbageinit) return;
	passCollectionBlock();
	if(garbageinit) {
		int toFree = garbagebinremove(mainmem,item);
		if(toFree) free(item);
	}
	unblockCollection();
}

void freegAll() {
	if(!garbageinit) return;
	passCollectionBlock();
	if(garbageinit) {
		freeGarbageBin(mainmem);
		garbageinit = 0;
	}
	unblockCollection();
}

var* createvar(void* mem, char type) {
	var* v = callocg(sizeof(var));
	switch(type) {
		case 'p': v->data.p = mem; break;
		case 'c': v->data.c = *(char*)mem; break;
		case 'i': v->data.i = *(int*)mem; break;
		case 'l': v->data.l = *(long*)mem; break;
		case 'L': v->data.L = *(long long*)mem; break;
		case 'f': v->data.f = *(float*)mem; break;
		case 'd': v->data.d = *(double*)mem; break;
		case 'D': v->data.D = *(long double*)mem; break;
		default: return NULL;
	}
	v->type = type;
	return v;
}

int varcmp(var* v1, var* v2) {
	if(v1->type == v2->type) {
		switch(v1->type) {
			case 'p': return (v1->data.p<v2->data.p) ? -1:(v1->data.p>v2->data.p) ? 1:0;
			case 'c': return (v1->data.c<v2->data.c) ? -1:(v1->data.c>v2->data.c) ? 1:0;
			case 'i': return (v1->data.i<v2->data.i) ? -1:(v1->data.i>v2->data.i) ? 1:0;
			case 'l': return (v1->data.l<v2->data.l) ? -1:(v1->data.l>v2->data.l) ? 1:0;
			case 'L': return (v1->data.L<v2->data.L) ? -1:(v1->data.L>v2->data.L) ? 1:0;
			case 'f': return (v1->data.f<v2->data.f) ? -1:(v1->data.f>v2->data.f) ? 1:0;
			case 'd': return (v1->data.d<v2->data.d) ? -1:(v1->data.d>v2->data.d) ? 1:0;
			case 'D': return (v1->data.D<v2->data.D) ? -1:(v1->data.D>v2->data.D) ? 1:0;
		}
	}
	return -2;
}

int varsize(char type) {
	switch(type) {
		case 'p': return sizeof(void*);
		case 'c': return sizeof(char);
		case 'i': return sizeof(int);
		case 'l': return sizeof(long);
		case 'L': return sizeof(long long);
		case 'f': return sizeof(float);
		case 'd': return sizeof(double);
		case 'D': return sizeof(long double);
		default: return 0;
	}
}