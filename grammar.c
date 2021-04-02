/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "grammar.y"

    #include <stdlib.h>
    #include <stdio.h>
    #include "utilities.h"
    #include "grammar.h"

    int yyerror(char* message);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    int scope = 0;

    char* currFunc;

    Function* temp_func;
    int arg_index = 0;

    int anonFuncCounter = 0;
    int loopFlag = 0;
    int funcFlag = 0;
    int memberflag = 0;

#line 97 "grammar.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "grammar.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_INTEGER = 4,                    /* INTEGER  */
  YYSYMBOL_REAL = 5,                       /* REAL  */
  YYSYMBOL_STRING = 6,                     /* STRING  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_FUNCTION = 11,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_BREAK = 13,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 14,                  /* CONTINUE  */
  YYSYMBOL_LOCAL_KEYWORD = 15,             /* LOCAL_KEYWORD  */
  YYSYMBOL_TRUE = 16,                      /* TRUE  */
  YYSYMBOL_FALSE = 17,                     /* FALSE  */
  YYSYMBOL_NIL = 18,                       /* NIL  */
  YYSYMBOL_WHITESPACE = 19,                /* WHITESPACE  */
  YYSYMBOL_SEMICOLON = 20,                 /* SEMICOLON  */
  YYSYMBOL_COLON = 21,                     /* COLON  */
  YYSYMBOL_COMMA = 22,                     /* COMMA  */
  YYSYMBOL_DOUBLE_COLON = 23,              /* DOUBLE_COLON  */
  YYSYMBOL_LEFT_BRACKET = 24,              /* LEFT_BRACKET  */
  YYSYMBOL_RIGHT_BRACKET = 25,             /* RIGHT_BRACKET  */
  YYSYMBOL_LEFT_BRACE = 26,                /* LEFT_BRACE  */
  YYSYMBOL_RIGHT_BRACE = 27,               /* RIGHT_BRACE  */
  YYSYMBOL_LEFT_PARENTHESIS = 28,          /* LEFT_PARENTHESIS  */
  YYSYMBOL_RIGHT_PARENTHESIS = 29,         /* RIGHT_PARENTHESIS  */
  YYSYMBOL_OPERATOR_ASSIGN = 30,           /* OPERATOR_ASSIGN  */
  YYSYMBOL_OR = 31,                        /* OR  */
  YYSYMBOL_AND = 32,                       /* AND  */
  YYSYMBOL_OPERATOR_EQ = 33,               /* OPERATOR_EQ  */
  YYSYMBOL_OPERATOR_NEQ = 34,              /* OPERATOR_NEQ  */
  YYSYMBOL_OPERATOR_GRT = 35,              /* OPERATOR_GRT  */
  YYSYMBOL_OPERATOR_LES = 36,              /* OPERATOR_LES  */
  YYSYMBOL_OPERATOR_GRE = 37,              /* OPERATOR_GRE  */
  YYSYMBOL_OPERATOR_LEE = 38,              /* OPERATOR_LEE  */
  YYSYMBOL_OPERATOR_PLUS = 39,             /* OPERATOR_PLUS  */
  YYSYMBOL_OPERATOR_MINUS = 40,            /* OPERATOR_MINUS  */
  YYSYMBOL_OPERATOR_MUL = 41,              /* OPERATOR_MUL  */
  YYSYMBOL_OPERATOR_DIV = 42,              /* OPERATOR_DIV  */
  YYSYMBOL_OPERATOR_MOD = 43,              /* OPERATOR_MOD  */
  YYSYMBOL_NOT = 44,                       /* NOT  */
  YYSYMBOL_OPERATOR_PP = 45,               /* OPERATOR_PP  */
  YYSYMBOL_OPERATOR_MM = 46,               /* OPERATOR_MM  */
  YYSYMBOL_DOT = 47,                       /* DOT  */
  YYSYMBOL_DOUBLE_DOT = 48,                /* DOUBLE_DOT  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_program = 50,                   /* program  */
  YYSYMBOL_set = 51,                       /* set  */
  YYSYMBOL_stmt = 52,                      /* stmt  */
  YYSYMBOL_expr = 53,                      /* expr  */
  YYSYMBOL_term = 54,                      /* term  */
  YYSYMBOL_55_1 = 55,                      /* $@1  */
  YYSYMBOL_56_2 = 56,                      /* $@2  */
  YYSYMBOL_assignexpr = 57,                /* assignexpr  */
  YYSYMBOL_58_3 = 58,                      /* $@3  */
  YYSYMBOL_primary = 59,                   /* primary  */
  YYSYMBOL_lvalue = 60,                    /* lvalue  */
  YYSYMBOL_member = 61,                    /* member  */
  YYSYMBOL_call = 62,                      /* call  */
  YYSYMBOL_63_4 = 63,                      /* $@4  */
  YYSYMBOL_callsuffix = 64,                /* callsuffix  */
  YYSYMBOL_normcall = 65,                  /* normcall  */
  YYSYMBOL_methodcall = 66,                /* methodcall  */
  YYSYMBOL_elist = 67,                     /* elist  */
  YYSYMBOL_objectdef = 68,                 /* objectdef  */
  YYSYMBOL_indexed = 69,                   /* indexed  */
  YYSYMBOL_indexedelem = 70,               /* indexedelem  */
  YYSYMBOL_block = 71,                     /* block  */
  YYSYMBOL_72_5 = 72,                      /* $@5  */
  YYSYMBOL_73_6 = 73,                      /* $@6  */
  YYSYMBOL_funcdef = 74,                   /* funcdef  */
  YYSYMBOL_75_7 = 75,                      /* $@7  */
  YYSYMBOL_76_8 = 76,                      /* $@8  */
  YYSYMBOL_77_9 = 77,                      /* $@9  */
  YYSYMBOL_78_10 = 78,                     /* $@10  */
  YYSYMBOL_const = 79,                     /* const  */
  YYSYMBOL_idlist = 80,                    /* idlist  */
  YYSYMBOL_81_11 = 81,                     /* $@11  */
  YYSYMBOL_ifstmt = 82,                    /* ifstmt  */
  YYSYMBOL_whilestmt = 83,                 /* whilestmt  */
  YYSYMBOL_84_12 = 84,                     /* $@12  */
  YYSYMBOL_forstmt = 85,                   /* forstmt  */
  YYSYMBOL_86_13 = 86,                     /* $@13  */
  YYSYMBOL_returnstmt = 87                 /* returnstmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  69
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   560

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  100
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  183

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,    78,    81,    82,    85,    86,    87,    88,
      89,    95,   101,   107,   108,   109,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   129,   130,   131,   132,   142,   142,   151,   161,   161,
     174,   177,   177,   194,   199,   200,   201,   202,   203,   206,
     226,   230,   236,   242,   243,   244,   245,   248,   249,   249,
     262,   266,   269,   272,   275,   278,   279,   280,   283,   284,
     287,   288,   291,   294,   294,   299,   299,   306,   313,   306,
     369,   379,   369,   440,   441,   442,   443,   444,   445,   448,
     468,   468,   487,   490,   491,   494,   494,   497,   497,   501,
     502
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "INTEGER",
  "REAL", "STRING", "IF", "ELSE", "WHILE", "FOR", "FUNCTION", "RETURN",
  "BREAK", "CONTINUE", "LOCAL_KEYWORD", "TRUE", "FALSE", "NIL",
  "WHITESPACE", "SEMICOLON", "COLON", "COMMA", "DOUBLE_COLON",
  "LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_BRACE", "RIGHT_BRACE",
  "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "OPERATOR_ASSIGN", "OR", "AND",
  "OPERATOR_EQ", "OPERATOR_NEQ", "OPERATOR_GRT", "OPERATOR_LES",
  "OPERATOR_GRE", "OPERATOR_LEE", "OPERATOR_PLUS", "OPERATOR_MINUS",
  "OPERATOR_MUL", "OPERATOR_DIV", "OPERATOR_MOD", "NOT", "OPERATOR_PP",
  "OPERATOR_MM", "DOT", "DOUBLE_DOT", "$accept", "program", "set", "stmt",
  "expr", "term", "$@1", "$@2", "assignexpr", "$@3", "primary", "lvalue",
  "member", "call", "$@4", "callsuffix", "normcall", "methodcall", "elist",
  "objectdef", "indexed", "indexedelem", "block", "$@5", "$@6", "funcdef",
  "$@7", "$@8", "$@9", "$@10", "const", "idlist", "$@11", "ifstmt",
  "whilestmt", "$@12", "forstmt", "$@13", "returnstmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
#endif

#define YYPACT_NINF (-163)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-91)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     202,  -163,  -163,  -163,  -163,    -6,  -163,  -163,    16,   106,
      12,    19,    38,  -163,  -163,  -163,  -163,    41,    30,   234,
     266,   298,   298,     2,     2,    56,   202,  -163,    65,  -163,
    -163,  -163,   -10,  -163,    -2,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,   298,    29,    32,  -163,    33,  -163,   314,
    -163,  -163,  -163,  -163,   202,    37,   298,   383,    31,    51,
      57,   451,    52,    11,  -163,    69,    -5,     1,    -5,  -163,
    -163,  -163,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,    86,    45,    46,    61,
       3,   298,   298,   298,    90,   466,   298,   298,    66,   110,
    -163,   158,  -163,   361,   298,  -163,  -163,    91,  -163,    88,
      89,   506,   154,   517,   517,   324,   324,   324,   324,    11,
      11,  -163,  -163,  -163,   419,  -163,  -163,  -163,   298,   298,
     114,  -163,  -163,  -163,   436,    96,   494,  -163,   202,   481,
     107,   110,   108,    99,  -163,   298,  -163,  -163,   298,    88,
    -163,   494,   102,   109,  -163,  -163,   127,   202,   298,   111,
     116,  -163,   402,   112,  -163,  -163,   202,  -163,   338,  -163,
     110,   119,  -163,  -163,  -163,   298,   119,  -163,  -163,   115,
    -163,   202,  -163
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    49,    84,    83,    85,     0,    95,    97,    80,     0,
       0,     0,     0,    87,    88,    86,    15,     0,    73,    67,
       0,     0,     0,     0,     0,     0,     2,     4,     0,    30,
      16,    40,    44,    52,    45,    46,    13,    14,    48,     7,
       8,     9,    10,     0,     0,     0,    77,     0,   100,     0,
      11,    12,    50,    51,     0,     0,     0,    65,     0,     0,
      70,     0,     0,    32,    33,     0,    34,     0,    37,     1,
       5,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,     0,     0,     0,    67,     0,    92,
      99,     0,    76,     0,    67,    68,    69,     0,    31,    47,
       0,    29,    28,    26,    27,    22,    24,    23,    25,    17,
      18,    21,    20,    19,     0,    53,    36,    39,     0,    67,
       0,    59,    62,    61,     0,     0,    43,    55,     0,     0,
       0,    92,    89,     0,    74,     0,    66,    71,    67,     0,
      54,    42,     0,     0,    56,    57,    93,     0,     0,     0,
       0,    81,     0,     0,    63,    64,     0,    96,     0,    78,
      92,     0,    72,    60,    94,    67,     0,    91,    82,     0,
      79,     0,    98
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -163,  -163,    93,   -24,    -9,  -163,  -163,  -163,  -163,  -163,
    -163,   -17,  -163,    26,  -163,  -163,    -8,  -163,   -89,  -163,
      47,  -163,  -162,  -163,  -163,   -19,  -163,  -163,  -163,  -163,
    -163,  -137,  -163,  -163,  -163,  -163,  -163,  -163,  -163
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    28,    29,    87,    88,    30,    89,
      31,    32,    33,    34,    90,   131,   132,   133,    58,    35,
      59,    60,    36,    54,    55,    37,    98,   176,    47,   171,
      38,   143,   160,    39,    40,    44,    41,    45,    42
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    62,    70,   135,   159,     1,    66,    68,   140,   178,
      57,    61,    63,    64,   180,   146,    85,    12,   -58,    46,
     -41,    85,    43,   -58,    91,    17,    92,    91,    93,    92,
      65,   129,    50,   177,    95,   -35,   -38,    86,   -58,    51,
     152,    52,    86,   -58,    53,    94,   110,   103,    94,    67,
      67,   130,    82,    83,    84,   -75,    69,    96,   105,   163,
      97,    99,   102,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    70,   106,   107,
       8,   109,   134,    57,   136,    71,   179,   139,    57,   125,
     126,   128,   127,   137,   141,    57,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,     1,
       2,     3,     4,   142,   156,    56,   148,   153,   149,   151,
      57,    12,    13,    14,    15,   155,    48,   158,   161,    17,
     -90,   164,    19,   167,    20,   166,   162,   129,   170,    57,
     169,   173,   174,    18,   181,   165,    21,   101,     0,   168,
      22,    23,    24,     0,   147,     0,     0,   182,     0,     0,
       0,     1,     2,     3,     4,     5,    57,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,     0,    16,     0,
       0,    17,    18,   144,    19,     0,    20,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    21,     0,
       0,     0,    22,    23,    24,     1,     2,     3,     4,     5,
       0,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,     0,    16,     0,     0,    17,    18,     0,    19,     0,
      20,     0,     0,     0,     0,     0,     0,     1,     2,     3,
       4,     0,    21,     0,     0,     0,    22,    23,    24,    12,
      13,    14,    15,     0,     0,     0,     0,    17,    56,     0,
      19,     0,    20,     0,     0,     0,     0,     0,     0,     1,
       2,     3,     4,     0,    21,     0,     0,     8,    22,    23,
      24,    12,    13,    14,    15,     0,     0,     0,     0,    17,
       0,     0,    19,     0,    20,     0,     0,     0,     0,     0,
       0,     1,     2,     3,     4,     0,    21,     0,     0,     0,
      22,    23,    24,    12,    13,    14,    15,     0,     0,     0,
       0,    17,     0,     0,    19,     0,    20,     0,     0,     0,
       0,     0,     0,     0,   100,     0,     0,     0,    21,     0,
       0,     0,    22,    23,    24,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,   175,    76,
      77,    78,    79,    80,    81,    82,    83,    84,     0,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,   104,     0,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,   172,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,   150,     0,     0,     0,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,   154,     0,     0,     0,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     108,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,   138,     0,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     157,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     -91,   -91,    76,    77,    78,    79,    80,    81,    82,    83,
      84
};

static const yytype_int16 yycheck[] =
{
       9,    20,    26,    92,   141,     3,    23,    24,    97,   171,
      19,    20,    21,    22,   176,   104,    26,    15,    28,     3,
      30,    26,    28,    28,    26,    23,    28,    26,    30,    28,
      28,    28,    20,   170,    43,    45,    46,    47,    48,    20,
     129,     3,    47,    48,     3,    47,    65,    56,    47,    23,
      24,    48,    41,    42,    43,    25,     0,    28,    27,   148,
      28,    28,    25,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,   101,    27,    22,
      11,    29,    91,    92,    93,    20,   175,    96,    97,     3,
      45,    30,    46,     3,    28,   104,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,     3,
       4,     5,     6,     3,   138,    24,    28,     3,    29,   128,
     129,    15,    16,    17,    18,    29,    20,    20,    29,    23,
      22,    29,    26,   157,    28,     8,   145,    28,    22,   148,
      29,    29,   166,    24,    29,   153,    40,    54,    -1,   158,
      44,    45,    46,    -1,   107,    -1,    -1,   181,    -1,    -1,
      -1,     3,     4,     5,     6,     7,   175,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    20,    -1,
      -1,    23,    24,    25,    26,    -1,    28,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    40,    -1,
      -1,    -1,    44,    45,    46,     3,     4,     5,     6,     7,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    20,    -1,    -1,    23,    24,    -1,    26,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    40,    -1,    -1,    -1,    44,    45,    46,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    23,    24,    -1,
      26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    40,    -1,    -1,    11,    44,    45,
      46,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    44,    45,    46,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    20,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    27,    -1,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    27,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    20,    23,    24,    26,
      28,    40,    44,    45,    46,    50,    51,    52,    53,    54,
      57,    59,    60,    61,    62,    68,    71,    74,    79,    82,
      83,    85,    87,    28,    84,    86,     3,    77,    20,    53,
      20,    20,     3,     3,    72,    73,    24,    53,    67,    69,
      70,    53,    74,    53,    53,    28,    60,    62,    60,     0,
      52,    20,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    26,    47,    55,    56,    58,
      63,    26,    28,    30,    47,    53,    28,    28,    75,    28,
      20,    51,    25,    53,    22,    27,    27,    22,    29,    29,
      74,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,     3,    45,    46,    30,    28,
      48,    64,    65,    66,    53,    67,    53,     3,    29,    53,
      67,    28,     3,    80,    25,    21,    67,    69,    28,    29,
      27,    53,    67,     3,    27,    29,    52,    29,    20,    80,
      81,    29,    53,    67,    29,    65,     8,    52,    53,    29,
      22,    78,    25,    29,    52,    20,    76,    80,    71,    67,
      71,    29,    52
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    54,    54,    54,    54,    55,    54,    54,    56,    54,
      54,    58,    57,    57,    59,    59,    59,    59,    59,    60,
      60,    60,    60,    61,    61,    61,    61,    62,    63,    62,
      62,    64,    64,    65,    66,    67,    67,    67,    68,    68,
      69,    69,    70,    72,    71,    73,    71,    75,    76,    74,
      77,    78,    74,    79,    79,    79,    79,    79,    79,    80,
      81,    80,    80,    82,    82,    84,    83,    86,    85,    87,
      87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     2,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     2,     2,     2,     0,     3,     2,     0,     3,
       1,     0,     4,     3,     1,     1,     1,     3,     1,     1,
       2,     2,     1,     3,     4,     3,     4,     4,     0,     3,
       6,     1,     1,     3,     3,     1,     3,     0,     3,     3,
       1,     3,     5,     0,     4,     0,     3,     0,     0,     8,
       0,     0,     7,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     0,     5,     7,     0,     6,     0,    10,     3,
       2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: set  */
