/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/main.y"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// define type table
#define False 0
#define True 1
#define DEBUG_MODE 1

#define MAX_SIZE 9999

#define BOOL_TYPE 0
#define INT_TYPE 1
#define STR_TYPE 2
#define PRINT_NUM_TYPE 3
#define PRINT_BOOL_TYPE 4
#define NOT_TYPE 5
#define PLUS_TYPE 6
#define MINUS_TYPE 7
#define MULTIPLY_TYPE 8
#define DIVIDE_TYPE 9
#define MOD_TYPE 10
#define EMPTY_TYPE 11
#define GREATER_TYPE 12
#define SMALLER_TYPE 13
#define EQUAL_TYPE 14
#define EXPRS_TYPE 15
#define AND_TYPE 16
#define OR_TYPE 17
#define DEFINE_TYPE 18
#define IF_TYPE 19
#define THEN_ELSE_TYPE 20
#define VARIABLE_TYPE 21
#define DEFINE_VARIABLE_TYPE 22
int yylex();
void yyerror(const char *s);

typedef struct Element {
    int type;
    char* cval;
    int ival;
} Element;

Element* newElement(int type, char* cval, int ival){
    Element* temp = (Element *)malloc(sizeof(Element));
    temp->type = type;
    if(type == BOOL_TYPE || type == INT_TYPE){
        temp->ival = ival;
    } else {
        temp->cval = cval;
    }
    return temp;
}
typedef struct Node {
    Element* val; 
    struct Node* left;
    struct Node* right;
} Node;

Node* root;

Node* variables[MAX_SIZE];
int top_variables = -1;


Node* newNode(Element* val, Node* left, Node* right) {
    Node* temp = (Node *)malloc(sizeof(Node));
    temp->val = val;
    temp->left = left;
    temp->right = right;
    return temp;
}
Node* copyNode(Node* node){
    if(node == NULL) return NULL;
   
    Node* temp = newNode(newElement(EMPTY_TYPE, NULL, 0), NULL, NULL);
    
    temp->val->type = node->val->type;
    temp->val->ival = node->val->ival;
    temp->val->cval = node->val->cval;

    temp->left = copyNode(node->left);
    temp->right = copyNode(node->right);

    return temp;
}
void freeNode(Node** node) {
    if(node == NULL) return;
    freeNode(&((*node)->left));
    freeNode(&((*node)->right));
    free(node);
}

