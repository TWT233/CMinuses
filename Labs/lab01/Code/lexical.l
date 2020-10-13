%{
#include <stdio.h>

#include "syntax.tab.h"

int yycolumn = 1;

#define YY_USER_ACTION                             \
  yylloc.first_line = yylloc.last_line = yylineno; \
  yylloc.first_column = yycolumn;                  \
  yylloc.last_column = yycolumn + yyleng - 1;      \
  yycolumn += yyleng;

#define YY_T_STR yylval.t_str = yytext;
%}

%option yylineno

d  [0-9]
w  [A-Za-z0-9_]
s  [ \f\t\v\r]

RAW_F [+-]?({d}*\.{d}+|{d}+\.{d}*)

%%


">="                            {YY_T_STR return RELOP;}
"<="                            {YY_T_STR return RELOP;}
">"                             {YY_T_STR return RELOP;}
"<"                             {YY_T_STR return RELOP;}
"=="                            {YY_T_STR return RELOP;}
"!="                            {YY_T_STR return RELOP;}
"&&"                            {YY_T_STR return AND;}
"||"                            {YY_T_STR return OR;}
"!"                             {YY_T_STR return NOT;}
"+"                             {YY_T_STR return PLUS;}
"-"                             {YY_T_STR return MINUS;}
"*"                             {YY_T_STR return STAR;}
"/"                             {YY_T_STR return DIV;}
"="                             {YY_T_STR return ASSIGNOP;}
"."                             {YY_T_STR return DOT;}

"int"                           {YY_T_STR return TYPE;}
"float"                         {YY_T_STR return TYPE;}
"struct"                        {YY_T_STR return STRUCT;}
"return"                        {YY_T_STR return RETURN;}
"if"                            {YY_T_STR return IF;}
"else"                          {YY_T_STR return ELSE;}
"while"                         {YY_T_STR return WHILE;}

";"                             {YY_T_STR return SEMI;}
","                             {YY_T_STR return COMMA;}
"("                             {YY_T_STR return LP;}
")"                             {YY_T_STR return RP;}
"["                             {YY_T_STR return LB;}
"]"                             {YY_T_STR return RB;}
"{"                             {YY_T_STR return LC;}
"}"                             {YY_T_STR return RC;}


([1-9]{d}{0,31})|0              {return INT;}
0[0-7]{1,31}                    {return INT;}
0[xX][0-9a-fA-F]{1,31}          {return INT;}
{RAW_F}([eE][+-]?{d}+)?         {return FLOAT;}
[_a-zA-Z]{w}*                   {YY_T_STR return ID;}

{s}+                            {}
\n                              {yycolumn = 1;}

.                               {
                                    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);
                                }

%%