#line 77 "grammar.y"
               {printf("set -> program\n");}
#line 1376 "grammar.c"
    break;

  case 3: /* program: %empty  */
#line 78 "grammar.y"
        {printf("EMPTY -> program\n");}
#line 1382 "grammar.c"
    break;

  case 6: /* stmt: expr SEMICOLON  */
#line 85 "grammar.y"
                        {printf("expr ; -> stmt\n");}
#line 1388 "grammar.c"
    break;

  case 7: /* stmt: ifstmt  */
#line 86 "grammar.y"
            {printf("ifstmt -> stmt\n");}
#line 1394 "grammar.c"
    break;

  case 8: /* stmt: whilestmt  */
#line 87 "grammar.y"
                {printf("whilestmt -> stmt\n");}
#line 1400 "grammar.c"
    break;

  case 9: /* stmt: forstmt  */
#line 88 "grammar.y"
                {printf("forstmt -> stmt\n");}
#line 1406 "grammar.c"
    break;

  case 10: /* stmt: returnstmt  */
#line 89 "grammar.y"
                   {
                printf("returnstmt -> stmt\n");
                if(funcFlag == 0) {
                    yyerror("\033[31mERROR: return statement outside function\033[0m\t");
                }
            }
#line 1417 "grammar.c"
    break;

  case 11: /* stmt: BREAK SEMICOLON  */
