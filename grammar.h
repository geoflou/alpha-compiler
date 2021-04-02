/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_GRAMMAR_H_INCLUDED
# define YY_YY_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    INTEGER = 259,                 /* INTEGER  */
    REAL = 260,                    /* REAL  */
    STRING = 261,                  /* STRING  */
    IF = 262,                      /* IF  */
    ELSE = 263,                    /* ELSE  */
    WHILE = 264,                   /* WHILE  */
    FOR = 265,                     /* FOR  */
    FUNCTION = 266,                /* FUNCTION  */
    RETURN = 267,                  /* RETURN  */
    BREAK = 268,                   /* BREAK  */
    CONTINUE = 269,                /* CONTINUE  */
    LOCAL_KEYWORD = 270,           /* LOCAL_KEYWORD  */
    TRUE = 271,                    /* TRUE  */
    FALSE = 272,                   /* FALSE  */
    NIL = 273,                     /* NIL  */
    WHITESPACE = 274,              /* WHITESPACE  */
    SEMICOLON = 275,               /* SEMICOLON  */
    COLON = 276,                   /* COLON  */
    COMMA = 277,                   /* COMMA  */
    DOUBLE_COLON = 278,            /* DOUBLE_COLON  */
    LEFT_BRACKET = 279,            /* LEFT_BRACKET  */
    RIGHT_BRACKET = 280,           /* RIGHT_BRACKET  */
    LEFT_BRACE = 281,              /* LEFT_BRACE  */
    RIGHT_BRACE = 282,             /* RIGHT_BRACE  */
    LEFT_PARENTHESIS = 283,        /* LEFT_PARENTHESIS  */
    RIGHT_PARENTHESIS = 284,       /* RIGHT_PARENTHESIS  */
    OPERATOR_ASSIGN = 285,         /* OPERATOR_ASSIGN  */
    OR = 286,                      /* OR  */
    AND = 287,                     /* AND  */
    OPERATOR_EQ = 288,             /* OPERATOR_EQ  */
    OPERATOR_NEQ = 289,            /* OPERATOR_NEQ  */
    OPERATOR_GRT = 290,            /* OPERATOR_GRT  */
    OPERATOR_LES = 291,            /* OPERATOR_LES  */
    OPERATOR_GRE = 292,            /* OPERATOR_GRE  */
    OPERATOR_LEE = 293,            /* OPERATOR_LEE  */
    OPERATOR_PLUS = 294,           /* OPERATOR_PLUS  */
    OPERATOR_MINUS = 295,          /* OPERATOR_MINUS  */
    OPERATOR_MUL = 296,            /* OPERATOR_MUL  */
    OPERATOR_DIV = 297,            /* OPERATOR_DIV  */
    OPERATOR_MOD = 298,            /* OPERATOR_MOD  */
    NOT = 299,                     /* NOT  */
    OPERATOR_PP = 300,             /* OPERATOR_PP  */
    OPERATOR_MM = 301,             /* OPERATOR_MM  */
    DOT = 302,                     /* DOT  */
    DOUBLE_DOT = 303               /* DOUBLE_DOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ID 258
#define INTEGER 259
#define REAL 260
#define STRING 261
#define IF 262
#define ELSE 263
#define WHILE 264
#define FOR 265
#define FUNCTION 266
#define RETURN 267
#define BREAK 268
#define CONTINUE 269
#define LOCAL_KEYWORD 270
#define TRUE 271
#define FALSE 272
#define NIL 273
#define WHITESPACE 274
#define SEMICOLON 275
#define COLON 276
#define COMMA 277
#define DOUBLE_COLON 278
#define LEFT_BRACKET 279
#define RIGHT_BRACKET 280
#define LEFT_BRACE 281
#define RIGHT_BRACE 282
#define LEFT_PARENTHESIS 283
#define RIGHT_PARENTHESIS 284
#define OPERATOR_ASSIGN 285
#define OR 286
#define AND 287
#define OPERATOR_EQ 288
#define OPERATOR_NEQ 289
#define OPERATOR_GRT 290
#define OPERATOR_LES 291
#define OPERATOR_GRE 292
#define OPERATOR_LEE 293
#define OPERATOR_PLUS 294
#define OPERATOR_MINUS 295
#define OPERATOR_MUL 296
#define OPERATOR_DIV 297
#define OPERATOR_MOD 298
#define NOT 299
#define OPERATOR_PP 300
#define OPERATOR_MM 301
#define DOT 302
#define DOUBLE_DOT 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "grammar.y"

    int intVal;
    char *strVal;
    double doubleVal;

#line 168 "grammar.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_H_INCLUDED  */
