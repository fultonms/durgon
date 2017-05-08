#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "scope.h"

scope_t* top;

extern int offsetMode;
int depth = 0;

//Magical hashing.
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

//Checks to see if a name exists in the passed scope, returns it if so.
node_t* scope_search(scope_t* scope, char* name){
    if( scope == NULL) 
        return NULL;
    
    int i;
    node_t* head;

    i = hashpjw(name);
    head = scope->table[i];

    return node_search(head, name);
}

//Inserts a name into a scope, returning an error, if the name already exists.
node_t* scope_insert(scope_t* scope, char* name){
    if( scope == NULL)
        return NULL;

    if( scope_search(scope, name)){
        fprintf(stderr, "Name %s already defined in scope.\n", name);
        assert(0);
    }

    int i;
    node_t* head;
    int offset ;

    i = hashpjw(name);
    head = scope->table[i];

    //Need to set offset properlly.
    offset = 0;

    scope->table[i] = node_push(head, name, offset);
    return scope->table[i];
}

//Checks all scopes down from the current scope to see if the name exists.
node_t* scope_searchall(scope_t* scope, char* name){
   node_t * ret;

   if(scope == NULL)
      return NULL;

   while(scope != NULL){
      ret = scope_search(scope, name);
      
      if(ret != NULL){
         ret->depth = depth;
         depth = 0;
         return ret;
      }
      depth++;
      scope = scope->next;
   }

   depth = 0;
   fprintf(stderr, "Could not find %s\n", name);
   assert(0);
   return NULL;
}

//Creates a new scope.
scope_t* make_scope(){
    scope_t* s = calloc(1, sizeof(scope_t));
    s->off_arg = 8;
    return s;
}

//Stack functions
scope_t* scope_push(scope_t* top, int type){
    scope_t* s = make_scope();
    s->next = top;
    s->type = type;
    return s;
}

//Frees the memory of the list in each scope.
static void list_free(node_t* n){
    node_t* temp = NULL;

    if( n == NULL)
        return;

    temp = n->next;
    node_free(n);
    list_free(temp);
}

//Frees the memory of a scope.
static void scope_free(scope_t* scope){
    for(int i=0; i < HASH_SIZE; i++){
        list_free(scope->table[i]);
    }

    free(scope);
}

// Pops a scope off the stack.
scope_t* scope_pop(scope_t* head){
    scope_t* temp;
    
    if(head == NULL)
        return NULL;
    
    temp = head->next;
    scope_free(head);
    return temp;
}