#line 95 "grammar.y"
                        {
            printf("break; -> stmt\n");
            if(loopFlag == 0) {
                yyerror("\033[31mERROR: break statement outside loop\033[0m\t");
            }
        }
#line 1428 "grammar.c"
    break;

  case 12: /* stmt: CONTINUE SEMICOLON  */
#line 101 "grammar.y"
                        {
            printf("continue; -> stmt\n");
            if(loopFlag == 0) {
                yyerror("\033[31mERROR: continue statement outside loop\033[0m\t");
            }
        }
#line 1439 "grammar.c"
    break;

  case 13: /* stmt: block  */
#line 107 "grammar.y"
            {printf("block -> stmt\n");}
#line 1445 "grammar.c"
    break;

  case 14: /* stmt: funcdef  */
#line 108 "grammar.y"
                {printf("funcdef -> stmt\n");}
#line 1451 "grammar.c"
    break;

  case 15: /* stmt: SEMICOLON  */
#line 109 "grammar.y"
                {printf("; -> stmt\n");}
#line 1457 "grammar.c"
    break;

  case 16: /* expr: assignexpr  */
#line 112 "grammar.y"
                    {printf("assignexpr -> expr\n");}
#line 1463 "grammar.c"
    break;

  case 17: /* expr: expr OPERATOR_PLUS expr  */
