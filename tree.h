#ifndef TREE_H
#define TREE_H
#include "node.h"

typedef struct tree_s {
	int type;	/* NUM, ADDOP, MULOP */
	union {
		int ival;	/* INUM */
        float rval; /*RNUM*/
		node_t* sval;	/* NAME */
		int opval;	/* ADDOP, MULOP, RELOP*/
      
	} attribute;
	int label;
	struct tree_s *left, *right;
} 
tree_t;


tree_t* make_tree( int type, tree_t* left, tree_t* right );
tree_t* make_inum(int i);
tree_t* make_rnum(float r);
tree_t* make_id(node_t* n);
tree_t* make_op(int type, int attr, tree_t* left, tree_t* right);

int label_tree(tree_t* t);
tree_t* type_tree(tree_t* t, int type);

void print_tree( tree_t *t);
void print_spaced_branch(tree_t* t, int spaces);

void tree_recycle(tree_t* t);

#endif

