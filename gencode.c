#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "scope.h"
#include "gencode.h"
#include "y.tab.h"

extern FILE* outfile;

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

int gencode(tree_t* t){
    return 0;
}