#line 113 "grammar.y"
                                {printf("expr + expr -> expr\n");}
#line 1469 "grammar.c"
    break;

  case 18: /* expr: expr OPERATOR_MINUS expr  */
#line 114 "grammar.y"
                                {printf("expr - expr -> expr\n");}
#line 1475 "grammar.c"
    break;

  case 19: /* expr: expr OPERATOR_MOD expr  */
#line 115 "grammar.y"
                                {printf("expr % expr -> expr\n");}
#line 1481 "grammar.c"
    break;

  case 20: /* expr: expr OPERATOR_DIV expr  */
#line 116 "grammar.y"
                                {printf("expr / expr -> expr\n");}
#line 1487 "grammar.c"
    break;

  case 21: /* expr: expr OPERATOR_MUL expr  */
#line 117 "grammar.y"
                                {printf("expr * expr -> expr\n");}
#line 1493 "grammar.c"
    break;

  case 22: /* expr: expr OPERATOR_GRT expr  */
#line 118 "grammar.y"
                                {printf("expr > expr -> expr\n");}
#line 1499 "grammar.c"
    break;

  case 23: /* expr: expr OPERATOR_GRE expr  */
#line 119 "grammar.y"
                                {printf("expr >= expr -> expr\n");}
#line 1505 "grammar.c"
    break;

  case 24: /* expr: expr OPERATOR_LES expr  */
