%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"

int line = 1;
%}

id    [a-zA-Z][a-zA-Z0-9]*
inum  [0-9]+
rnum  [0-9]+"."[0-9]+
whitespace  [ \t]
comment \(\*.*\*\)

%%
"program"   {fprintf(stderr, "PROGRAM\n"); return PROGRAM;}
"var"       {fprintf(stderr, "VAR\n"); return VAR;}
"array"     {fprintf(stderr, "ARRAY\n"); return ARRAY;}
"of"        {fprintf(stderr, "OF\n"); return OF;}
"integer"   {fprintf(stderr, "INTEGER\n"); return INTEGER;}
"real"      {fprintf(stderr, "REAL\n"); return REAL;}
"function"  {fprintf(stderr, "FUNCTION\n"); return FUNCTION;}
"procedure" {fprintf(stderr, "PROCEDURE\n"); return PROCEDURE;}
"begin"     {fprintf(stderr, "BBEGIN\n"); return BBEGIN;}
"end"       {fprintf(stderr, "END\n"); return END;}
":="        {fprintf(stderr, "ASSIGNOP\n"); return ASSIGNOP;}
"if"        {fprintf(stderr, "IF\n"); return IF;}
"then"      {fprintf(stderr, "THEN\n"); return THEN;}
"else"      {fprintf(stderr, "ELSE\n"); return ELSE;}
"while"     {fprintf(stderr, "WHILE\n"); return WHILE;}
"do"        {fprintf(stderr, "DO\n"); return DO;}
"for"       {fprintf(stderr, "FOR\n"); return FOR;}
"to"        {fprintf(stderr, "TO\n"); return TO;}

"<"|">"|"<="|">="|"="|"<>" {fprintf(stderr, "[RELOP:%c]\n", yylval.opval = yytext[0]); return RELOP;}
"+"|"-"|"or"               {fprintf(stderr, "[ADDOP:%c]\n", yylval.opval = yytext[0]); return ADDOP;}
"*"|"/"|"and"              {fprintf(stderr, "[MULOP:%c]\n", yylval.opval = yytext[0]); return MULOP;}
"not"                      {fprintf(stderr, "NOT\n"); return NOT;}
".."                       {fprintf(stderr, "DOTDOT\n"); return DOTDOT;}

{id}            {fprintf(stderr, "[ID:%s]\n", yylval.sval = strdup(yytext)); return ID;}
{inum}          {fprintf(stderr, "[INUM:%d]\n", yylval.ival = atoi(yytext)); return INUM;}
{rnum}          {fprintf(stderr, "[RNUM:%f]\n", yylval.rval = atof(yytext)); return RNUM;}
{whitespace}    { ; }
{comment}       { ; }
\n              { fprintf( stderr, "[EOL]\n" ); line++;}
.               { fprintf( stderr, "{%c}\n", yytext[0] ); return yytext[0]; }
%%
