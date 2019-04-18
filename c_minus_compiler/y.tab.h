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
    IF = 258,
    ELSE = 259,
    INT = 260,
    RETURN = 261,
    VOID = 262,
    ID = 263,
    NUM = 264,
    ASSIGN = 265,
    EQ = 266,
    LT = 267,
    LE = 268,
    NE = 269,
    GT = 270,
    GE = 271,
    COMMA = 272,
    SEMI = 273,
    PLUS = 274,
    MINUS = 275,
    TIMES = 276,
    OVER = 277,
    LBRACE = 278,
    RBRACE = 279,
    LCURLY = 280,
    RCURLY = 281,
    LPAREN = 282,
    RPAREN = 283,
    ERROR = 284,
    THEN = 285,
    END = 286,
    WHILE = 287,
    REPEAT = 288,
    UNTIL = 289,
    READ = 290,
    WRITE = 291
  };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define INT 260
#define RETURN 261
#define VOID 262
#define ID 263
#define NUM 264
#define ASSIGN 265
#define EQ 266
#define LT 267
#define LE 268
#define NE 269
#define GT 270
#define GE 271
#define COMMA 272
#define SEMI 273
#define PLUS 274
#define MINUS 275
#define TIMES 276
#define OVER 277
#define LBRACE 278
#define RBRACE 279
#define LCURLY 280
#define RCURLY 281
#define LPAREN 282
#define RPAREN 283
#define ERROR 284
#define THEN 285
#define END 286
#define WHILE 287
#define REPEAT 288
#define UNTIL 289
#define READ 290
#define WRITE 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