#line 120 "grammar.y"
                                {printf("expr < expr -> expr\n");}
#line 1511 "grammar.c"
    break;

  case 25: /* expr: expr OPERATOR_LEE expr  */
#line 121 "grammar.y"
                                {printf("expr <= expr -> expr\n");}
#line 1517 "grammar.c"
    break;

  case 26: /* expr: expr OPERATOR_EQ expr  */
#line 122 "grammar.y"
                                {printf("expr == expr -> expr\n");}
#line 1523 "grammar.c"
    break;

  case 27: /* expr: expr OPERATOR_NEQ expr  */
#line 123 "grammar.y"
                                {printf("expr != expr -> expr\n");}
#line 1529 "grammar.c"
    break;

  case 28: /* expr: expr AND expr  */
#line 124 "grammar.y"
                                {printf("expr and expr -> expr\n");}
#line 1535 "grammar.c"
    break;

  case 29: /* expr: expr OR expr  */
#line 125 "grammar.y"
                                {printf("expr or expr -> expr\n");}
#line 1541 "grammar.c"
    break;

  case 30: /* expr: term  */
#line 126 "grammar.y"
            {printf("term -> expr\n");}
#line 1547 "grammar.c"
    break;

  case 31: /* term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS  */
#line 129 "grammar.y"
                                                {printf("(expr) -> term");}
#line 1553 "grammar.c"
    break;

  case 32: /* term: OPERATOR_MINUS expr  */
#line 130 "grammar.y"
                            {printf("- expr -> term\n");}
#line 1559 "grammar.c"
    break;

  case 33: /* term: NOT expr  */
#line 131 "grammar.y"
                        {printf("not expr -> term\n");}
#line 1565 "grammar.c"
    break;

  case 34: /* term: OPERATOR_PP lvalue  */
#line 132 "grammar.y"
                        {
            printf("++lvalue -> term\n");
            SymbolTableEntry* temp;
            temp = lookupScope(yylval.strVal, scope);
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                }
            }
        }
#line 1580 "grammar.c"
    break;

  case 35: /* $@1: %empty  */
#line 142 "grammar.y"
            {
                SymbolTableEntry* temp;
                temp = lookupScope(yylval.strVal, scope);
                if(temp != NULL) {
                    if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                        yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                    }
                }
    }
#line 1594 "grammar.c"
    break;

  case 36: /* term: lvalue $@1 OPERATOR_PP  */
#line 150 "grammar.y"
                    {printf("lvalue++ -> term\n");}
#line 1600 "grammar.c"
    break;

  case 37: /* term: OPERATOR_MM lvalue  */
#line 151 "grammar.y"
                        {
            printf("--lvalue -> term\n");
            SymbolTableEntry* temp;
            temp = lookupScope(yylval.strVal, scope);
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                }
            }
        }
#line 1615 "grammar.c"
    break;

  case 38: /* $@2: %empty  */
#line 161 "grammar.y"
            {
            printf("++lvalue -> term\n");
            SymbolTableEntry* temp;
            temp = lookupScope(yylval.strVal, scope);
            if(temp != NULL) {
                if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                    yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                }
            }
        }
#line 1630 "grammar.c"
    break;

  case 39: /* term: lvalue $@2 OPERATOR_MM  */
#line 170 "grammar.y"
                      {
            printf("lvalue-- -> term\n");

        }
#line 1639 "grammar.c"
    break;

  case 40: /* term: primary  */
#line 174 "grammar.y"
                {printf("primary -> term\n");}
#line 1645 "grammar.c"
    break;

  case 41: /* $@3: %empty  */
