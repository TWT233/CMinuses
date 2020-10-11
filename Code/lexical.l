%{

%}


%%


[0-9]+                      {return DATA_INT;}
^[-+]?[0-9]*\.?[0-9]+$      {return DATA_FLOAT;}
([_a-zA-Z][_a-zA-Z0-9]*)    {return ID;}
">="                        {return OP_REL_GE;}
"<="                        {return OP_REL_LE;}
">"                         {return OP_REL_G;}
"<"                         {return OP_REL_L;}
"=="                        {return OP_REL_EQ;}
"!="                        {return OP_REL_NE;}
"&&"                        {return OP_BOO_AND;}
"||"                        {return OP_BOO_OR;}
"!"                         {return OP_BOO_NOT;}
"+"                         {return OP_CAL_PLUS;}
"-"                         {return OP_CAL_MINUS;}
"*"                         {return OP_CAL_STAR;}
"/"                         {return OP_CAL_DIV;}
"="                         {return OP_ASSIGN;}
"."                         {return OP_DOT;}
";"                         {return SEMI;}
","                         {return COMMA;}
"("                         {return LP;}
")"                         {return RP;}
"["                         {return LB;}
"]"                         {return RB;}
"{"                         {return LC;}
"}"                         {return RC;}
"int"                       {return KWD_INT;}
"float"                     {return KWD_FLOAT;}
"struct"                    {return KWD_STRUCT;}
"return"                    {return KWD_RETURN;}
"if"                        {return KWD_IF;}
"else"                      {return KWD_ELSE;}
"while"                     {return KWD_WHILE;}


%%

