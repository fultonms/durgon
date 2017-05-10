#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"

extern int _yacout;

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

tree_t* make_id(node_t* n){
	tree_t* ptr = make_tree(ID, NULL, NULL);
	ptr->attribute.sval = n;
	return ptr;
}

tree_t* make_op(int type, int attr, tree_t* left, tree_t* right){
	tree_t* ptr = make_tree(type, left, right);
	ptr->attribute.opval = attr;
	return ptr;
}

void print_tree(tree_t* t){
	if(_yacout){
		fprintf(stderr, "------------------BEGIN_TREE------------------\n");
		print_spaced_branch(t, 0);
		fprintf(stderr, "------------------END_TREE------------------\n\n");
	}
}

void print_spaced_branch( tree_t *t, int spaces )
{
	int i;
   
	if ( t == NULL )
		return;

	for (i=0; i<spaces; i++) {
		fprintf( stderr, " ");
	}

	switch( t->type ) {
	case ID:
		fprintf( stderr, "[ID:%s]", t->attribute.sval->name );
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
	case RELOP:
		fprintf( stderr, "[RELOP:%c]", t->attribute.opval);
		break;
	case ASSIGNOP:
		fprintf( stderr, "[ASSIGNOP]");
		break;
	case IF:
		fprintf( stderr, "[IF]");
		break;
	case THEN:
		fprintf( stderr, "[THEN]");
		break;
	case ELSE:
		fprintf( stderr, "[ELSE]");
		break;		
	case WHILE:
		fprintf( stderr, "[WHILE]");
		break;
	case DO:
		fprintf( stderr, "[DO]");
		break;
	case FOR:
		fprintf( stderr, "[FOR]");
		break;
	case TO:
		fprintf( stderr, "[TO]");
		break;
	default:
		fprintf( stderr, "[UKNOWN:%d]", t->type );
		//exit(1);
	}

	fprintf( stderr, "\n" );
	print_spaced_branch( t->left, spaces+4 );
	print_spaced_branch( t->right, spaces+4 );
}

int label_tree(tree_t* t){
	assert(t != NULL );
	
	if(t->left == NULL && t->right == NULL)
		return 0;
	
	if(t->left && !t->left->left && !t->left->right){
		t->left->label = 1;
	}
	if(t->right && !t->right->left && !t->right->right){
		t->right->label = 0;
	}

	label_tree(t->left);
	label_tree(t->right);

	if(t->left->label == t->right->label){
		t->label = t->left->label + 1;
	}else{
		if(t->left->label < t->right->label)
			t->label = t->right->label;
		else
			t->label = t->left->label;
	}
	return 0;
}

void tree_recycle(tree_t* t){

}
