%{
#include<stdio.h>
#include<string.h>
int yylex();
void yyerror(const char *s);

%}
%union {
    char* str;
    int num;
    int* pNum;
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

%type   <num>   expr
%type   <str>   print_stmt
%type   <str>   VARIABLE
%type   <pNum>  exprs
%%
program :   program stmt
        |   stmt
        ;
stmt    :   expr
        |   def_stmt    {}
        |   print_stmt  {printf("%s", $1);}
        ;

print_stmt  :   '(' print_bool expr ')'  {if($3 == 0)$$ = "false"; if($3 == 1)$$ = "true";}
            |   '(' print_num expr ')' {char *temp = malloc(12*sizeof(char)); if(temp==NULL){printf("Fail to allocate memory");} sprintf(temp, "%d", $3); $$ = temp;}
            ;

exprs   :   exprs expr  {}
        |   expr        {}
        ;
expr    :   bool_val    {$$ = $1;}
        |   number      {$$ = $1;}
        |   VARIABLE    {}
        |   NUM_OP      {}
        |   LOGICAL_OP  {}
        |   FUN_expr    {}
        |   FUN_Call    {}
        |   IF_expr     {}
        ;

NUM_OP  :   PLUS
        |   MINUS
        |   MULTIPLY
        |   DIVIDE
        |   MODULUS
        |   GREATER
        |   SMALLER
        |   EQUAL
        ;
PLUS    :   '(' '+' expr exprs ')'  
        ;
MINUS   :   '(' '-' expr expr ')'   {$$ = $3 - $4;}
        ;
MULTIPLY:   '(' '*' expr exprs ')'
        ;
DIVIDE  :   '(' '/' expr expr ')'   {$$ = $3 / $4;}
        ;
MODULUS :   '(' mod expr expr ')'   {$$ = $3 % $4;}
        ;
GREATER :   '(' '>' expr expr ')'   {$$ = $3 > $4;}
        ;
SMALLER :   '(' '<' expr expr ')'   {$$ = $3 < $4;}
        ;
EQUAL   :   '(' '=' expr exprs ')'
        ;

LOGICAL_OP: AND_OP
          | OR_OP
          | NOT_OP
          ;
AND_OP  :   '(' and expr exprs ')'
        ;
OR_OP   :   '(' or expr exprs ')'
        ;
NOT_OP  :   '(' not expr ')'    {if($3 == 0 || $3 == 1){$$ = !$3;} else {printf("Type error!");}}
        ;

def_stmt:   '(' define VARIABLE expr ')'  {printf("DEFINE %s %d", $3, $4);}
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
TEST_expr:  expr
         ;
THEN_expr:  expr
         ;
ELSE_expr:  expr
         ;
IDs     :   /* empty */
        |   IDs ID
        ;

%%
int main() {
    yyparse();
    return 0;
}