void eval(Node* node, int type) {
    if(DEBUG_MODE) {
        printf("<--EVAL-->\n");
    }
    if(node == NULL)  {
        return;
    }
    if(node->val->type == EXPRS_TYPE){
        if(DEBUG_MODE){
            printf("EXPRS, TYPE:%d\n", type);
        }
        node->val->type = type;
    }
    switch(node->val->type){
        case EMPTY_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            break;
        case PRINT_NUM_TYPE:
            if(DEBUG_MODE){
                printf("PRINT_NUM\n");
            }
            eval(node->left, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            printf("%d\n", node->left->val->ival);
            break;
        case PRINT_BOOL_TYPE:
            eval(node->left, node->val->type);
            if(node->left->val->type != BOOL_TYPE) yyerror("Type error");
            if(node->left->val->ival == 0) printf("#f\n");
            else if(node->left->val->ival == 1) printf("#t\n");
            else {
               yyerror("BOOL Error");
            }
            break;
        case PLUS_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            if(node->right->val->type != INT_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival + node->right->val->ival;
            node->val->type = INT_TYPE;
            if(DEBUG_MODE) {
                printf("PLUS_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case MINUS_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            if(node->right->val->type != INT_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival - node->right->val->ival;
            node->val->type = INT_TYPE;
            if(DEBUG_MODE) {
                printf("MINUS_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case MULTIPLY_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            if(node->right->val->type != INT_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival * node->right->val->ival;
            node->val->type = INT_TYPE;
            if(DEBUG_MODE){
                printf("MULTIPLY_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case DIVIDE_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            if(node->right->val->type != INT_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival / node->right->val->ival;
            node->val->type = INT_TYPE;
            if(DEBUG_MODE){
                printf("DIVIDE_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case GREATER_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            if(node->right->val->type != INT_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival > node->right->val->ival;
            node->val->type = BOOL_TYPE;
            if(DEBUG_MODE){
                printf("GREATER_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case SMALLER_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            if(node->right->val->type != INT_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival < node->right->val->ival;
            node->val->type = BOOL_TYPE;
            if(DEBUG_MODE){
                printf("SMALLER_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case EQUAL_TYPE:
            break;
        case AND_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != BOOL_TYPE) yyerror("Type error");
            if(node->right->val->type != BOOL_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival && node->right->val->ival;
            node->val->type = BOOL_TYPE;
            if(DEBUG_MODE){
                printf("AND_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case OR_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != BOOL_TYPE) yyerror("Type error");
            if(node->right->val->type != BOOL_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival || node->right->val->ival;
            node->val->type = BOOL_TYPE;
            if(DEBUG_MODE){
                printf("OR_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case NOT_TYPE:
            eval(node->left, node->val->type);
            if(node->left->val->type != BOOL_TYPE) yyerror("Type error");
            node->val->ival = !node->left->val->ival;
            node->val->type = BOOL_TYPE;
            if(DEBUG_MODE){
                printf("NOT_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case IF_TYPE:
            eval(node->left, node->val->type);
            if(node->left->val->type != BOOL_TYPE) yyerror("Type error");
            if(node->left->val->ival == 1){
                eval(node->right->left, node->right->val->type);
                node->val->type = node->right->left->val->type;
                node->val->ival = node->right->left->val->ival;
            } else {
                eval(node->right->right, node->right->val->type);
                node->val->type = node->right->right->val->type;
                node->val->ival = node->right->right->val->ival;
            }
            if(DEBUG_MODE){
                printf("IF_TYPE, VAL=%d\n", node->val->ival);
            }
            break;
        case DEFINE_VARIABLE_TYPE:
            for(int i=0;i<=top_variables;i++){
                if(strcmp(variables[i]->val->cval, node->left->val->cval) == 0){
                    yyerror("Variable redefinition");
                }
            }
            node->right->val->cval = node->left->val->cval;
            variables[++top_variables] = copyNode(node->right);
            
            if(DEBUG_MODE){
                printf("DEFINE VARIABLE %s\n", node->left->val->cval);
            }
            break;
        case VARIABLE_TYPE:
            if(DEBUG_MODE){
                printf("EVAL VARIABLE %s\n", node->val->cval);
            }
            for(int i=0;i<=top_variables;i++){
                if(strcmp(variables[i]->val->cval,node->val->cval) == 0) {
                    Node* temp = copyNode(variables[i]);
                    eval(temp, node->val->type);
                    node->val->type = temp->val->type;
                    node->val->ival = temp->val->ival;
                    node->val->cval = temp->val->cval;
                    break;
                }
            }
            break;

    }
}

#line 347 "y.tab.c"

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

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    number = 258,                  /* number  */
    bool_val = 259,                /* bool_val  */
    ID = 260,                      /* ID  */
    and = 261,                     /* and  */
    or = 262,                      /* or  */
    not = 263,                     /* not  */
    mod = 264,                     /* mod  */
    define = 265,                  /* define  */
    print_num = 266,               /* print_num  */
    print_bool = 267,              /* print_bool  */
    IF = 268                       /* IF  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define number 258
#define bool_val 259
#define ID 260
#define and 261
#define or 262
#define not 263
#define mod 264
#define define 265
#define print_num 266
#define print_bool 267
#define IF 268

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 276 "src/main.y"

    char* str;
    int num;
    struct Node* node;

#line 432 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_number = 3,                     /* number  */
  YYSYMBOL_bool_val = 4,                   /* bool_val  */
  YYSYMBOL_ID = 5,                         /* ID  */
  YYSYMBOL_and = 6,                        /* and  */
  YYSYMBOL_or = 7,                         /* or  */
  YYSYMBOL_not = 8,                        /* not  */
  YYSYMBOL_mod = 9,                        /* mod  */
  YYSYMBOL_define = 10,                    /* define  */
  YYSYMBOL_print_num = 11,                 /* print_num  */
  YYSYMBOL_print_bool = 12,                /* print_bool  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_14_ = 14,                       /* '('  */
  YYSYMBOL_15_ = 15,                       /* ')'  */
  YYSYMBOL_16_ = 16,                       /* '+'  */
  YYSYMBOL_17_ = 17,                       /* '-'  */
  YYSYMBOL_18_ = 18,                       /* '*'  */
  YYSYMBOL_19_ = 19,                       /* '/'  */
  YYSYMBOL_20_ = 20,                       /* '>'  */
  YYSYMBOL_21_ = 21,                       /* '<'  */
  YYSYMBOL_22_ = 22,                       /* '='  */
  YYSYMBOL_YYACCEPT = 23,                  /* $accept  */
  YYSYMBOL_program = 24,                   /* program  */
  YYSYMBOL_stmts = 25,                     /* stmts  */
  YYSYMBOL_stmt = 26,                      /* stmt  */
  YYSYMBOL_expr = 27,                      /* expr  */
  YYSYMBOL_print_stmt = 28,                /* print_stmt  */
  YYSYMBOL_NUM_OP = 29,                    /* NUM_OP  */
  YYSYMBOL_PLUS = 30,                      /* PLUS  */
  YYSYMBOL_MINUS = 31,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 32,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 33,                    /* DIVIDE  */
  YYSYMBOL_MODULUS = 34,                   /* MODULUS  */
  YYSYMBOL_GREATER = 35,                   /* GREATER  */
  YYSYMBOL_SMALLER = 36,                   /* SMALLER  */
  YYSYMBOL_EQUAL = 37,                     /* EQUAL  */
  YYSYMBOL_LOGICAL_OP = 38,                /* LOGICAL_OP  */
  YYSYMBOL_AND_OP = 39,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 40,                     /* OR_OP  */
  YYSYMBOL_NOT_OP = 41,                    /* NOT_OP  */
  YYSYMBOL_def_stmt = 42,                  /* def_stmt  */
  YYSYMBOL_VARIABLE = 43,                  /* VARIABLE  */
  YYSYMBOL_FUN_expr = 44,                  /* FUN_expr  */
  YYSYMBOL_FUN_IDs = 45,                   /* FUN_IDs  */
  YYSYMBOL_FUN_Body = 46,                  /* FUN_Body  */
  YYSYMBOL_FUN_Call = 47,                  /* FUN_Call  */
  YYSYMBOL_PARAMs = 48,                    /* PARAMs  */
  YYSYMBOL_PARAM = 49,                     /* PARAM  */
  YYSYMBOL_FUN_Name = 50,                  /* FUN_Name  */
  YYSYMBOL_IF_expr = 51,                   /* IF_expr  */
  YYSYMBOL_TEST_expr = 52,                 /* TEST_expr  */
  YYSYMBOL_THEN_expr = 53,                 /* THEN_expr  */
  YYSYMBOL_ELSE_expr = 54,                 /* ELSE_expr  */
  YYSYMBOL_IDs = 55,                       /* IDs  */
  YYSYMBOL_exprs = 56                      /* exprs  */
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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   146

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  23
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  58
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  112

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   268


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
      14,    15,    18,    16,     2,    17,     2,    19,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      21,    22,    20,     2,     2,     2,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   322,   322,   325,   326,   329,   330,   331,   334,   335,
     336,   337,   338,   339,   340,   341,   345,   346,   349,   350,
     351,   352,   353,   354,   355,   356,   359,   361,   363,   365,
     367,   369,   371,   373,   376,   377,   378,   380,   382,   384,
     387,   391,   394,   397,   400,   402,   403,   405,   406,   408,
     410,   412,   414,   416,   418,   420,   421,   424,   425
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
  "\"end of file\"", "error", "\"invalid token\"", "number", "bool_val",
  "ID", "and", "or", "not", "mod", "define", "print_num", "print_bool",
  "IF", "'('", "')'", "'+'", "'-'", "'*'", "'/'", "'>'", "'<'", "'='",
  "$accept", "program", "stmts", "stmt", "expr", "print_stmt", "NUM_OP",
  "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULUS", "GREATER", "SMALLER",
  "EQUAL", "LOGICAL_OP", "AND_OP", "OR_OP", "NOT_OP", "def_stmt",
  "VARIABLE", "FUN_expr", "FUN_IDs", "FUN_Body", "FUN_Call", "PARAMs",
  "PARAM", "FUN_Name", "IF_expr", "TEST_expr", "THEN_expr", "ELSE_expr",
  "IDs", "exprs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-13)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      23,   -13,   -13,   -13,     2,     3,    23,   -13,   -13,   -13,
     -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   132,
     132,   132,   132,    -1,   132,   132,   132,   -12,   132,   132,
     132,   132,   132,   132,   132,   -13,   132,   -13,   -13,   -13,
      87,   132,   132,   -10,   132,   132,    10,    18,   -13,   132,
     -13,    65,   132,   132,   132,   132,   132,   132,   132,    47,
     -13,    30,    74,   -13,   107,   110,   -13,    34,    38,   -13,
     -13,   -13,   132,   -13,   -13,   113,    41,   126,    42,    53,
      56,   129,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,    58,   -13,   -13,   -13,   -13,   -13,   -13,
     -13,   -13
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    14,    15,    41,     0,     0,     2,     4,     5,     7,
       8,    18,    19,    20,    21,    22,    23,    24,    25,     9,
      34,    35,    36,     6,    13,    10,    11,    12,    50,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,    47,     0,    47,     1,     3,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,    58,     0,     0,    39,     0,     0,    17,
      16,    53,     0,    56,    43,     0,     0,     0,     0,     0,
       0,     0,    45,    49,    48,    42,    46,    37,    57,    38,
      30,    40,    54,     0,    26,    27,    28,    29,    31,    32,
      33,    51
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -13,   -13,   -13,    11,     0,   -13,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,
      43,    -3,   -13,   -13,   -13,    36,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,    -4
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     5,     6,     7,    73,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    46,    71,    26,    69,    94,    47,    27,    59,
      82,   103,    61,    74
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       8,    45,    60,    48,     3,    76,     8,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    49,    38,    39,
      40,    41,    42,    43,    44,    79,     1,     2,     3,    51,
      52,    53,    54,    80,    56,    57,    58,     4,    62,    63,
      64,    65,    66,    67,    68,    95,    70,    45,    75,   100,
       1,     2,     3,   101,    77,    78,   105,   107,    85,    81,
      87,    50,    92,    86,    91,    88,    89,    90,   108,    93,
      83,   109,    93,   111,    98,    98,    55,     1,     2,     3,
      84,     0,   102,    72,     0,    98,     0,    98,    50,    96,
       0,    98,    28,    29,    30,    31,    32,     0,     0,     0,
      36,    37,     0,    38,    39,    40,    41,    42,    43,    44,
       1,     2,     3,     1,     2,     3,     1,     2,     3,     0,
       0,    50,    97,     0,    50,    99,     0,    50,   104,     1,
       2,     3,     1,     2,     3,     1,     2,     3,     0,     0,
      50,   106,     0,    50,   110,     0,    50
};

static const yytype_int8 yycheck[] =
{
       0,     4,    14,     0,     5,    15,     6,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     6,    16,    17,
      18,    19,    20,    21,    22,    15,     3,     4,     5,    29,
      30,    31,    32,    15,    34,    35,    36,    14,    38,    39,
      40,    41,    42,    43,    44,    15,    46,    50,    52,    15,
       3,     4,     5,    15,    54,    55,    15,    15,    62,    59,
      64,    14,    15,    63,    68,    65,    66,    67,    15,    69,
       5,    15,    72,    15,    74,    75,    33,     3,     4,     5,
      15,    -1,    82,    47,    -1,    85,    -1,    87,    14,    15,
      -1,    91,     5,     6,     7,     8,     9,    -1,    -1,    -1,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
       3,     4,     5,     3,     4,     5,     3,     4,     5,    -1,
      -1,    14,    15,    -1,    14,    15,    -1,    14,    15,     3,
       4,     5,     3,     4,     5,     3,     4,     5,    -1,    -1,
      14,    15,    -1,    14,    15,    -1,    14
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,    14,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    47,    51,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    16,    17,
      18,    19,    20,    21,    22,    44,    45,    50,     0,    26,
      14,    27,    27,    27,    27,    43,    27,    27,    27,    52,
      14,    55,    27,    27,    27,    27,    27,    27,    27,    48,
      27,    46,    48,    27,    56,    56,    15,    27,    27,    15,
      15,    27,    53,     5,    15,    56,    27,    56,    27,    27,
      27,    56,    15,    27,    49,    15,    15,    15,    27,    15,
      15,    15,    27,    54,    15,    15,    15,    15,    15,    15,
      15,    15
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    23,    24,    25,    25,    26,    26,    26,    27,    27,
      27,    27,    27,    27,    27,    27,    28,    28,    29,    29,
      29,    29,    29,    29,    29,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    38,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    47,    48,    48,    49,
      50,    51,    52,    53,    54,    55,    55,    56,    56
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     5,     5,     5,
       5,     5,     5,     5,     1,     1,     1,     5,     5,     4,
       5,     1,     4,     3,     1,     4,     4,     0,     2,     1,
       1,     6,     1,     1,     1,     0,     2,     2,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
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
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
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
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
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
  case 2: /* program: stmts  */
#line 322 "src/main.y"
                    {root = (yyvsp[0].node);}
#line 1543 "y.tab.c"
    break;

  case 3: /* stmts: stmts stmt  */
#line 325 "src/main.y"
                        {(yyval.node) = newNode(newElement(EMPTY_TYPE, NULL,0), (yyvsp[-1].node), (yyvsp[0].node));}
#line 1549 "y.tab.c"
    break;

  case 4: /* stmts: stmt  */
#line 326 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1555 "y.tab.c"
    break;

  case 5: /* stmt: expr  */
#line 329 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1561 "y.tab.c"
    break;

  case 6: /* stmt: def_stmt  */
#line 330 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1567 "y.tab.c"
    break;

  case 7: /* stmt: print_stmt  */
#line 331 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1573 "y.tab.c"
    break;

  case 8: /* expr: NUM_OP  */
#line 334 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1579 "y.tab.c"
    break;

  case 9: /* expr: LOGICAL_OP  */
#line 335 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1585 "y.tab.c"
    break;

  case 10: /* expr: FUN_expr  */
#line 336 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1591 "y.tab.c"
    break;

  case 11: /* expr: FUN_Call  */
#line 337 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1597 "y.tab.c"
    break;

  case 12: /* expr: IF_expr  */
#line 338 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1603 "y.tab.c"
    break;

  case 13: /* expr: VARIABLE  */
#line 339 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1609 "y.tab.c"
    break;

  case 14: /* expr: number  */
#line 340 "src/main.y"
                        {(yyval.node) = newNode(newElement(INT_TYPE, NULL, (yyvsp[0].num)), NULL, NULL);}
#line 1615 "y.tab.c"
    break;

  case 15: /* expr: bool_val  */
#line 341 "src/main.y"
                        {(yyval.node) = newNode(newElement(BOOL_TYPE, NULL, (yyvsp[0].num)),NULL,NULL);}
#line 1621 "y.tab.c"
    break;

  case 16: /* print_stmt: '(' print_bool expr ')'  */
#line 345 "src/main.y"
                                         {(yyval.node) = newNode(newElement(PRINT_BOOL_TYPE, NULL,0), (yyvsp[-1].node), NULL);}
#line 1627 "y.tab.c"
    break;

  case 17: /* print_stmt: '(' print_num expr ')'  */
#line 346 "src/main.y"
                                       {(yyval.node) = newNode(newElement(PRINT_NUM_TYPE, NULL, 0), (yyvsp[-1].node), NULL);}
#line 1633 "y.tab.c"
    break;

  case 18: /* NUM_OP: PLUS  */
#line 349 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1639 "y.tab.c"
    break;

  case 19: /* NUM_OP: MINUS  */
#line 350 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1645 "y.tab.c"
    break;

  case 20: /* NUM_OP: MULTIPLY  */
#line 351 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1651 "y.tab.c"
    break;

  case 21: /* NUM_OP: DIVIDE  */
#line 352 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1657 "y.tab.c"
    break;

  case 22: /* NUM_OP: MODULUS  */
#line 353 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1663 "y.tab.c"
    break;

  case 23: /* NUM_OP: GREATER  */
#line 354 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1669 "y.tab.c"
    break;

  case 24: /* NUM_OP: SMALLER  */
#line 355 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1675 "y.tab.c"
    break;

  case 25: /* NUM_OP: EQUAL  */
#line 356 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1681 "y.tab.c"
    break;

  case 26: /* PLUS: '(' '+' expr exprs ')'  */
#line 359 "src/main.y"
                                    {(yyval.node) = newNode(newElement(PLUS_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1687 "y.tab.c"
    break;

  case 27: /* MINUS: '(' '-' expr expr ')'  */
#line 361 "src/main.y"
                                    {(yyval.node) = newNode(newElement(MINUS_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1693 "y.tab.c"
    break;

  case 28: /* MULTIPLY: '(' '*' expr exprs ')'  */
#line 363 "src/main.y"
                                    {(yyval.node) = newNode(newElement(MULTIPLY_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1699 "y.tab.c"
    break;

  case 29: /* DIVIDE: '(' '/' expr expr ')'  */
#line 365 "src/main.y"
                                    {(yyval.node) = newNode(newElement(DIVIDE_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1705 "y.tab.c"
    break;

  case 30: /* MODULUS: '(' mod expr expr ')'  */
#line 367 "src/main.y"
                                    {(yyval.node) = newNode(newElement(MOD_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1711 "y.tab.c"
    break;

  case 31: /* GREATER: '(' '>' expr expr ')'  */
#line 369 "src/main.y"
                                    {(yyval.node) = newNode(newElement(GREATER_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1717 "y.tab.c"
    break;

  case 32: /* SMALLER: '(' '<' expr expr ')'  */
#line 371 "src/main.y"
                                    {(yyval.node) = newNode(newElement(SMALLER_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1723 "y.tab.c"
    break;

  case 33: /* EQUAL: '(' '=' expr exprs ')'  */
#line 373 "src/main.y"
                                    {(yyval.node) = newNode(newElement(EQUAL_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1729 "y.tab.c"
    break;

  case 34: /* LOGICAL_OP: AND_OP  */
#line 376 "src/main.y"
                    {(yyval.node) = (yyvsp[0].node);}
#line 1735 "y.tab.c"
    break;

  case 35: /* LOGICAL_OP: OR_OP  */
#line 377 "src/main.y"
                    {(yyval.node) = (yyvsp[0].node);}
#line 1741 "y.tab.c"
    break;

  case 36: /* LOGICAL_OP: NOT_OP  */
#line 378 "src/main.y"
                    {(yyval.node) = (yyvsp[0].node);}
#line 1747 "y.tab.c"
    break;

  case 37: /* AND_OP: '(' and expr exprs ')'  */
#line 380 "src/main.y"
                                    {(yyval.node) = newNode(newElement(AND_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1753 "y.tab.c"
    break;

  case 38: /* OR_OP: '(' or expr exprs ')'  */
#line 382 "src/main.y"
                                    {(yyval.node) = newNode(newElement(OR_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));}
#line 1759 "y.tab.c"
    break;

  case 39: /* NOT_OP: '(' not expr ')'  */
#line 384 "src/main.y"
                                {(yyval.node) = newNode(newElement(NOT_TYPE, NULL, 0), (yyvsp[-1].node), NULL);}
#line 1765 "y.tab.c"
    break;

  case 40: /* def_stmt: '(' define VARIABLE expr ')'  */
#line 387 "src/main.y"
                                          {
                (yyval.node) = newNode(newElement(DEFINE_VARIABLE_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node));
            }
#line 1773 "y.tab.c"
    break;

  case 41: /* VARIABLE: ID  */
#line 391 "src/main.y"
                {char* temp = (char *)malloc(sizeof(char*)); strcpy(temp, (yyvsp[0].str)); (yyval.node) = newNode(newElement(VARIABLE_TYPE, temp, 0), NULL, NULL);}
#line 1779 "y.tab.c"
    break;

  case 51: /* IF_expr: '(' IF TEST_expr THEN_expr ELSE_expr ')'  */
#line 412 "src/main.y"
                                                    {Node* expr_node = newNode(newElement(THEN_ELSE_TYPE, NULL, 0), (yyvsp[-2].node), (yyvsp[-1].node)); (yyval.node) = newNode(newElement(IF_TYPE, NULL, 0), (yyvsp[-3].node), expr_node);}
#line 1785 "y.tab.c"
    break;

  case 52: /* TEST_expr: expr  */
#line 414 "src/main.y"
                    {}
#line 1791 "y.tab.c"
    break;

  case 53: /* THEN_expr: expr  */
#line 416 "src/main.y"
                    {}
#line 1797 "y.tab.c"
    break;

  case 54: /* ELSE_expr: expr  */
#line 418 "src/main.y"
                    {}
#line 1803 "y.tab.c"
    break;

  case 57: /* exprs: exprs expr  */
#line 424 "src/main.y"
                        {(yyval.node) = newNode(newElement(EXPRS_TYPE, NULL, 0), (yyvsp[-1].node), (yyvsp[0].node));}
#line 1809 "y.tab.c"
    break;

  case 58: /* exprs: expr  */
#line 425 "src/main.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 1815 "y.tab.c"
    break;


#line 1819 "y.tab.c"

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
  ++yynerrs;

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
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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

#line 429 "src/main.y"

void yyerror(const char *s) {
    printf("syntax error ::%s \n", s);
    exit(0);
}
int main() {
    yyparse(); 
    eval(root, EMPTY_TYPE);
    return 0;
}
