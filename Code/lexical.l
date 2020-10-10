%{

%}


%%


[0-9]+                      {return INT;}
^[-+]?[0-9]*\.?[0-9]+$      {return FLOAT;}
([_a-zA-Z][_a-zA-Z0-9]*)    {return ID;}
";"                         {return SEMI;}
","                         {return COMMA;}
"="                         {return ASSIGNOP;}
">"                         {return RELOP_G;}
"<"                         {return RELOP_L;}
">="                        {return RELOP_GE;}
"<="                        {return RELOP_LE;}
"=="                        {return RELOP_EQ;}
"!="                        {return RELOP_NE;}
"+"                         {return PLUS;}
"-"                         {return MINUS;}
"*"                         {return STAR;}
"/"                         {return DIV;}
"&&"                        {return AND;}
"||"                        {return OR;}
"."                         {return DOT;}
"!"                         {return NOT;}
"int"                       {return TYPE_INT;}
"float"                     {return TYPE_FLOAT;}
"("                         {return LP;}
")"                         {return RP;}
"["                         {return LB;}
"]"                         {return RB;}
"{"                         {return LC;}
"}"                         {return RC;}
"struct"                    {return STRUCT;}
"return"                    {return RETURN;}
"if"                        {return IF;}
"else"                      {return ELSE;}
"while"                     {return WHILE;}


%%

