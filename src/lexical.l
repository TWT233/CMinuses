%{
#include <stdio.h>

#include "syntax.tab.h"
#include "gtree.h"
#include "utils.h"

int yycolumn = 1;

extern int error_mark;

#define YY_USER_ACTION                             \
  yylloc.first_line = yylloc.last_line = yylineno; \
  yylloc.first_column = yycolumn;                  \
  yylloc.last_column = yycolumn + yyleng - 1;      \
  yycolumn += yyleng;

#define TEXTCOPY strcpy(malloc(strlen(yytext) + 1), yytext)

#define NEW_TOKEN(type)                                                \
  yylval.t_g = t_new(d_new(TEXTCOPY, gpos_new(&yylloc), type, #type)); \
  return type;

#define NEW_DEC                                                   \
  yylval.t_g = t_new(d_new_int(atoi(yytext), gpos_new(&yylloc))); \
  return INT;

#define NEW_OCT                                                   \
  yylval.t_g = t_new(d_new_int(otoi(yytext), gpos_new(&yylloc))); \
  return INT;

#define NEW_HEX                                                   \
  yylval.t_g = t_new(d_new_int(htoi(yytext), gpos_new(&yylloc))); \
  return INT;

#define NEW_FLT                                                   \
  yylval.t_g = t_new(d_new_flt(atof(yytext), gpos_new(&yylloc))); \
  return FLOAT;

%}

%option yylineno

d  [0-9]
w  [A-Za-z0-9_]
s  [ \f\t\v\r]

RAW_F [+-]?({d}*\.{d}+|{d}+\.{d}*)

%%


">="                            {NEW_TOKEN(RELOP)}
"<="                            {NEW_TOKEN(RELOP)}
">"                             {NEW_TOKEN(RELOP)}
"<"                             {NEW_TOKEN(RELOP)}
"=="                            {NEW_TOKEN(RELOP)}
"!="                            {NEW_TOKEN(RELOP)}
"&&"                            {NEW_TOKEN(AND)}
"||"                            {NEW_TOKEN(OR)}
"!"                             {NEW_TOKEN(NOT)}
"+"                             {NEW_TOKEN(PLUS)}
"-"                             {NEW_TOKEN(MINUS)}
"*"                             {NEW_TOKEN(STAR)}
"/"                             {NEW_TOKEN(DIV)}
"="                             {NEW_TOKEN(ASSIGNOP)}
"."                             {NEW_TOKEN(DOT)}

"int"                           {NEW_TOKEN(TYPE)}
"float"                         {NEW_TOKEN(TYPE)}
"struct"                        {NEW_TOKEN(STRUCT)}
"return"                        {NEW_TOKEN(RETURN)}
"if"                            {NEW_TOKEN(IF)}
"else"                          {NEW_TOKEN(ELSE)}
"while"                         {NEW_TOKEN(WHILE)}

";"                             {NEW_TOKEN(SEMI)}
","                             {NEW_TOKEN(COMMA)}
"("                             {NEW_TOKEN(LP)}
")"                             {NEW_TOKEN(RP)}
"["                             {NEW_TOKEN(LB)}
"]"                             {NEW_TOKEN(RB)}
"{"                             {NEW_TOKEN(LC)}
"}"                             {NEW_TOKEN(RC)}


([+-]?[1-9]{d}{0,31})|0         {NEW_DEC}
[+-]?0[0-7]{1,31}               {NEW_OCT}
[+-]?0[xX][0-9a-fA-F]{1,31}     {NEW_HEX}
{RAW_F}([eE][+-]?{d}+)?         {NEW_FLT}
[_a-zA-Z]{w}*                   {NEW_TOKEN(ID)}

{s}+                            {}
\n                              {yycolumn = 1;}

.                               {
                                    error_mark = 1;
                                    printf("Error type A at Line %d: Mysterious characters \"%s\".\n",yylineno,yytext);
                                }

%%

