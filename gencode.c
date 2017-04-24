#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "scope.h"
#include "gencode.h"
#include "y.tab.h"

extern FILE* outfile;

int genHead(){
    out(".LC0:\n");
    out("\t.string \"%%ld\\n\"\n");
    out(".LC1:\n");
    out("\t.string \"%%ld\"\n");
	out("\t.globl main\n");

    return 0;
}

int genTail(char* name){
    out("main:\n");
	out("\tpushq\t%%rbp\n");
	out("\tmovq\t%%rsp, %%rbp\n");
	out("\tcall\t%s\n", name);

    return 0;
}