#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "scope.h"
#include "register.h"
#include "gencode.h"
#include "y.tab.h"

extern FILE* outfile;
extern scope_t* top;
extern reg_stack rst;

static void out_id(char * fmt, tree_t* t, char * option){
    int i;
    char output[16] = {0};

    if (t == NULL)
        fprintf(stderr, "NULL TREE IN ID PRINT\n");

    if(t->type == INUM){
        sprintf(output, "%d", t->attribute.ival);
        out(fmt, output, option);
    }

    scope_searchall(top, t->attribute.sval->name);
    if(t->attribute.sval->depth != 0){
        out("\tmovq\t(%%rbp), %%rcx\n");
        for(i = t->attribute.sval->depth -1;i != 0; i--){
            out("\tmovq\t(%%rcx), %%rcx\n");
        }
        sprintf(output, "%d(%%rcx)", t->attribute.sval->offset);
        out(fmt, output, option);

    }else{
        sprintf(output, "%d(%%rbp)", t->attribute.sval->offset);
        out(fmt, output, option);
    }

}

//The warpper for the whole assembly file.
int gen_head(){
    out(".LC0:\n");
    out("\t.string \"%%ld\\n\"\n");
    out(".LC1:\n");
    out("\t.string \"%%ld\"\n");
	out("\t.globl main\n");

    return 0;
}

//The closed wrapper for the whole assembly file.
int gen_tail(char* name){
    out("paarthunax:\n");
	out("\tpushq\t%%rbp\n");
	out("\tmovq\t%%rsp, %%rbp\n");
	out("\tcall\t%s\n", name);
    gen_func_tail();

    return 0;
}

//Wrapper for a function.
int gen_func_head(char * name){
    out("%s:\n", name);
    out("\tpushq\t%%rbp\n");
    out("\tmovq\t%%rsp, %%rbp\n");

    return 0;
}

//Close of wrapper for a function.
int gen_func_tail(){
    out("\tleave\n");
    out("\tret\n");

    return 0;
}

//Generates a call to printf to cover writes.
int gen_write(tree_t* t){
    if(t == NULL)
        return 1;

    if (t->type != COMMA) {
		out("\tmovq\t$0, %%rax\n");
		if (t->type == INUM)
			out("\tmovq\t$%d, %%rsi\n", t->attribute.ival);
		else {
			out("\tmovq\t%d(%%rbp), %%rsi\n", t->attribute.sval->offset);
		}
		out("\tmovq\t$.LC0, %%rdi\n");
		out("\tcall\tprintf\n");
		return 1;
	}

	out("\tmovq\t$0, %%rax\n");
	if (t->right->type == INUM)
		out("\tmovq\t$%d, %%rsi\n", t->right->attribute.ival);
	else if (t->right->type == ID) {
		out_id("\tmovq\t%s, %%rsi\n", t->right, NULL);
	}
	else {
		gencode(t->right);
		out("\tmovq\t%s, %%rsi\n", rst.top->name);
	}
	out("\tmovq\t$0, %%rax\n");
	out("\tmovq\t$.LC0, %%rdi\n");
	out("\tcall\tprintf\n");
	gen_write( t->left);

    return 0;
}

//Generates a call to scanf to cover read.
int gen_read(tree_t* t){
    if(t == NULL)
        return 1;

    return 0;
}

//Generates an addop instruction from the op, and the two registers.
static int gen_addop(tree_t* t, reg_t* l, reg_t* r){

    if( l == NULL){
        if( t->attribute.opval == PLUS){
            if (t-> right->type == INUM){
                out("\taddq\t$%d, %s\n", t->right->attribute.ival, r->name);
            } else{
                out_id("\taddq\t%s, %s\n", t->right, r->name);
            }
        } else if( t->attribute.opval == MINUS){
            if (t-> right->type == INUM){
                out("\tsubq\t$%d, %s\n", t->right->attribute.ival, r->name);
            } else{
                out_id("\tsubq\t%s, %s\n", t->right, r->name);
            }
        }
    }

    assert(r);
    if(t->attribute.opval == PLUS){
        out("\taddq\t%s, %s\n", l->name, r->name);
    }else{
        out("\tsubq\t%s, %s\n", l->name, r->name);
    }

    return 0;
}

//Generates a MULOP instruction from the tree and the two registers passed.
static int gen_mulop(tree_t* t, reg_t* l, reg_t* r){
    return 0;
}

//Generates the proper instruction for any operation.
static int gen_op(tree_t* t, reg_t* l, reg_t* r){
    switch(t->type){
        case ADDOP: gen_addop(t, l, r); break;
        case MULOP: gen_mulop(t, l, r); break;
        default: fprintf(stderr, "WAT BAD OP\n");
    }
    return 0;
}

//Figures out which case you are in for gencode.
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

//Classic gencode algorithim, should cover most expressions.
static int gencode_statement(tree_t* t){
    reg_t* r;
 
    if(!t->left && !t->right && t->label == 0){
        fprintf(stderr, "We have a problem in gencode_statement. \n");
    }

    switch(gencase(t)){
        case 0:
            fprintf(stderr, "Case 0\n");
            out("\tmovq\t$%d, %s\n", t->attribute.ival, rst.top->name);
            break;
        case 1:
            fprintf(stderr, "Case 1\n");
            out_id("\tmovq\t%s, %s\n", t, rst.top->name);
            break;
        case 2:
            fprintf(stderr, "Case 2\n");
            gencode_statement(t->left);
            gen_op(t, NULL, rst.top);
            break;
        case 3:
            fprintf(stderr, "Case 3\n");
            reg_swap();
            gencode_statement(t->right);
            r = reg_pop();
            gencode_statement(t->left);
            gen_op(t, r, rst.top);
            reg_push(r);
            reg_swap();
            break;
        case 4:
            fprintf(stderr, "Case 4\n");
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

//Covers all non-expression gencode eventually.
//Thats ASSIGNOP, IF, WHILE, FOR, etc.
int gencode(tree_t* t){
    if(t == NULL)
        return 0;

    if(t->type == ASSIGNOP){
        gencode(t->right);
        out("\tmovq\t%%r10, %d(%%rbp)\n", t->left->attribute.sval->offset);

    }else if(t->type == ADDOP || t->type == MULOP){
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