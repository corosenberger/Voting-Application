#ifndef _STRING_GARBAGE_H
#define _STRING_GARBAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Garbage.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct String {
    char* chars;
    int size;
    int spaceAvailable;
} String;

String* createString();
String* createStringArr(char* s);
String* createStringArrLen(char* s, int len);
String* createStringFile(char* filename);
char stringget(String* str, int idx);
int stringset(String* str, int idx, char c);
void stringappend(String* str, char c);
void stringextend(String* str, char* s);
int stringfind(String* str, char c);
int stringrfind(String* str, char c);
String* stringsub(String* str, int sidx, int eidx);
int stringcount(String* str, char c);
String** stringsplit(String* str, char c);
void freeString(String* lt);

#define STRING_INIT_SPACE_AVAILABLE 10
#define STRING_RESIZING_FACTOR 2

#ifdef __cplusplus
}
#endif

#endif