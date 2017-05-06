#ifndef SCOPE_H
#define SCOPE_H

#include "node.h"

#define HASH_SIZE 211
#define EOS         '\0'

typedef struct scope_s {
    node_t* table[HASH_SIZE];

    int type;
    struct scope_s* next;
} scope_t;

//Basic search and insert
node_t* scope_search(scope_t* scope, char* name);
node_t* scope_insert(scope_t* scope, char* name);

node_t* scope_seachall(scope_t* head, char* name);

scope_t* make_scope();

//Stack functions
scope_t* scope_push(scope_t* top, int type);
scope_t* scope_pop(scope_t* head);

#endif