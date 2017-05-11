%{
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include "tree.h"
#include "scope.h"
#include "semmantic.h"
#include "y.tab.h"
#include "gencode.h"

extern scope_t* top;
extern int line;

int offsetMode = 0;
int _lexout = 0;
int _yacout = 1;

FILE* outfile;
#define ERROR(fmt, ...) printf("\x1B[31m" fmt "\x1B[0m\n",  __VA_ARGS__)
%}

%union {
   int ival;
   float rval;
   int opval;
   char* sval;

   tree_t* tval;
}

%token   <ival>  INUM 
%token   <rval>  RNUM
%token   <sval>  ID

%token   ASSIGNOP
%token   <opval> RELOP
%token   <opval> ADDOP
%token   <opval> MULOP
%token   NOT

%token LT LE GT GE EQ NE 
%token OR PLUS MINUS 
%token AND STAR SLASH

%token PROGRAM
%token VAR INTEGER REAL
%token ARRAY OF DOTDOT
%token FUNCTION PROCEDURE
%token BBEGIN END
%token IF THEN ELSE
%token WHILE DO
%token FOR TO

%token FUNCTION_CALL
%token PROCEDURE_CALL
%token ARRAY_ACCESS
%token COMMA

%left ADDOP
%left MULOP

%type <tval> expression
%type <tval> expression_list
%type <tval> identifier_list
%type <tval> declarations
%type <tval> simple_expression
%type <tval> statement
%type <ival> standard_type
%type <ival> type
%type <tval> term
%type <tval> factor
%type <sval> variable

%%
start: program ; 

program: {gen_head(); top = scope_push(top, PROCEDURE);}
   PROGRAM ID '(' identifier_list ')' ';' 
   declarations 
   subprogram_declarations {gen_func_head($3);} 
   compound_statement {gen_func_tail();}
   '.' { {gen_tail($3); top = scope_pop(top);}}
   ;

identifier_list: ID         {$$ = make_tree(COMMA, NULL, make_id(scope_insert(top, $1)));}
   | identifier_list ',' ID {$$ = make_tree(COMMA, $1, make_id(scope_insert(top, $3))); } 
   ; 

declarations: declarations VAR identifier_list ':' type ';'
                 {type_tree($3, $5); $$ = $3;}
   | /* empty */ {$$ = NULL;} 
   ;

type: standard_type { $$ = $1;}
   | ARRAY '[' INUM DOTDOT INUM ']' OF standard_type 
   ;

standard_type: INTEGER {$$ = INUM; } 
    | REAL {$$ = RNUM; }
    ;

subprogram_declarations: subprogram_declarations subprogram_declaration ';'
   | ;

subprogram_declaration: 
    subprogram_head 
    declarations 
    subprogram_declarations
    compound_statement { top = scope_pop(top);}
    ;

subprogram_head: 
    FUNCTION ID {
                    scope_insert(top, $2);
                    scope_search(top, $2)->type = FUNCTION;
                    top = scope_push(top, FUNCTION);
                }
    arguments ':' standard_type ';'
                {
                    node_t* n = scope_search(top->next, $2);
                    argumentize(top, n, $6);
                }
    | PROCEDURE ID {
                    scope_insert(top, $2);
                    scope_search(top, $2)->type = PROCEDURE;
                    top = scope_push(top, PROCEDURE);
                    }
    arguments ';'{
                    node_t* n = scope_search(top->next, $2);
                    argumentize(top, n, 0);
                 }
    ;

arguments:                 
    '(' parameter_list ')'  
    | ;

parameter_list: identifier_list ':' type {type_tree($1,$3);}
   | parameter_list ';' identifier_list ':' type;

compound_statement: BBEGIN optional_statements END ;

optional_statements: statement_list
   | ;

statement_list: statement
   | statement_list ';' statement;

statement: variable ASSIGNOP expression 
            {
                tree_t* t;
                t = make_tree(ASSIGNOP, make_id(scope_searchall(top, $1)), $3);
                print_tree(t); 
                assert(!check_tree(t));
                //gencode(t);
                $$ = t;
                //tree_recycle(t);
                
            }
   | procedure_statement
   | compound_statement
   | IF expression THEN statement ELSE statement
   {
       tree_t* t = NULL;
       t = make_tree(IF, $2, make_tree(THEN, $4, make_tree(ELSE, $6, NULL)));
       assert(!check_tree(t));
       print_tree(t);
   }
   | WHILE expression DO statement
   {
       tree_t* t = NULL;
       t = make_tree(WHILE, $2, make_tree(DO, $4, NULL));
       assert(!check_tree(t));
       print_tree(t);
   }

   | FOR variable ASSIGNOP expression TO expression DO statement
   {
       tree_t* t = NULL;
       t = make_tree(FOR, make_tree(ASSIGNOP, make_id(scope_searchall(top, $2)), $4), make_tree(TO, $6, make_tree(DO, $8, NULL)));
       assert(!check_tree(t));
       print_tree(t);
   }
   ;   

variable: ID { $$ = $1; }
   //| ID '[' expression ']'
   ;

procedure_statement: ID 
   | ID '(' expression_list ')' 
        {
            if(strcmp("read", $1) == 0){
                gen_read($3);
            }else if(strcmp("write", $1) == 0){
                gen_write($3);
            }else{
                tree_t* t = make_tree(PROCEDURE_CALL, make_id(scope_searchall(top, $1)), $3);
                assert(!check_tree(t));
                tree_recycle(t);
            }
        }
   ;

expression_list: expression           { $$ = make_tree(COMMA, NULL, $1);}
   | expression_list ',' expression  { $$ = make_tree(COMMA, $1, $3); }
   ; 

expression: simple_expression       { $$ = $1;}
   | simple_expression RELOP simple_expression     { $$ = make_op(RELOP, $2, $1, $3); }
   ;

simple_expression: term             { $$ = $1; }
   //| ADDOP term                     
   | simple_expression ADDOP term   { 
                                        tree_t* t = make_op(ADDOP, $2, $1, $3); 
                                        assert(!check_tree(t)); 
                                        $$ = t;
                                    }
   ;

term : factor                       { $$ = $1; }     
   | term MULOP factor              { 
                                        tree_t* t = make_op(MULOP, $2, $1, $3); 
                                        assert(!check_tree(t)); 
                                        $$ = t;
                                    }
   ;

factor: ID                          { $$ = make_id(scope_searchall(top,$1));} 
   | ID '(' expression_list ')'     { 
                                        tree_t* t = make_tree(FUNCTION_CALL, make_id(scope_search(top, $1)), $3); 
                                        assert(!check_tree(t)); 
                                        $$ = t;
                                    }  
   | ID '[' expression ']'          { $$ = make_tree(ARRAY_ACCESS, make_id(scope_search(top, $1)), $3); }
   | INUM                           { $$ = make_inum($1); }         
   | RNUM                           { $$ = make_rnum($1); }
   | '(' expression ')'             { $$ = $2; }
   | NOT factor                     { $$ = make_tree(NOT, NULL, NULL); }
   ;
%%

int main(int argc, char** argv)
{

    outfile = fopen("paarthurnax.s", "w");
    yyparse();
    fclose(outfile);

    return 0;
}

int yyerror(char * s){
   ERROR("Error at line %d: %s", line, s);
   return 1;
}
