%{
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"tree.h"
#include"y.tab.h"
%}

%union {
   int ival;
   double rval;
   int opval;
   char* sval;

   tree_t *tval;
}

%token   <opval> RELOP
%token   <opval> ADDOP
%token   <opval> MULOP
%token   <ival>  INUM 
%token   <rval>  RNUM
%token   <sval>  ID

%token PROGRAM
%token VAR
%token ARRAY OF
%token INTEGER
%token REAL
%token FUNCTION
%token PROCEDURE
%token BEGIN END
%token IF THEN ELSE
%token WHILE DO
%token NOT
%token DOTDOT

%left ADDOP
%left MULOP

%%


%%

main()
   yyparse();
   fprintf(stderr, "ENDING");
}
