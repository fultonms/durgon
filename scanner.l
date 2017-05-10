%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "y.tab.h"

//Inspired by macros by richteer and exm.
#define SHOW(...) _lexout&&fprintf(stderr,__VA_ARGS__)
#define LEX(a) _lexout&&fprintf(stderr,"[%s]\n",#a);return a
#define LEXY(a) _lexout&&fprintf(stderr,"[%s{%s}]\n",#a,yytext);return a

extern int _lexout;

int line = 1;
%}

id    [a-zA-Z][a-zA-Z0-9]*
inum  [0-9]+
rnum  [0-9]+"."[0-9]+
whitespace  [ \t]
comment \(\*.*\*\)

%%
"program"   {LEX(PROGRAM);}
"var"       {LEX(VAR);}
"array"     {LEX(ARRAY);}
"of"        {LEX(OF);}
"integer"   {LEX(INTEGER);}
"real"      {LEX(REAL);}
"function"  {LEX(FUNCTION);}
"procedure" {LEX(PROCEDURE);}
"begin"     {LEX(BBEGIN);}
"end"       {LEX(END);}
":="        {LEX(ASSIGNOP);}
"if"        {LEX(IF);}
"then"      {LEX(THEN);}
"else"      {LEX(ELSE);}
"while"     {LEX(WHILE);}
"do"        {LEX(DO);}
"for"       {LEX(FOR);}
"to"        {LEX(TO);}

"<"|">"|"<="|">="|"="|"<>" {yylval.opval = yytext[0]; LEXY(RELOP);}
"+"|"-"|"or"               {yylval.opval = yytext[0]; LEXY(ADDOP);}
"*"|"/"|"and"              {yylval.opval = yytext[0]; LEXY(MULOP);}
"not"                      {LEX(NOT);}
".."                       {LEX(DOTDOT);}

{id}            {yylval.sval = strdup(yytext); LEXY(ID);}
{inum}          {yylval.ival = atoi(yytext); LEXY(INUM);}
{rnum}          {yylval.rval = atof(yytext); LEXY(RNUM);}
{whitespace}    { ; }
{comment}       { ; }
\n              { SHOW("[EOL]\n" ); line++;}
.               { SHOW("{%c}\n", yytext[0]); return yytext[0];}
%%
