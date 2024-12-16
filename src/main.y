%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// define type table
#define False 0
#define True 1

#define DEBUG_MODE 1
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

Node* newNode(Element* val, Node* left, Node* right) {
    Node* temp = (Node *)malloc(sizeof(Node));
    temp->val = val;
    temp->left = left;
    temp->right = right;
    return temp;
}

void eval(Node* node, int type) {
    if(DEBUG_MODE) {
        printf("<--EVAL-->\n");
    }
    if(node == NULL)  {
        return;
    }

    switch(node->val->type){
        case EMPTY_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            break;
        case PRINT_NUM_TYPE:
            eval(node->left, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            printf("%d\n", node->left->val->ival);
            break;
        case PRINT_BOOL_TYPE:
            eval(node->left, node->val->type);
            if(node->left->val->type != BOOL_TYPE) yyerror("Type error");
            if(node->left->val == 0) printf("#f\n");
            else if(node->left->val == 1) printf("#t\n");
            else {
               yyerror("BOOL Error");
            }
            break;
    }


}
%}
%union {
    char* str;
    int num;
    struct Node* node;
}
%token  number
%token  bool_val
%token  ID
%token  and 
%token  or
%token  not
%token  mod
%token  define
%token  print_num
%token  print_bool
%token  IF
%type <num> number
%type <num> bool_val
%type <str> ID


%type <node> program
%type <node> stmts
%type <node> stmt
%type <node> expr
%type <node> print_stmt
%type <node> def_stmt
%type <node> NUM_OP
%type <node> LOGICAL_OP
%type <node> NOT_OP
%type <node> OR_OP
%type <node> AND_OP
%type <node> FUN_expr
%type <node> FUN_Call
%type <node> IF_expr
%type <node> VARIABLE
%type <node> PLUS
%type <node> MINUS
%type <node> MULTIPLY
%type <node> DIVIDE
%type <node> MODULUS
%type <node> GREATER
%type <node> SMALLER
%type <node> EQUAL

%%
program :   stmts   {root = $1;}
        ;

stmts   :   stmts stmt  {$$ = newNode(newElement(EMPTY_TYPE, NULL,0), $1, $2);}
        |   stmt        {$$ = $1;}
        ;

stmt    :   expr        {$$ = $1;}
        |   def_stmt    {$$ = $1;}
        |   print_stmt  {$$ = $1;}
        ;

expr    :   NUM_OP      {$$ = $1;}
        |   LOGICAL_OP  {$$ = $1;}
        |   FUN_expr    {$$ = $1;}
        |   FUN_Call    {$$ = $1;}
        |   IF_expr     {$$ = $1;}
        |   VARIABLE    {$$ = $1;}
        |   number      {$$ = newNode(newElement(INT_TYPE, NULL, $1), NULL, NULL);}
        |   bool_val    {$$ = newNode(newElement(BOOL_TYPE, NULL, $1),NULL,NULL);}
        ;


print_stmt  :   '(' print_bool expr ')'  {$$ = newNode(newElement(PRINT_BOOL_TYPE, NULL,0), $3, NULL);}
            |   '(' print_num expr ')' {$$ = newNode(newElement(PRINT_NUM_TYPE, NULL, 0), $3, NULL);}
            ;

NUM_OP  :   PLUS        {$$ = $1;}
        |   MINUS       {$$ = $1;}
        |   MULTIPLY    {$$ = $1;}
        |   DIVIDE      {$$ = $1;}
        |   MODULUS     {$$ = $1;}
        |   GREATER     {$$ = $1;}
        |   SMALLER     {$$ = $1;}
        |   EQUAL       {$$ = $1;}
        ;

PLUS    :   '(' '+' expr exprs ')'  {}
        ;
MINUS   :   '(' '-' expr expr ')'   {$$ = newNode(newElement(MINUS_TYPE, NULL, 0), $3, $4);}
        ;
MULTIPLY:   '(' '*' expr exprs ')'  {}
        ;
DIVIDE  :   '(' '/' expr expr ')'   {$$ = newNode(newElement(DIVIDE_TYPE, NULL, 0), $3, $4);}
        ;
MODULUS :   '(' mod expr expr ')'   {$$ = newNode(newElement(MOD_TYPE, NULL, 0), $3, $4);}
        ;
GREATER :   '(' '>' expr expr ')'   {$$ = newNode(newElement(GREATER_TYPE, NULL, 0), $3, $4);}
        ;
SMALLER :   '(' '<' expr expr ')'   {$$ = newNode(newElement(SMALLER_TYPE, NULL, 0), $3, $4);}
        ;
EQUAL   :   '(' '=' expr exprs ')'  {}
        ;

LOGICAL_OP: AND_OP  {}
          | OR_OP   {}    
          | NOT_OP  {$$ = $1;}
          ;
AND_OP  :   '(' and expr exprs ')'  {}
        ;
OR_OP   :   '(' or expr exprs ')'   {} 
        ;
NOT_OP  :   '(' not expr ')'    {$$ = newNode(newElement(NOT_TYPE, NULL, 0), $3, NULL);}
        ;

def_stmt:   '(' define VARIABLE expr ')'  {}
        ;

VARIABLE:   ID  {$$ = newNode(newElement(STR_TYPE, $1, 0), NULL, NULL);}
        ;

FUN_expr:   '(' FUN_IDs FUN_Body ')'
        ;

FUN_IDs :   '(' IDs ')'
        ;

FUN_Body:   expr
        ;
FUN_Call:   '(' FUN_expr PARAMs ')'
        |   '(' FUN_Name PARAMs ')'
        ;
PARAMs  :   /* empty */
        |   PARAMs PARAM
        ;
PARAM   :   expr
        ;
FUN_Name:   ID
        ;
IF_expr :  '(' IF TEST_expr THEN_expr ELSE_expr ')' {}
        ;
TEST_expr:  expr    {}
         ;
THEN_expr:  expr    {}
         ;
ELSE_expr:  expr    {}
         ;
IDs     :   /* empty */
        |   IDs ID
        ;

exprs   :   exprs expr  {}  
        |   expr        {}
        ;


%%
void yyerror(const char *s) {
    printf("syntax error ::%s \n", s);
    exit(0);
}
int main() {
    yyparse(); 
    eval(root, EMPTY_TYPE);
    return 0;
}
