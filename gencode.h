#include "tree.h"

#define out(...) fprintf(outfile, __VA_ARGS__)

int gen_head();
int gen_tail(char * name);
int gen_func_head(char * name);
int gen_func_tail();


//I/O functions
int gen_write(tree_t* t);
int gen_read(tree_t* t);

int gencode(tree_t* t);