#line 177 "grammar.y"
                   {
                if(memberflag == 0){
                    SymbolTableEntry* temp;
                    temp = lookupScope(yylval.strVal, scope);
                    if(temp != NULL) {
                        if(temp -> type == USERFUNC || temp -> type == LIBFUNC) {
                            yyerror("\033[31mERROR: Function cannot be used as lvalue\033[0m\t");
                        }
                    }
                }else{
                    memberflag = 0;
                }
            }
#line 1663 "grammar.c"
    break;

  case 42: /* assignexpr: lvalue $@3 OPERATOR_ASSIGN expr  */
#line 190 "grammar.y"
                                 {
            printf("lvalue = expr -> assignexpr\n");
            
        }
#line 1672 "grammar.c"
    break;

  case 43: /* assignexpr: call OPERATOR_ASSIGN expr  */
#line 194 "grammar.y"
                                {
        yyerror("\033[31mERROR: function call cannot be an lvalue\033[0m\t");
    }
#line 1680 "grammar.c"
    break;

  case 44: /* primary: lvalue  */
#line 199 "grammar.y"
                {printf("lvalue -> primary\n");}
#line 1686 "grammar.c"
    break;

  case 45: /* primary: call  */
#line 200 "grammar.y"
            {printf("call -> primary\n");}
#line 1692 "grammar.c"
    break;

  case 46: /* primary: objectdef  */
#line 201 "grammar.y"
                {printf("objectdef -> primary\n");}
#line 1698 "grammar.c"
    break;

  case 47: /* primary: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS  */
#line 202 "grammar.y"
                                                {printf("(funcdef) -> primary\n");}
#line 1704 "grammar.c"
    break;

  case 48: /* primary: const  */
#line 203 "grammar.y"
            {printf("const -> primary\n");}
#line 1710 "grammar.c"
    break;

  case 49: /* lvalue: ID  */
#line 206 "grammar.y"
            {
    int searchScope;
    SymbolTableEntry *temp, *f;
    printf("ID -> lvalue\n");
    if(funcFlag != 0) {
        f = lookupEverything(currFunc, scope);
        assert(f != NULL);
        searchScope = getEntryScope(f);
        temp = lookupEverything(yylval.strVal, searchScope + 1);
        if(temp != NULL) {
            printf("EDW %d\n", searchScope);
            if(getEntryScope(temp) != 0 && getEntryScope(temp) <= (searchScope) && temp -> type  != USERFUNC) {
                    printf("\033[31mERROR: Cannot access %s inside function\033[0m", getEntryName(temp));
                    yyerror("\t");
            }
        }
    }
    insertID(yylval.strVal, scope, yylineno);

    }
#line 1735 "grammar.c"
    break;

  case 50: /* lvalue: LOCAL_KEYWORD ID  */
#line 226 "grammar.y"
                        {
        printf("local ID -> lvalue\n");
        insertLocalID(yylval.strVal, scope, yylineno);
    }
#line 1744 "grammar.c"
    break;

  case 51: /* lvalue: DOUBLE_COLON ID  */
#line 230 "grammar.y"
                        {  
        printf("::ID -> lvalue\n");
        if(lookupScope(yylval.strVal, 0) == NULL) {
            yyerror("\033[31mERROR: Global variable cannot be found!\033[0m\t");
        }   
    }
#line 1755 "grammar.c"
    break;

  case 52: /* lvalue: member  */
#line 236 "grammar.y"
            {
        printf("member -> lvalue\n");
        memberflag = 1;
    }
#line 1764 "grammar.c"
    break;

  case 53: /* member: lvalue DOT ID  */
#line 242 "grammar.y"
                        {printf("lvalue.ID -> mebmer\n");}
#line 1770 "grammar.c"
    break;

  case 54: /* member: lvalue LEFT_BRACE expr RIGHT_BRACE  */
#line 243 "grammar.y"
                                        {printf("lvalue[expr] -> member\n");}
#line 1776 "grammar.c"
    break;

  case 55: /* member: call DOT ID  */
#line 244 "grammar.y"
                    {printf("call.id -> member\n");}
#line 1782 "grammar.c"
    break;

  case 56: /* member: call LEFT_BRACE expr RIGHT_BRACE  */
#line 245 "grammar.y"
                                        {printf("call[expr] -> member\n");}
#line 1788 "grammar.c"
    break;

  case 57: /* call: call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 248 "grammar.y"
                                                    {printf("call(elist) -> call\n");}
#line 1794 "grammar.c"
    break;

  case 58: /* $@4: %empty  */
#line 249 "grammar.y"
           {
            SymbolTableEntry* temp;
            temp = lookupforCalls(yylval.strVal, scope);
            if(temp != NULL){
                if(getEntryScope(temp) != 0 && getEntryScope(temp) != scope && temp-> type  != USERFUNC) {
                    printf("\033[31mERROR: Cannot access %s inside function\033[0m", getEntryName(temp));
                    yyerror("\t");
                }
            }
            
    }
#line 1810 "grammar.c"
    break;

  case 59: /* call: lvalue $@4 callsuffix  */
#line 259 "grammar.y"
                  {
            printf("lvalue() -> call\n");
        }
