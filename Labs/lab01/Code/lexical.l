%{
#include <stdio.h>

#include "syntax.tab.h"
%}

%option yylineno

d  [0-9]
w  [A-Za-z0-9_]
s  [ \f\t\v\n\r]

%%


">="                            {return RELOP;}
"<="                            {return RELOP;}
">"                             {return RELOP;}
"<"                             {return RELOP;}
"=="                            {return RELOP;}
"!="                            {return RELOP;}
"&&"                            {return AND;}
"||"                            {return OR;}
"!"                             {return NOT;}
"+"                             {return PLUS;}
"-"                             {return MINUS;}
"*"                             {return STAR;}
"/"                             {return DIV;}
"="                             {return ASSIGNOP;}
"."                             {return DOT;}

"int"                           {return TYPE;}
"float"                         {return TYPE;}
"struct"                        {return STRUCT;}
"return"                        {return RETURN;}
"if"                            {return IF;}
"else"                          {return ELSE;}
"while"                         {return WHILE;}

";"                             {return SEMI;}
","                             {return COMMA;}
"("                             {return LP;}
")"                             {return RP;}
"["                             {return LB;}
"]"                             {return RB;}
"{"                             {return LC;}
"}"                             {return RC;}


([1-9]{d}{0,31})|0              {return INT;}
0[0-7]{1,31}                    {return INT;}
0(x|X)[0-9a-fA-F]{1,31}         {return INT;}
[+-]?({d}*\.{d}+|{d}+\.{d}*)    {return FLOAT;}
[_a-zA-Z]{w}*                   {return ID;}

{s}+                            {}

.                               {
                                    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);
                                }

%%

