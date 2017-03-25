#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"


tree_t* make_tree( int type, tree_t *left, tree_t *right )
{
	tree_t *ptr = malloc(sizeof(tree_t));
	assert( ptr != NULL );

	ptr->type = type;
	ptr->left = left;
	ptr->right = right;

	return ptr;
}

tree_t* make_inum(int i){
	tree_t * ptr = make_tree(INUM, NULL, NULL);
	ptr->attribute.ival = i;
	return ptr;

}

tree_t* make_rnum(float r){
	tree_t * ptr = make_tree(RNUM, NULL, NULL);
	ptr->attribute.rval = r;
	return ptr;
}

tree_t* make_name(char* s){
	tree_t* ptr = make_tree(ID, NULL, NULL);
	ptr->attribute.sval = s;
	return ptr;
}

tree_t* make_op(int type, int attr, tree_t* left, tree_t* right){
	tree_t* ptr = make_tree(type, left, right);
	ptr->attribute.opval = attr;
	return ptr;
}


void print_tree( tree_t *t, int spaces )
{
	int i;

	if ( t == NULL )
		return;

	for (i=0; i<spaces; i++) {
		fprintf( stderr, " ");
	}

	switch( t->type ) {
	case ID:
		fprintf( stderr, "[ID:%s]", t->attribute.sval );
		break;
	case INUM:
		fprintf( stderr, "[INUM:%d]", t->attribute.ival );
		break;
   case RNUM:
      fprintf( stderr, "[RNUM:%f]", t->attribute.rval );
	case ADDOP:
		fprintf( stderr, "[ADDOP:%c]", t->attribute.opval );
		break;
	case MULOP:
		fprintf( stderr, "[MULOP:%c]", t->attribute.opval );
		break;
	default:
		fprintf( stderr, "[UKNOWN]" );
		exit(1);
	}
	fprintf( stderr, "\n" );

	print_tree( t->left, spaces+4 );
	print_tree( t->right, spaces+4 );

}

int eval_tree( tree_t *t )
{
}








