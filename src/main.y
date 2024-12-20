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
#define EXPRS_TYPE 15
#define AND_TYPE 16
#define OR_TYPE 17
#define DEFINE_TYPE 18
#define IF_TYPE 19
#define THEN_ELSE_TYPE 20

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
%type <node> exprs
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
%type <node> TEST_expr
%type <node> THEN_expr
%type <node> ELSE_expr
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

PLUS    :   '(' '+' expr exprs ')'  {$$ = newNode(newElement(PLUS_TYPE, NULL, 0), $3, $4);}
        ;
MINUS   :   '(' '-' expr expr ')'   {$$ = newNode(newElement(MINUS_TYPE, NULL, 0), $3, $4);}
        ;
MULTIPLY:   '(' '*' expr exprs ')'  {$$ = newNode(newElement(MULTIPLY_TYPE, NULL, 0), $3, $4);}
        ;
DIVIDE  :   '(' '/' expr expr ')'   {$$ = newNode(newElement(DIVIDE_TYPE, NULL, 0), $3, $4);}
        ;
MODULUS :   '(' mod expr expr ')'   {$$ = newNode(newElement(MOD_TYPE, NULL, 0), $3, $4);}
        ;
GREATER :   '(' '>' expr expr ')'   {$$ = newNode(newElement(GREATER_TYPE, NULL, 0), $3, $4);}
        ;
SMALLER :   '(' '<' expr expr ')'   {$$ = newNode(newElement(SMALLER_TYPE, NULL, 0), $3, $4);}
        ;
EQUAL   :   '(' '=' expr exprs ')'  {$$ = newNode(newElement(EQUAL_TYPE, NULL, 0), $3, $4);}
        ;

LOGICAL_OP: AND_OP  {$$ = $1;}
          | OR_OP   {$$ = $1;}    
          | NOT_OP  {$$ = $1;}
          ;
AND_OP  :   '(' and expr exprs ')'  {$$ = newNode(newElement(AND_TYPE, NULL, 0), $3, $4);}
        ;
OR_OP   :   '(' or expr exprs ')'   {$$ = newNode(newElement(OR_TYPE, NULL, 0), $3, $4);} 
        ;
NOT_OP  :   '(' not expr ')'    {$$ = newNode(newElement(NOT_TYPE, NULL, 0), $3, NULL);}
        ;

def_stmt:   '(' define VARIABLE expr ')'  {$$ = newNode(newElement(DEFINE_TYPE, NULL, 0), $3, $4);}
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
IF_expr :  '(' IF TEST_expr THEN_expr ELSE_expr ')' {Node* expr_node = newNode(newElement(THEN_ELSE_TYPE, NULL, 0), $4, $5); $$ = newNode(newElement(IF_TYPE, NULL, 0), $3, expr_node);}
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

exprs   :   exprs expr  {$$ = newNode(newElement(EXPRS_TYPE, NULL, 0), $1, $2);}  
        |   expr        {$$ = $1;}
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
