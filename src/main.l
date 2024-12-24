%{
#include<stdio.h>
#include<string.h>
#include "y.tab.h"
%}
letter      [a-z]
digit       [0-9]
%%

"print-num"     {return(print_num);}
"print-bool"    {return(print_bool);}
"mod"           {return(mod);}
"and"           {return(and);}
"or"            {return(or);}
"not"           {return(not);}
"if"            {return(IF);}
"define"        {return(define);}
"fun"           {return(fun);}
"("             {return('(');}
")"             {return(')');}
"+"             {return('+');}
"-"             {return('-');}
"*"             {return('*');}
"/"             {return('/');}
"="             {return('=');}
"<"             {return('<');}
">"             {return('>');}
"0"|[1-9]{digit}*|-[1-9]{digit}*        {yylval.num = atoi(yytext); return(number);}
{letter}({letter}|{digit}|"-")*          {yylval.str = yytext; return(ID);}
"#t"|"#f"                          { if(yytext[1] == 'f')yylval.num=0; if(yytext[1]== 't')yylval.num=1; return(bool_val);}
. ;
%%

