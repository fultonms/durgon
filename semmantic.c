#include <stdio.h>
#include "scope.h"
#include "node.h"
#include "semmantic.h"
#include "y.tab.h"

//Gets the type of a tree.
static int get_type(tree_t* t){
    if (t->type == INUM){
        return INUM;
    }else if(t->type == RNUM){
        return RNUM;
    }

    return 0;
}

//Ensures that addops are only between equal types.
int check_addop(tree_t* t){
    int l, r;

    l = get_type(t->left);
    r = get_type(t->right);

    if(l != r){
        fprintf(stderr, "Error: Type mismatch between %d and %d\n", l, r);
        return 1;
    }
    
    return 0;
}

int check_mulop(tree_t* t){
    return 0;
}

int check_asnop(tree_t* t){

}

//Runs all semmantic checks that can be done by just looking at the tree.
//Scoping semmantic checks should be getting caught in symbol table calls.
int check_tree(tree_t* t){
    if( t == NULL)
        return -1;

    int ret;

    switch(t->type){
        case ADDOP: ret = check_addop(t); break;
        case MULOP: ret = check_mulop(t); break;
        case ASSIGNOP: ret = check_asnop(t); break;
    }
    return ret;
}