#line 1818 "grammar.c"
    break;

  case 60: /* call: LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS  */
#line 263 "grammar.y"
        {printf("(funcdef)(elist) -> call\n");}
#line 1824 "grammar.c"
    break;

  case 61: /* callsuffix: methodcall  */
#line 266 "grammar.y"
                       {
        printf("methodcall -> callsuffix\n");
    }
#line 1832 "grammar.c"
    break;

  case 62: /* callsuffix: normcall  */
#line 269 "grammar.y"
                {printf("normcall -> callsuffix\n");}
#line 1838 "grammar.c"
    break;

  case 64: /* methodcall: DOUBLE_DOT ID normcall  */
#line 275 "grammar.y"
                                    {printf("..id(elist) -> methodcall\n");}
#line 1844 "grammar.c"
    break;

  case 68: /* objectdef: LEFT_BRACE elist RIGHT_BRACE  */
#line 283 "grammar.y"
                                        {printf("[elist] -> objectdef\n");}
#line 1850 "grammar.c"
    break;

  case 69: /* objectdef: LEFT_BRACE indexed RIGHT_BRACE  */
#line 284 "grammar.y"
                                    {printf("[indexed] -> objectdef\n]");}
#line 1856 "grammar.c"
    break;

  case 72: /* indexedelem: LEFT_BRACKET expr COLON expr RIGHT_BRACKET  */
#line 291 "grammar.y"
                                                        {printf("{expr : expr} -> indexed elem\n");}
#line 1862 "grammar.c"
    break;

  case 73: /* $@5: %empty  */
#line 294 "grammar.y"
                    {scope++;}
#line 1868 "grammar.c"
    break;

  case 74: /* block: LEFT_BRACKET $@5 set RIGHT_BRACKET  */
#line 294 "grammar.y"
                                                 {
        printf("block with stmts -> block\n");
        hideEntries(scope);
        scope--;
    }
#line 1878 "grammar.c"
    break;

  case 75: /* $@6: %empty  */
#line 299 "grammar.y"
                  {scope++;}
#line 1884 "grammar.c"
    break;

  case 76: /* block: LEFT_BRACKET $@6 RIGHT_BRACKET  */
#line 299 "grammar.y"
                                             {
            printf("empty block -> block\n");
            hideEntries(scope);
            scope--;
        }
#line 1894 "grammar.c"
    break;

  case 77: /* $@7: %empty  */
#line 306 "grammar.y"
                     {
        funcFlag++;
        currFunc = strdup(yylval.strVal);
        temp_func -> name = yylval.strVal;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
    }
#line 1906 "grammar.c"
    break;

  case 78: /* $@8: %empty  */
#line 313 "grammar.y"
                                               {
        SymbolTableEntry *temp;
        temp = lookupScope(temp_func -> name, temp_func -> scope);

        if(temp != NULL){
            if(temp -> type == LIBFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s \" is a library function\033[0m", temp_func -> name);
                yyerror("\t");
            }

            if(temp -> type == USERFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s \" is already in use\033[0m", temp_func -> name);
                yyerror("\t");
            }
            if(temp -> type == GLOBAL || temp -> type == LOCAL || temp -> type == FORMAL){
                printf("\033[31mERROR: Variable %s already defined\033[0m", temp_func -> name);
                yyerror("\t");
            }
        }
        else{
            SymbolTableEntry *new_entry;
            Function *new_func;
            int i;
            
            new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
            new_func =  (Function *)malloc(sizeof(Function));
            new_func -> arguments = (char**)malloc(10*sizeof(char *));

            new_func -> name = strdup(temp_func -> name);
            new_func -> scope = temp_func -> scope;
            new_func -> line = temp_func -> line;
            for(i = 0;i < arg_index;i++){
                new_func -> arguments[i] = strdup(temp_func -> arguments[i]);
                insertFormal(new_func -> arguments[i], scope + 1, yylineno);
            }
            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
        }

    }
#line 1955 "grammar.c"
    break;

  case 79: /* funcdef: FUNCTION ID $@7 LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS $@8 block  */
#line 357 "grammar.y"
             {
        printf("function id(idlist)block -> funcdef\n", yytext);
        
        int i = 0;
        temp_func -> name = "";
        temp_func -> scope = 0;
        temp_func -> line = 0;
        for(i = 0;i < arg_index;i++)
            temp_func -> arguments[i] = ""; 
        arg_index = 0;
        funcFlag--;
    }
#line 1972 "grammar.c"
    break;

  case 80: /* $@9: %empty  */
#line 369 "grammar.y"
              {
        funcFlag++;
        char* fname = (char*) malloc(sizeof(char)*50);
        sprintf( fname, "_anonfunc%d", anonFuncCounter);
        currFunc = strdup(fname);
        anonFuncCounter++;
        temp_func -> scope = scope;
        temp_func -> line = yylineno;
        temp_func -> name = fname;
        
    }
#line 1988 "grammar.c"
    break;

  case 81: /* $@10: %empty  */
