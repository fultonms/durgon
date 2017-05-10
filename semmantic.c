#include <stdio.h>
#include "scope.h"
#include "node.h"
#include "semmantic.h"
#include "y.tab.h"

extern int line;

#define ERROR(fmt, ...) printf("\x1B[31mLine %d: " fmt "\x1B[0m\n", line, __VA_ARGS__)

//Gets the type of a tree.
static int get_type(tree_t* t){
    if (t->type == INUM){
        return INUM;
    }else if(t->type == RNUM){
        return RNUM;
    }else if(t->type == ID){
        return t->attribute.sval->type;
    }
    return 0;
}

static char* get_type_name(int type){
    switch(type){
        case INUM:
            return "INUM";
        case RNUM:
            return "RNUM";
        default:
            return "UKNOWN TYPE";
    }
}

//Ensures that addops are only between equal types.
int check_addop(tree_t* t){
    int l, r;

    l = get_type(t->left);
    r = get_type(t->right);

    if(l != r){
        ERROR("Error: Type mismatch between %s and %s", get_type_name(l), get_type_name(r));
        return 1;
    }
    
    return 0;
}

int check_mulop(tree_t* t){
    return 0;
}

int check_asnop(tree_t* t){
    return 0;
}

//Runs all semmantic checks that can be done by just looking at the tree.
//Scoping semmantic checks should be getting caught in symbol table calls.
int check_tree(tree_t* t){
    if( t == NULL)
        return -1;

    int ret = 0;

    switch(t->type){
        case ADDOP: ret = check_addop(t); break;
        case MULOP: ret = check_mulop(t); break;
        case ASSIGNOP: ret = check_asnop(t); break;
    }
    return ret;
}