/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    RELOP = 258,
    ADDOP = 259,
    MULOP = 260,
    INUM = 261,
    RNUM = 262,
    ID = 263,
    PROGRAM = 264,
    VAR = 265,
    ARRAY = 266,
    OF = 267,
    INTEGER = 268,
    REAL = 269,
    FUNCTION = 270,
    PROCEDURE = 271,
    BBEGIN = 272,
    END = 273,
    ASSIGNOP = 274,
    IF = 275,
    THEN = 276,
    ELSE = 277,
    WHILE = 278,
    DO = 279,
    NOT = 280,
    DOTDOT = 281
  };
#endif
/* Tokens.  */
#define RELOP 258
#define ADDOP 259
#define MULOP 260
#define INUM 261
#define RNUM 262
#define ID 263
#define PROGRAM 264
#define VAR 265
#define ARRAY 266
#define OF 267
#define INTEGER 268
#define REAL 269
#define FUNCTION 270
#define PROCEDURE 271
#define BBEGIN 272
#define END 273
#define ASSIGNOP 274
#define IF 275
#define THEN 276
#define ELSE 277
#define WHILE 278
#define DO 279
#define NOT 280
#define DOTDOT 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "parser.y" /* yacc.c:1909  */

   int ival;
   float rval;
   int opval;
   char* sval;

   tree_t* tval;

#line 115 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
