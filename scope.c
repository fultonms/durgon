#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "scope.h"

extern int offsetMode;

static int hashpjw(char* s){
    char* p;
    unsigned h = 0, g;

    for(p = s; *p != EOS; p++){
        h = (h << 4) + (*p);
        if(( g = h & 0xf0000000)) {
            h = h ^ (g << 24);
            h = h ^ g;
        }
    }

    return h % HASH_SIZE;
}

node_t* scope_search(scope_t* scope, char* name){
    if( scope == NULL) 
        return NULL;
    
    int i;
    node_t* head;

    i = hashpjw(name);
    head = scope->table[i];

    return node_search(head, name);
}

node_t* scope_insert(scope_t* scope, char* name){
    if( scope == NULL)
        return NULL

    if( scope_search(scope, name)){
        fprintf(stderr, "Name %s already defined in scope.\n", name);
        assert(0);
    }

    int i;
    node_t* head;
    int offset;

    i = hashpjw(name);
    head = scope->table[i];

    switch(offsetMode){
        case -1: offset = 0;
        case 0 : offset = 0;
        case 1 : offset = 0;
    }

    scope->table[i] = node_push(head, name, offset);
    return scope-table[i]
}

node_t* scope_seachall(scope_t* head, char* name){

}

scope_t* make_scope(){
    scope_t* s = calloc(i, sizeof(scope_t));
    s->off_arg = 0;
    return s;
}

//Stack functions
scope_t* scope_push(scope_t* top, int type){
    scope_t* s = make_scope();
    s->top = top;
    s->type = type;
    return s;
}

scope_t* scope_pop(scope_t* head){

}