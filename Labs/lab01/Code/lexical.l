%{

#include "tokens.h"
#include <stdio.h>

%}

d  [0-9]
w  [A-Za-z0-9_]
s  [ \f\t\v]

%%


([1-9]{d}{0,31})|0              {return T_DATA_INT_DEC;}
0[0-7]{1,31}                    {return T_DATA_INT_OCT;}
0(x|X)[0-9a-fA-F]{1,31}         {return T_DATA_INT_HEX;}
[+-]?({d}*\.{d}+|{d}+\.{d}*)    {return T_DATA_FLOAT;}

">="                            {return T_OP_REL_GE;}
"<="                            {return T_OP_REL_LE;}
">"                             {return T_OP_REL_G;}
"<"                             {return T_OP_REL_L;}
"=="                            {return T_OP_REL_EQ;}
"!="                            {return T_OP_REL_NE;}
"&&"                            {return T_OP_BOO_AND;}
"||"                            {return T_OP_BOO_OR;}
"!"                             {return T_OP_BOO_NOT;}
"+"                             {return T_OP_CAL_PLUS;}
"-"                             {return T_OP_CAL_MINUS;}
"*"                             {return T_OP_CAL_STAR;}
"/"                             {return T_OP_CAL_DIV;}
"="                             {return T_OP_ASSIGN;}
"."                             {return T_OP_DOT;}

"int"                           {return T_KWD_TYPE_INT;}
"float"                         {return T_KWD_TYPE_FLOAT;}
"struct"                        {return T_KWD_STRUCT;}
"return"                        {return T_KWD_RETURN;}
"if"                            {return T_KWD_IF;}
"else"                          {return T_KWD_ELSE;}
"while"                         {return T_KWD_WHILE;}

[_a-zA-Z]{w}*                   {return T_ID;}
";"                             {return T_SEMI;}
","                             {return T_COMMA;}
"("                             {return T_LP;}
")"                             {return T_RP;}
"["                             {return T_LB;}
"]"                             {return T_RB;}
"{"                             {return T_LC;}
"}"                             {return T_RC;}

\n                              {++yylineno;}
{s}+                            {}

.                               {
                                    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);
                                    return T_UNKNOWN;
                                }

%%

