#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

//Creates a new node.
node_t* node_create(char* name){
    node_t* n = (node_t*) calloc(1, sizeof(node_t));
    n->name = strdup(name);
    n->func = NULL;
    return n;
}

//Linked list functions
node_t* node_push(node_t* head, char* name, int offset){
    node_t* n = node_create(name);
    n->offset = offset;
    n->next = head;
    return n;
}

//Searches within the linked list that the node heads to find a name.
node_t* node_search(node_t* head, char* name){
    node_t* n = head;

    while (n != NULL){
        if(!strcmp(n->name, name)){
            return n;
        }
        n = n->next;
    }
    return NULL;
}

//Frees the memory space of a node.
void node_free(node_t* n){
    free(n->name);
    free(n);
}