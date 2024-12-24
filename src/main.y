%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// define type table
#define False 0
#define True 1
#define DEBUG_MODE 0

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
#define FUN_CALL 23
#define FUN_TYPE 24
#define IDS_TYPE 25
#define PARAMS_TYPE 26
#define FUN_NAME_TYPE 27
#define ID_TYPE 28
#define DEFINE_FUNCTION_TYPE 29
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

typedef struct Function {
    char* name;
    struct Node* params;
    struct Node* body;
} Function;

Node* newNode(Element* val, Node* left, Node* right);
Node* copyNode(Node* node);
Node* freeNode(Node* node);
void dump(Node* node, int idx);
void eval(Node* node, int type);
void assignParamsNameAndBind(Node* ids, Node* params, Node* func);
void bindParams(Node* taskNode, Node* toReplace);
Node* root;

Node* variables[MAX_SIZE];
int top_variables = -1;


Function* functions[MAX_SIZE];
int top_functions = -1;


Function* newFunction(char* name, Node* func){
    Function* temp = (Function *)malloc(sizeof(Function));
    temp->name = name;
    temp->params = func->left;
    temp->body = func->right;
    functions[++top_functions] = temp;
    return temp;
}

Function* getFunction(char* name) {
    for(int i = 0; i <= top_functions; i++) {
        if(strcmp(functions[i]->name, name) == 0) {
            Function* result = (Function *) malloc(sizeof(Function));

            result->name = strdup(functions[i]->name);
            result->params = copyNode(functions[i]->params);
            result->body = copyNode(functions[i]->body);

            return result;
        }
    }
    return NULL;
}

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

void assignParamsNameAndBind(struct Node* ids, struct Node* params, struct Node* func) {
    
    switch(ids->val->type) {
        case EMPTY_TYPE:
            return;
        case ID_TYPE:
            params->val->cval = ids->val->cval;
            
            if(DEBUG_MODE) {
                printf("to assign: %d (%d)\n", params->val->ival, params->val->type);
            }

            bindParams(func, copyNode(params));

            break;
        case IDS_TYPE:
            params->right->val->cval = ids->right->val->cval;
            
            if(DEBUG_MODE) {
                printf("to assign: %d (%d)\n", params->right->val->ival, params->right->val->type);
            }

            bindParams(func, copyNode(params->right));

            assignParamsNameAndBind(ids->left, params->left, func);
            break;
        default:
            printf("unknown at bind, type: %d\n", ids->val->type);
            break;

    }
}

