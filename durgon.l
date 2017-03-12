%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"
%}

id    [a-zA-Z][a-zA-Z0-9]*
inum  [0-9]+
rnum  [0-9]+"."[0-9]+
whitespace  [ \t]

%%
"program"   {fprintf(stderr, "PROGRAM"); return PROGRAM;}
"var"       {fprintf(stderr, "VAR"); return VAR;}
"array"     {fprintf(stderr, "ARRAY"); return ARRAY;}
"of"        {fprintf(stderr, "OF"); return OF;}
"integer"   {fprintf(stderr, "INTEGER"); return INTEGER;}
"real"      {fprintf(stderr, "REAL"); return REAL;}
"function"  {fprintf(stderr, "FUNCTION"); return FUNCTION;}
"procedure" {fprintf(stderr, "PROCEDURE"); return PROCEDURE;}
"begin"     {fprintf(stderr, "BBEGIN"); return BBEGIN;}
"end"       {fprintf(stderr, "END"); return END;}
":="        {fprintf(stderr, "ASSIGNOP"); return ASSIGNOP;}
"if"        {fprintf(stderr, "IF"); return IF;}
"then"      {fprintf(stderr, "THEN"); return THEN;}
"else"      {fprintf(stderr, "ELSE"); return ELSE;}
"while"     {fprintf(stderr, "WHILE"); return WHILE;}
"do"        {fprintf(stderr, "DO"); return DO;}

"<"|">"|"<="|">="|"="|"<>" {fprintf(stderr, "[RELOP:%c]", yylval.opval = yytext[0]); return RELOP;}
"+"|"-"|"or"               {fprintf(stderr, "[ADDOP:%c]", yylval.opval = yytext[0]); return ADDOP;}
"*"|"/"|"and"              {fprintf(stderr, "[MULOP:%c]", yylval.opval = yytext[0]); return MULOP;}
"not"                      {fprintf(stderr, "NOT"); return NOT;}
".."                       {fprintf(stderr, "DOTDOT"); return DOTDOT;}

{id}        {fprintf(stderr, "[ID:%s]", yylval.sval = strdup(yytext)); return ID;}
{inum}      {fprintf(stderr, "[INUM:%d]", yylval.ival = atoi(yytext)); return INUM;}
{rnum}      {fprintf(stderr, "[RNUM:%f]", yylval.rval = atof(yytext)); return RNUM;}
{whitespace}   { ; }
\n          { fprintf( stderr, "[EOL]" );}
.           { fprintf( stderr, "{%c}", yytext[0] ); return yytext[0]; }
%%
