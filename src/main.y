%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int yylex();
void yyerror(const char *s);

int num_variables = 0;
char variable_keys[100][100];
int variable_values[100];
int getVariableIndex(char key[]){
    for(int i=0;i<num_variables;i++){
        if(strcmp(variable_keys[i], key) == 0){
            return i;
        }
    }
    return -1;
}
void assignVariable(char key[], int value){
    int index = getVariableIndex(key);
    if(index == -1){
        strcpy(variable_keys[num_variables], key);
        variable_values[num_variables] = value;
        num_variables++;
    } else {
        variable_values[index] = value;
    }
}
int getVariable(char key[]){
    int index = getVariableIndex(key);
    if(index == -1){
        return -1;
    } else {
        return variable_values[index];
    }
}
void printVariableTable() {
    printf("VARIABLE TABLE\n");
    for(int i=0;i<num_variables;i++){
        printf("%s: %d\n", variable_keys[i], variable_values[i]); 
    }
}

typedef struct ListNode {
    int val;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

ListNode* createNode(int data) {
    ListNode *temp = (ListNode*)malloc(sizeof(ListNode));
    temp->val = data;
    temp->next = temp;
    temp->prev = temp;
    return temp;
}
void insertEnd(ListNode** head, int data){
    // printf("INSERT HEAD = %d \n", (*head)->val);
    ListNode* newNode = createNode(data);
    ListNode** tail = &((*head)->prev);
    (*tail)->next = newNode;
    (*head)->prev = newNode;
    newNode->next = *head;
    newNode->prev = *tail;
}

int iterativeSum(ListNode** head){
    int sum = 0;
    ListNode* temp = *head;
    //printf("START SUM");
    do {
        //printf("%d", temp->val);
        sum += temp->val;
        temp = temp->next;
    }while(temp != *head);
    return sum;
}
int iterativeProduct(ListNode** head){
    int product = 1;
    ListNode* temp = *head;
    do {
        product *= temp->val;
        temp = temp->next;
    }while(temp != *head);
    return product;
}
int iterativeEqual(ListNode** head){
    ListNode* temp = *head;
    do {
       if(temp->val != temp->next->val){
            return 0;
       }
       temp = temp->next;
    }while(temp != *head);
    return 1;
}
int iterativeAnd(ListNode** head){
    ListNode* temp = *head;
    do {
        if(temp->val != 1){
            return 0;
        }
        temp = temp->next;
    }while(temp != *head);
    return 1;
}
int iterativeOr(ListNode** head){
    ListNode* temp = *head;
    do {
        if(temp->val == 1){
            return 1;
        }
        temp = temp->next;
    }while(temp != *head);
    return 0;
}
%}
%union {
    char* str;
    int num;
    int* pNum;
    struct ListNode* List;
}
%token  <num>   number
%token  <str>   ID
%token  <num>   bool_val
%token  and 
%token  or
%token  not
%token  mod
%token  define
%token  print_num
%token  print_bool
%token  IF


%type   <List>  exprs
%type   <num>   expr
%type   <num>   NUM_OP
%type   <num>   LOGICAL_OP
%type   <num>   IF_expr
%type   <num>   TEST_expr
%type   <num>   THEN_expr
%type   <num>   ELSE_expr
%type   <num>   AND_OP
%type   <num>   OR_OP
%type   <num>   NOT_OP
%type   <num>   DIVIDE
%type   <num>   MULTIPLY
%type   <num>   EQUAL
%type   <num>   MINUS
%type   <num>   MODULUS
%type   <num>   GREATER
%type   <num>   SMALLER
%type   <num>   PLUS
%type   <str>   print_stmt
%type   <str>   VARIABLE

%left '+' '-'
%left '*' '/'
%%
program :   program stmt
        |   stmt
        ;
stmt    :   expr        {printf("EXPR %d", $1);}
        |   def_stmt    {}
        |   print_stmt  {printf("%s", $1);}
        ;

print_stmt  :   '(' print_bool expr ')'  {if($3 == 0)$$ = "false"; if($3 == 1)$$ = "true";}
            |   '(' print_num expr ')' {char *temp = malloc(12*sizeof(char)); if(temp==NULL){yyerror("Fail to allocate memory");} sprintf(temp, "%d", $3); $$ = temp;}
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

PLUS    :   '(' '+' expr exprs ')'  {$$ = $3 + iterativeSum(&$4);}
        ;
MINUS   :   '(' '-' expr expr ')'   {$$ = $3 - $4;}
        ;
MULTIPLY:   '(' '*' expr exprs ')'  {$$ = $3 * iterativeProduct(&$4);}
        ;
DIVIDE  :   '(' '/' expr expr ')'   {$$ = $3 / $4;}
        ;
MODULUS :   '(' mod expr expr ')'   {$$ = $3 % $4;}
        ;
GREATER :   '(' '>' expr expr ')'   {$$ = $3 > $4;}
        ;
SMALLER :   '(' '<' expr expr ')'   {$$ = $3 < $4;}
        ;
EQUAL   :   '(' '=' expr exprs ')'  {insertEnd(&$4, $3);$$ = iterativeEqual(&$4);}
        ;

LOGICAL_OP: AND_OP  {$$ = $1;}
          | OR_OP   {$$ = $1;}    
          | NOT_OP  {$$ = $1;}
          ;
AND_OP  :   '(' and expr exprs ')'  {insertEnd(&$4, $3); $$ = iterativeAnd(&$4);}
        ;
OR_OP   :   '(' or expr exprs ')'   {insertEnd(&$4, $3); $$ = iterativeOr(&$4);} 
        ;
NOT_OP  :   '(' not expr ')'    {if($3 == 0) {$$ = 1;} else if($3 == 1){$$ = 0;} else {yyerror("Type error!");}}
        ;

def_stmt:   '(' define VARIABLE expr ')'  {assignVariable($3, $4); }
        ;

VARIABLE:   ID  {char* temp = malloc(sizeof($1)); sprintf(temp, "%s", $1); $$ = temp;}
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
IF_expr :  '(' IF TEST_expr THEN_expr ELSE_expr ')' {if($3 == 1){$$ = $4;} else {$$ = $5;}}
        ;
TEST_expr:  expr    {$$ = $1;}
         ;
THEN_expr:  expr    {$$ = $1;}
         ;
ELSE_expr:  expr    {$$ = $1;}
         ;
IDs     :   /* empty */
        |   IDs ID
        ;

exprs   :   exprs expr  {
                insertEnd(&$1, $2)  ;
                $$ = $1;
            }
        |   expr        {
                ListNode* head = createNode($1);
                $$ = head;
            }
        ;

expr    :   NUM_OP      {$$ = $1;}
        |   LOGICAL_OP  {$$ = $1;}
        |   FUN_expr    {}
        |   FUN_Call    {}
        |   IF_expr     {$$ = $1;}
        |   VARIABLE    {$$ = getVariable($1);}
        |   number      {$$ = $1;}
        |   bool_val    {$$ = $1;}
        ;


%%
void yyerror(const char *s) {
    printf("syntax error ::%s \n", s);
    exit(0);
}
int main() {
    yyparse();
    return 0;
}
