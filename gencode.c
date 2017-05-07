#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "scope.h"
#include "register.h"
#include "gencode.h"
#include "y.tab.h"

extern FILE* outfile;
extern reg_stack rst;

int gen_head(){
    out(".LC0:\n");
    out("\t.string \"%%ld\\n\"\n");
    out(".LC1:\n");
    out("\t.string \"%%ld\"\n");
	out("\t.globl main\n");

    return 0;
}

int gen_tail(char* name){
    out("paarthunax:\n");
	out("\tpushq\t%%rbp\n");
	out("\tmovq\t%%rsp, %%rbp\n");
	out("\tcall\t%s\n", name);
    gen_func_tail();

    return 0;
}

int gen_func_head(char * name){
    out("%s:\n", name);
    out("\tpushq\t%%rbp\n");
    out("\tmovq\t%%rsp, %%rbp\n");

    return 0;
}

int gen_func_tail(){
    out("\tleave\n");
    out("\tret\n");

    return 0;
}

int gen_write(tree_t* t){
    if(t == NULL)
        return 1;

    return 0;
}

int gen_read(tree_t* t){
    if(t == NULL)
        return 1;

    return 0;
}

static int gen_op(tree_t* t, reg_t* l, reg_t* r){
    return 0;
}

static int gencase(tree_t* t){
    if( t->type == INUM)
        return 0;
    else if(!t->left && !t->right && (t->label == 1))
        return 1;
    else if(t->right && (t->right->label == 0))
        return 2;
    else if (t->left && t->right && (t->left->label < t->right->label) && (t->left->label >= 1))
        return 3;
        else if (t->left && t->right && (t->left->label >= t->right->label) && (t->right->label >= 1))
        return 4;
    else 
        return 5;
}

static int gencode_statement(tree_t* t){
    reg_t* r;

    if(!t->left && !t->left && t->label == 0){
        fprintf(stderr, "We have a problem in gencode_statement. \n");
    }

    switch(gencase(t)){
        case 0:
            out("\tmovq\t$%d, %s\n", t->attribute.ival, rst.top->name);
            break;
        case 1:
            //out("\tmovq\t%s, %s\n", t, rst.top->name);
            break;
        case 2:
            gencode_statement(t->left);
            gen_op(t, NULL, rst.top);
            break;
        case 3:
            reg_swap();
            gencode_statement(t->right);
            r = reg_pop();
            gencode_statement(t->left);
            gen_op(t, r, rst.top);
            reg_push(r);
            reg_swap();
            break;
        case 4:
            gencode_statement(t->left);
            r = reg_pop();
            gencode_statement(t->right);
            gen_op(t, rst.top, r);
            reg_push(r);
            break;
        default:
            fprintf(stderr, "Wut. Gencode case 4\n");
            break;
    }
    return 0;
}

int gencode(tree_t* t){
    if(t == NULL)
        return 0;

    if(t->type == ADDOP || t->type == MULOP){
        label_tree(t);
        initialize_registers();
        gencode_statement(t);
        free_registers();
    }else{
        gencode(t->left);
        gencode(t->right);
    }
    return 0;
}