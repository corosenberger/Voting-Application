#include "String.h"

String* createString() {
    String* str = callocg(sizeof(String));
    str->spaceAvailable = STRING_INIT_SPACE_AVAILABLE;
    str->size = 0;
    str->chars = callocg((str->spaceAvailable+1)*sizeof(char));
    return str;
}

String* createStringArr(char* s) {
    String* str = createString();
    stringextend(str,s);
    return str;
}

String* createStringArrLen(char* s, int len) {
    String* str = createString();
    for(int i = 0; i < len; i++) stringappend(str,s[i]);
    return str;
}

String* createStringFile(char* filename) {
    String* str = createString();

    int fd = open(filename, O_RDONLY);
    char c; int bytesRead;
    while(bytesRead = read(fd,&c,sizeof(char))) {
        stringappend(str,c);
    }
    close(fd);

    return str;
}

char stringget(String* str, int idx) {
    if(idx >= str->size) return '\0';
    return str->chars[idx];
}

int stringset(String* str, int idx, char c) {
    if(idx >= str->size || str->chars[idx] == c) return 0;
    str->chars[idx] = c;
    return 1;
}

void stringresize(String* str) {
    str->spaceAvailable *= STRING_RESIZING_FACTOR;
    void* newchars = callocg((str->spaceAvailable+1)*sizeof(char));
    memcpy(newchars,str->chars,str->size*sizeof(char));
    freeg(str->chars);
    str->chars = newchars;
}

void stringappend(String* str, char c) {
    if(str->size == str->spaceAvailable) stringresize(str);
    str->chars[str->size++] = c;
}

void stringextend(String* str, char* s) {
    for(int i = 0; i < strlen(s); i++) stringappend(str,s[i]);
}

int stringfind(String* str, char c) {
    for(int i = 0; i < str->size; i++) {
        if(stringget(str,i) == c) return i;
    }
    return -1;
}

int stringrfind(String* str, char c) {
    for(int i = str->size; i >= 0 ; i--) {
        if(stringget(str,i) == c) return i;
    }
    return -1;
}

String* stringsub(String* str, int sidx, int eidx) {
    if(eidx>=sidx || str->size<=sidx || str->size<eidx) return NULL;
    String* rstr = createString();
    for(int i = sidx; i < eidx; i++) stringappend(rstr,str->chars[i]);
    return rstr;
}

int stringcount(String* str, char c) {
    int count = 0;
    for(int i = 0; i < str->size; i++) {
        if(stringget(str,i) == c) count++;
    }
    return count;
}

String** stringsplit(String* str, char c) {
    String** out = callocg((stringcount(str,c)+1)*sizeof(String*));
    String* s = createString();
    int j = 0;
    for(int i = 0; i < str->size; i++) {
        if(str->chars[i] == c) {
            out[j++] = s;
            s = createString();
        } else {
            stringappend(s,str->chars[i]);
        }
    }
    out[j++] = s;
    return out;
}

void freeString(String* str) {
    freeg(str->chars);
    freeg(str);
}
