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
%token ARRAY 
%token OF
%token INTEGER
%token REAL
%token FUNCTION
%token PROCEDURE
%token BBEGIN 
%token END
%token ASSIGNOP
%token IF 
%token THEN 
%token ELSE
%token WHILE 
%token DO
%token NOT
%token DOTDOT

%left ADDOP
%left MULOP

%%
start: program ; 

program:
   PROGRAM ID '(' identifier_list ')' ';'
   declarations
   subprogram_declarations
   compound_statement
   '.'
   ;

identifier_list: ID
   | identifier_list ',' ID ; 

declarations: declarations VAR identifier_list ':' type ';'
   | /* empty */ ;

type: standard_type
   | ARRAY '[' INUM DOTDOT INUM ']' OF standard_type ;

standard_type: INTEGER | REAL ;

subprogram_declarations: subprogram_declarations subprogram_declaration ';'
   | ;

subprogram_declaration: subprogram_head declarations subprogram_declarations compound_statement ;

subprogram_head: FUNCTION ID arguments ':' standard_type ';'
   | PROCEDURE ID arguments ';';

arguments: '(' parameter_list ')'
   | ;

parameter_list: identifier_list ':' type
   | parameter_list ';' identifier_list ':' type;

compound_statement: BBEGIN optional_statements END ;

optional_statements: statement_list
   | ;

statement_list: statement
   | statement_list ';' statement;

statement: variable ASSIGNOP expression
   | procedure_statement
   | compound_statement
   | IF expression THEN statement ELSE statement
   | WHILE expression DO statement ; 

variable: ID
   | ID '[' expression ']';

procedure_statement: ID
   | ID '(' expression_list ')';

expression_list: expression
   | expression_list ',' expression ;

expression: simple_expression
   | simple_expression RELOP simple_expression ;

simple_expression: term
   | ADDOP term
   | simple_expression ADDOP term ;

term : factor                  
   | term MULOP factor         
   ;

factor: ID                       {$$ = make_tree(NAME, NULL, NULL); $$->attribute.sval = $1; }
   | ID '(' expression_list ')'  
   | ID '[' expression ']'       
   | INUM                        {$$ = make_tree(INUM, NULL, NULL); $$->attribute.ival = $1;}
   | RNUM                        {$$ = make_tree(RNUM, NULL, NULL); $$->attribute.rval = $1;}
   | '(' expression ')'          {$$ = $2;}
   | NOT factor                  
   ;
%%

int main(int argc, char** argv)
{
   yyparse();
}

void yyerror(char * s){
   fprintf(stderr, "%s\n", s);
}
