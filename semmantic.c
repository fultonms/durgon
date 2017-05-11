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
    }else if(t->type == PROCEDURE){
        return PROCEDURE;
    }else if(t->type == FUNCTION){
        return FUNCTION;
    }

    return 0;
}

static char* get_type_name(int type){
    switch(type){
        case INUM:
            return "INUM";
        case RNUM:
            return "RNUM";
        case ADDOP:
            return "ADDOP";
        case MULOP:
            return "MULOP";
        default:
            return "UKNOWN TYPE";
    }
}

//Ensures that addops are only between equal types.
static int check_addop(tree_t* t){
    int l, r;

    l = get_type(t->left);
    r = get_type(t->right);

    if(l != r){
        ERROR("Error: Type mismatch between %s and %s", get_type_name(l), get_type_name(r));
        return 1;
    }
    
    return 0;
}

static int check_mulop(tree_t* t){
    return 0;
}

static int count_proc_calls(tree_t* t){
    int l, r;
    if(t == NULL)
        return 0;
    
    if(t->type == PROCEDURE_CALL)
        return 1;

    l = count_proc_calls(t->left);
    r = count_proc_calls(t->right);
    return l + r;
}


static int check_asnop(tree_t* t){
    if(get_type(t->left) == ID && t->left->attribute.sval->func != NULL && t->left->attribute.sval->func->ret){
        if(t->left->attribute.sval->func->ret != get_type(t->right)){
            ERROR("Function %s trying to return %s, return type is %s", t->left->attribute.sval->name, get_type_name(t->left->attribute.sval->func->ret), get_type_name(get_type(t->right)));
            return 1;
        }
    }

    if (count_proc_calls(t) > 0){
        ERROR("Attempting to assign a procedure call to %s.", t->left->attribute.sval->name);
        return 1;
    }


    return 0;
}

static int check_aaccess(tree_t* t){
    if(get_type(t->right) != INUM){
        ERROR("Attempting to access array %s with a non integer-typed expresion.", t->left->attribute.sval->name);
        return 1;
    }


    return 0;
}

static int match_args(tree_t* arglist, node_t* n){
    tree_t* t;
    typen_t* ptype = n->func->types;
    int ret = 0;

    for(t = arglist; t != NULL; t = t->left){
        if(get_type(t->right) != ptype->type){
            ERROR("Error in function %s: Type mistmatch on argument between %s and %s", n->name, get_type_name(get_type(t->right)), get_type_name(ptype->type));
            ret++;
        }
        ptype = ptype->next;
    }
    return ret;
}

static int count_args(tree_t* t){
    if(t == NULL)
        return 0;

    return count_args(t->left) + 1;   
}

static int check_function(tree_t* t){
    int argc = count_args(t->right);
    func_t* func = t->left->attribute.sval->func;

    print_tree(t);

    if(t->left->attribute.sval->type == PROCEDURE){
        ERROR("Attempting to use procedure %s as a function.", t->left->attribute.sval->name);
        return 1;
    }

    if(func->argc != argc){
        ERROR("Number of arguments is wrong in function %s.", t->left->attribute.sval->name);
        return 1;
    }

    return match_args(t->right, t->left->attribute.sval);
}

static int check_procedure(tree_t* t){
    int argc = count_args(t->right);
    func_t* func = t->left->attribute.sval->func;

    if(func ->ret != 0){
        ERROR("Procedures do not return values: procedure %s.", t->left->attribute.sval->name);
        return 1;
    }

    if(func->argc != argc){
        ERROR("Number of arguments is wrong in procedure %s.", t->left->attribute.sval->name);
        return 1;
    }

    return match_args(t->right, t->left->attribute.sval);
}

static int check_ifwhile(tree_t* t){
    if(t->left->type != RELOP){
        ERROR("Expecting a boolean typed expresion, instead got %s", get_type_name(t->left->type));
        return 1;
    }
    return 0;
}

static int check_for(tree_t* t){
    int var, upper, lower;

    var = get_type(t->left->left);
    lower = get_type(t->left->left);
    upper = get_type(t->right->left);

    if(!( var == lower == upper)){
        ERROR("Type mismatch in for loop. Expecting %s", get_type_name(var));
        return 1;
    }

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
        case ARRAY_ACCESS: ret = check_aaccess(t); break;
        case FUNCTION_CALL: ret = check_function(t); break;
        case PROCEDURE_CALL: ret = check_procedure(t); break;
        case IF: ret = check_ifwhile(t); break;
        case WHILE: ret = check_ifwhile(t); break;
        case FOR: ret = check_for(t); break;
    }
    return ret;
}