void bindParams(struct Node* taskNode, struct Node* toReplace) {
    if(taskNode == NULL || taskNode->val->type == DEFINE_FUNCTION_TYPE) {
        return;
    }

    if(taskNode->val->type == ID_TYPE && toReplace->val->type == FUN_TYPE) {
        if(DEBUG_MODE) {
            printf("bind: %d -> ", taskNode->val->type);
        }

        taskNode->val->type = toReplace->val->type;

        taskNode->val->ival = toReplace->val->ival;

        taskNode->left = copyNode(toReplace->left);
        taskNode->right = copyNode(toReplace->right);

        if(DEBUG_MODE) {
            printf("%d\n", taskNode->val->type);
        }

        return;
    } else if(taskNode->val->type == VARIABLE_TYPE) {
        if(strcmp(taskNode->val->cval, toReplace->val->cval) == 0) {
            if(DEBUG_MODE) {
                printf("bind: %d -> ", taskNode->val->type);
            }

            taskNode->val->type = toReplace->val->type;

            taskNode->val->ival = toReplace->val->ival;

            taskNode->left = copyNode(toReplace->left);
            taskNode->right = copyNode(toReplace->right);

            if(DEBUG_MODE) {
                printf("%d\n", taskNode->val->type);
            }

            return;
        }
    }

    bindParams(taskNode->left, toReplace);
    bindParams(taskNode->right, toReplace);
}
Node* freeNode(Node* node) {
    if(node == NULL) return NULL;
    freeNode(node->left);
    freeNode(node->right);
    free(node);
    return NULL;
}
void dump(Node* node, int idx){
    if(node == NULL)return;
    printf("IDX: %d, TYPE: %d\n", idx, node->val->type);
    printf("LEFT\n");
    dump(node->left, idx+1);
    printf("RIGHT\n");
    dump(node->right, idx+1);
}
void eval(Node* node, int type) {
    if(node == NULL)  {
        return;
    }
    if(DEBUG_MODE){
        printf("<--EVAL %d-->\n", node->val->type);
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
        case INT_TYPE:
            break;
        case STR_TYPE:
            break;
        case BOOL_TYPE:
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
            break;
        case MOD_TYPE:
            eval(node->left, node->val->type);
            eval(node->right, node->val->type);
            if(node->left->val->type != INT_TYPE) yyerror("Type error");
            if(node->right->val->type != INT_TYPE) yyerror("Type error");
            node->val->ival = node->left->val->ival % node->right->val->ival;
            node->val->type = INT_TYPE;
            if(DEBUG_MODE){
                printf("MOD_TYPE, VAL=%d\n", node->val->ival);
            }
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
 
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
 
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
 
            break;
        case EQUAL_TYPE:
            /*
                e.g.:
                    (= a b c d)
                     EQUAL
                    a  EXPRS
                        EXPRS d
                        b c
            */
            if(DEBUG_MODE){
                printf("EQUAL\n");
            }
            if(node->left->val->type == EXPRS_TYPE && node->right->val->type != EXPRS_TYPE){
                eval(node->right, node->val->type);
                int temp = node->right->val->ival;
                temp = node->left->right->val->ival;
                eval(node->left, node->val->type);
                node->val->type = BOOL_TYPE;
                node->val->ival = (node->right->val->ival == temp) && node->left->val->ival;
            } else if(node->left->val->type != EXPRS_TYPE && node->right->val->type == EXPRS_TYPE){
                eval(node->left, node->val->type);
                int temp = node->left->val->ival;
                temp = node->right->right->val->ival;
                eval(node->right, node->val->type);
                node->val->type = BOOL_TYPE;
                node->val->ival = (node->left->val->ival == temp) && node->right->val->ival;
            } else if(node->left->val->type != EXPRS_TYPE && node->right->val->type != EXPRS_TYPE){
                eval(node->left, node->val->type);
                eval(node->right, node->val->type);
                node->val->type = BOOL_TYPE;
                node->val->ival = node->left->val->ival == node->right->val->ival;
            } else {
                yyerror("UNKNOWN ERROR");
            }
            if(DEBUG_MODE){
                printf("VAL=%d\n", node->val->ival);
            }
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
 
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
 
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
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
 
            break;
        case NOT_TYPE:
            eval(node->left, node->val->type);
            if(node->left->val->type != BOOL_TYPE) yyerror("Type error");
            node->val->ival = !node->left->val->ival;
            node->val->type = BOOL_TYPE;
            if(DEBUG_MODE){
                printf("NOT_TYPE, VAL=%d\n", node->val->ival);
            }
            node->left = freeNode(node->left);
            node->right = freeNode(node->right);
 
            break;
        case IF_TYPE:
            if(DEBUG_MODE){
                printf("IF TYPE\n");
            }
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
                printf("VARIABLE TYPE %d\n", node->right->val->type);
            }
            break;
        case VARIABLE_TYPE:
            if(DEBUG_MODE){
                printf("EVAL VARIABLE %s\n", node->val->cval);
            }
            for(int i=0;i<=top_variables;i++){
                if(strcmp(variables[i]->val->cval,node->val->cval) == 0) {
                    Node* temp = copyNode(variables[i]);
                    node->val->type = temp->val->type;
                    node->val->ival = temp->val->ival;
                    node->val->cval = temp->val->cval;
                    node->left = copyNode(temp->left);
                    node->right = copyNode(temp->right);
                    eval(node, node->val->type);
                    break;
                }
            }
            break;
        case FUN_CALL:
            if(DEBUG_MODE){
                printf("CALL A FUNCTION\n");
            }
            /* 
                NODE FUN_EXPR PARAMS
                FUN_EXPR IDS BODY
            */
            eval(node->left, node->val->type);
            assignParamsNameAndBind(node->left->left, node->right, node->left->right);
            eval(node->left->right, node->left->val->type);

            node->val->type = node->left->right->val->type;
            node->val->ival = node->left->right->val->ival;
            node->val->cval = node->left->right->val->cval;
            if(node->left->right->val->type == FUN_TYPE){
                node->left = copyNode(node->left->right->left);
                node->right = copyNode(node->left->right->right);
                eval(node, node->val->type);
            }
            break;
        default:
            if(DEBUG_MODE) {
                printf("DEFAULT TYPE:%d \n", node->val->type);
                if(node->val->type == INT_TYPE){
                    printf("NUM_VAL: %d\n", node->val->ival);
                }
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
%token <num> number
%token <num> bool_val
%token <str> ID
%token  and 
%token  or
%token  not
%token  mod
%token  define
%token  print_num
%token  print_bool
%token  IF
%token  fun

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
%type <node> FUN_Body
%type <node> FUN_Name
%type <node> FUN_IDs
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
%type <node> IDs
%type <node> PARAM
%type <node> PARAMs
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

def_stmt:   '(' define VARIABLE expr ')'  {
                $$ = newNode(newElement(DEFINE_VARIABLE_TYPE, NULL, 0), $3, $4);
            }
        ;
VARIABLE:   ID  {
                $$ = newNode(newElement(VARIABLE_TYPE, strdup($1), 0), NULL, NULL);
            }
        ;

FUN_expr:   '(' fun FUN_IDs FUN_Body ')' { 
                $$ = newNode(newElement(FUN_TYPE, NULL, 0), $3, $4);
            }
        ;

FUN_IDs :   '(' IDs ')'     {$$ = $2;}
        ;

FUN_Body:   expr            {$$ = $1;}
        ;
FUN_Call:   '(' expr PARAMs ')'     {
                $$ = newNode(newElement(FUN_CALL, NULL, 0), $2, $3);
            }
        ;
PARAMs  :   PARAMs PARAM    {$$ = newNode(newElement(PARAMS_TYPE, NULL, 0), $1, $2);}
        |   /* empty */     {$$ = newNode(newElement(EMPTY_TYPE, NULL, 0), NULL, NULL);}
        ;
PARAM   :   expr    {$$ = $1;}
        ;
FUN_Name:   ID      {
                $$ = newNode(newElement(FUN_NAME_TYPE, strdup($1), 0), NULL, NULL);
            }
        ;
IF_expr :  '(' IF TEST_expr THEN_expr ELSE_expr ')' {
                Node* expr_node = newNode(newElement(THEN_ELSE_TYPE, NULL, 0), $4, $5); 
                $$ = newNode(newElement(IF_TYPE, NULL, 0), $3, expr_node);
            }
        ;
TEST_expr:  expr    {$$ = $1;}
         ;
THEN_expr:  expr    {$$ = $1;}
         ;
ELSE_expr:  expr    {$$ = $1;}
         ;
IDs     :   IDs ID      {
                Node* temp = newNode(newElement(ID_TYPE, strdup($2), 0), NULL, NULL);
                $$ = newNode(newElement(IDS_TYPE, NULL, 0), $1, temp);
            }
        |   /* empty */ {$$ = newNode(newElement(EMPTY_TYPE, NULL, 0), NULL, NULL);}
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
