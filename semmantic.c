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

static int check_asnop(tree_t* t){
    return 0;
}

static int count_args(tree_t* t){
    if(t == NULL)
        return 0;

    return count_args(t->left) + 1;   
}

static int check_function(tree_t* t){
    int argc = count_args(t->right);
    func_t* func = t->left->attribute.sval->func;

    if(func->argc != argc){
        ERROR("Number of arguments is wrong in function %s.", t->left->attribute.sval->name);
        return 1;
    }
    tree_t* param;
    typen_t* type = func->types;

    print_tree(param);
    
    for(param = t->right; param != NULL; param->left){
        fprintf(stderr, "%d, %d\n", type->type, param->right->attribute.sval->type);
        if(get_type(param->right) != type->type){
            ERROR("In function %s, parameter %s is the wrong type. Expected %s, got %s.", t->left->attribute.sval->name, param->right->attribute.sval->name, get_type_name(type->type), get_type_name(get_type(param->right)));
            return 1;
        }
        type = type->next;
    }
    return 0;
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

    return 0;
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
        case FUNCTION_CALL: ret = check_function(t); break;
        case PROCEDURE_CALL: ret = check_procedure(t); break;
        case IF: ret = check_ifwhile(t); break;
        case WHILE: ret = check_ifwhile(t); break;
        case FOR: ret = check_for(t); break;
    }
    return ret;
}