#ifndef NODE_H
#define NODE_H

//Stores type information for arguments.
typedef struct typen_s{
    int type;
    struct typen_s* next;
} typen_t;

//Stores function information, including the number of arguments, return value, and
//a list of all arguments that the function takes.
typedef struct {
    int argc;
    int ret;
    typen_t* types;
} func_t;

//The node backing the linked list of the hash table.__a
//contains name and type of the object, function info if 
//nessecary, ofset and depth information, and a pointer to 
//the next thing that hashed to that.
typedef struct node_s{
    char* name;
    int type;
    int offset;
    int depth;
    func_t* func;
    struct node_s* next;
} node_t;


node_t* node_create(char* name);

//Linked list functions
node_t* node_push(node_t* head, char* name, int offset);
node_t* node_search(node_t* head, char* name);
void node_free(node_t* n);

#endif