#line 379 "grammar.y"
                                               {
        SymbolTableEntry *temp;
        temp = lookupScope(temp_func -> name, temp_func -> scope);

        if(temp != NULL){
            if(temp -> type == LIBFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s \" is a library function\033[0m", temp_func -> name);
                yyerror("\t");
            }

            if(temp -> type == USERFUNC){
                printf("\033[31mERROR: Function name redefinition \"%s \" is already in use\033[0m", temp_func -> name);
                yyerror("\t");
            }
            if(temp -> type == GLOBAL || temp -> type == LOCAL || temp -> type == FORMAL){
                printf("\033[31mERROR: Variable %s already defined\033[0m", temp_func -> name);
                yyerror("\t");
            }
            
        }
        else{
            SymbolTableEntry *new_entry;
            Function *new_func;
            int i;
            
            new_entry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
            new_func =  (Function *)malloc(sizeof(Function));

            new_func -> arguments = (char**)malloc(10*sizeof(char *));
            new_func -> name = strdup(temp_func -> name);
            new_func -> scope = temp_func -> scope;
            new_func -> line = temp_func -> line;

            for(i = 0;i < arg_index;i++){
                new_func -> arguments[i] = (char*) malloc(sizeof(char)*100);
                new_func -> arguments[i] = strdup(temp_func -> arguments[i]);
                insertFormal(new_func -> arguments[i], scope + 1, yylineno);
            }

            new_entry -> isActive = 1;
            new_entry -> varVal = NULL;
            new_entry -> funcVal = new_func;
            new_entry -> type = USERFUNC;

            insertEntry(new_entry);
        }

    }
#line 2041 "grammar.c"
    break;

  case 82: /* funcdef: FUNCTION $@9 LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS $@10 block  */
#line 426 "grammar.y"
               {
        printf("function (idlist)block -> funcdef\n");

        int i = 0;
        temp_func -> name = "";
        temp_func -> scope = 0;
        temp_func -> line = 0;
        for(i = 0;i < arg_index;i++)
            temp_func -> arguments[i] = "";
        arg_index = 0;
        funcFlag--; 
    }
#line 2058 "grammar.c"
    break;

  case 89: /* idlist: ID  */
#line 448 "grammar.y"
           {
        int j;
        int flag = 1;
        
        for(j = 0; j < arg_index; j++){
            flag = strcmp(yylval.strVal, temp_func -> arguments[j]);
            if(flag == 0) 
                break;
        }

        if(flag == 0) {
            printf("\033[31mERROR: Symbol with name \"%s\" already exists!\033[0m\t", yylval.strVal);
            yyerror("\t");
        }else {
            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;
        }
        
        
    }
#line 2083 "grammar.c"
    break;

  case 90: /* $@11: %empty  */
#line 468 "grammar.y"
        {
        int j;
        int flag = 1;
        
        for(j = 0; j < arg_index; j++){
            flag = strcmp(yylval.strVal, temp_func -> arguments[j]);
            if(flag == 0) 
                break;
        }

        if(flag == 0) {
            printf("\033[31mERROR: Symbol with name \"%s\" already exists!\033[0m\t", yylval.strVal);
            yyerror("\t");
        }else {
            temp_func -> arguments[arg_index] = yylval.strVal;
            arg_index++;
        }
    }
#line 2106 "grammar.c"
    break;

  case 92: /* idlist: %empty  */
#line 487 "grammar.y"
     {printf("empty -> idlist\n");}
#line 2112 "grammar.c"
    break;

  case 93: /* ifstmt: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt  */
#line 490 "grammar.y"
                                                          {printf("if(expr) -> ifstmt\n");}
#line 2118 "grammar.c"
    break;

  case 94: /* ifstmt: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt  */
#line 491 "grammar.y"
                                                                  {printf("if(expr) else -> ifstmt\n");}
#line 2124 "grammar.c"
    break;

  case 95: /* $@12: %empty  */
#line 494 "grammar.y"
                 {loopFlag++;}
#line 2130 "grammar.c"
    break;

  case 96: /* whilestmt: WHILE $@12 LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt  */
#line 494 "grammar.y"
                                                                              {printf("while(expr) -> whilestmt\n"); loopFlag--;}
#line 2136 "grammar.c"
    break;

  case 97: /* $@13: %empty  */
#line 497 "grammar.y"
             {loopFlag++;}
#line 2142 "grammar.c"
    break;

  case 98: /* forstmt: FOR $@13 LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt  */
#line 498 "grammar.y"
        {printf("for(elist;expr;elist)stmt -> forstmt\n"); loopFlag--;}
#line 2148 "grammar.c"
    break;

  case 99: /* returnstmt: RETURN expr SEMICOLON  */
#line 501 "grammar.y"
                                    {printf("return expr ; -> returnstmt\n");}
#line 2154 "grammar.c"
    break;

  case 100: /* returnstmt: RETURN SEMICOLON  */
#line 502 "grammar.y"
                         {printf("return ; -> returnstmt\n");}
#line 2160 "grammar.c"
    break;


#line 2164 "grammar.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 504 "grammar.y"



int yyerror(char *message){
    printf("%s: in line %d\n", message, yylineno); 
}

int main(int argc, char* argv[]){

    temp_func = (Function *)malloc(sizeof(Function));
    temp_func -> arguments =(char**)malloc(10*sizeof(char*));


    initTable();

    yyparse();

    printEntries();

    return 0;
}
