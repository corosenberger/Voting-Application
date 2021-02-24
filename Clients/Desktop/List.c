#include "List.h"

List* createList() {
    List* lt = callocg(sizeof(List));
    lt->spaceAvailable = LIST_INIT_SPACE_AVAILABLE;
    lt->size = 0;
    lt->items = callocg(lt->spaceAvailable*sizeof(var*));
    return lt;
}

void* listget(List* lt, int idx) {
    if(idx >= lt->size) return NULL;
    var* item = lt->items[idx];
    return (item->type=='p') ? item->data.p:&item->data.p;
}

int listset(List* lt, int idx, char type, void* pval) {
    if(idx >= lt->size) return 0;
    var* vval = createvar(pval,type);
    lt->items[idx] = vval;
    return 1;
}

void listresize(List* lt) {
    lt->spaceAvailable *= LIST_RESIZING_FACTOR;
    void* newitems = callocg(lt->spaceAvailable*sizeof(void*));
    memcpy(newitems,lt->items,lt->size*sizeof(var*));
    freeg(lt->items);
    lt->items = newitems;
}

void listappend(List* lt, char type, void* pval) {
    if(lt->size == lt->spaceAvailable) listresize(lt);
    lt->size++;
    listset(lt,lt->size-1,type,pval);
}

void listextend(List* lt, char type, void* pvals, int vsize) {
    for(int i = 0; i < vsize; i++) {
        listappend(lt,type,pvals);
        pvals += varsize(type);
    }
}

void listextendlist(List* lt, List* other) {
    for(int i = 0; i < other->size; i++) {
        var* item = other->items[i];
        void* pval = (item->type=='p') ? item->data.p:&item->data.p;
        listappend(lt,item->type,pval);
    }
}

void freeList(List* lt) {
    for(int i = 0; i < lt->size; i++) freeg(lt->items[i]);
    freeg(lt->items);
    freeg(lt);
}
