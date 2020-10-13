%locations

%nonassoc INT FLOAT 
%nonassoc ID 
%nonassoc SEMI LC RC

%nonassoc LOWER_THAN_ELSE                   // helper
%nonassoc IF ELSE WHILE STRUCT RETURN TYPE  // operator

%left COMMA                                 // no precedence, 
                                            // but with associativity

%right ASSIGNOP                             // precedence 8
%left OR 
%left AND 
%left RELOP 
%left PLUS MINUS 
%left STAR DIV 
%right NOT 
%left LP RP LB RB DOT                       // precedence 1

%{
#include "lex.yy.c"

#define PRINT_NAME(x) do{ printf(#x" (%d)\n", yyloc.first_line);}while(0);

int yyerror(char* msg) {
    fprintf(stderr, "error: %s\n", msg);
    return 1;
}
%}

%%

// A.1.2 High-level Definitions

Program : ExtDefList                            {PRINT_NAME(Program)}
    ;

ExtDefList : ExtDef ExtDefList                  {PRINT_NAME(ExtDefList)}
    | /* Empty */
    ;

ExtDef : Specifier ExtDecList SEMI              {PRINT_NAME(ExtDef)}
    | Specifier SEMI                            {PRINT_NAME(ExtDef)}
    | Specifier FunDec CompSt                   {PRINT_NAME(ExtDef)}
    ;

ExtDecList : VarDec                             {PRINT_NAME(ExtDecList)}
    | VarDec COMMA ExtDecList                   {PRINT_NAME(ExtDecList)}
    ;

// A.1.3 Specifiers

Specifier : TYPE                                {PRINT_NAME(Specifier)}
    | StructSpecifier                           {PRINT_NAME(Specifier)}
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   {PRINT_NAME(StructSpecifier)}
    | STRUCT Tag                                {PRINT_NAME(StructSpecifier)}
    ;

OptTag : ID                                     {PRINT_NAME(OptTag)}
    | /* Empty */
    ;

Tag : ID                                        {PRINT_NAME(Tag)}
    ;

// A.1.4 Declarators

VarDec : ID                                     {PRINT_NAME(VarDec)}
    | VarDec LB INT RB                          {PRINT_NAME(VarDec)}
    ;

FunDec : ID LP VarList RP                       {PRINT_NAME(FunDec)}
    | ID LP RP                                  {PRINT_NAME(FunDec)}
    ;

VarList : ParamDec COMMA VarList                {PRINT_NAME(VarList)}
    | ParamDec                                  {PRINT_NAME(VarList)}
    ;

ParamDec : Specifier VarDec                     {PRINT_NAME(ParamDec)}
    ;

// A.1.5 Statements

CompSt : LC DefList StmtList RC                 {PRINT_NAME(CompSt)}
    ;

StmtList : Stmt StmtList                        {PRINT_NAME(StmtList)}
    | /* Empty */
    ;

Stmt : Exp SEMI                                 {PRINT_NAME(Stmt)}
    | CompSt                                    {PRINT_NAME(Stmt)}
    | RETURN Exp SEMI                           {PRINT_NAME(Stmt)}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   {PRINT_NAME(Stmt)}
    | IF LP Exp RP Stmt ELSE Stmt               {PRINT_NAME(Stmt)}
    | WHILE LP Exp RP Stmt                      {PRINT_NAME(Stmt)}
    ;

// A.1.6 Local Definitions

DefList : Def DefList                           {PRINT_NAME(DefList)}
    | /* Empty */
    ;

Def : Specifier DecList SEMI                    {PRINT_NAME(Def)}
    ;

DecList : Dec                                   {PRINT_NAME(DecList)}
    | Dec COMMA DecList                         {PRINT_NAME(DecList)}
    ;

Dec : VarDec                                    {PRINT_NAME(Dec)}
    | VarDec ASSIGNOP Exp                       {PRINT_NAME(Dec)}
    ;

// A.1.7 Expressions

Exp : Exp ASSIGNOP Exp                          {PRINT_NAME(Exp)}
    | Exp AND Exp                               {PRINT_NAME(Exp)}
    | Exp OR Exp                                {PRINT_NAME(Exp)}
    | Exp RELOP Exp                             {PRINT_NAME(Exp)}
    | Exp PLUS Exp                              {PRINT_NAME(Exp)}
    | Exp MINUS Exp                             {PRINT_NAME(Exp)}
    | Exp STAR Exp                              {PRINT_NAME(Exp)}
    | Exp DIV Exp                               {PRINT_NAME(Exp)}
    | LP Exp RP                                 {PRINT_NAME(Exp)}
    | MINUS Exp                                 {PRINT_NAME(Exp)}
    | NOT Exp                                   {PRINT_NAME(Exp)}
    | ID LP Args RP                             {PRINT_NAME(Exp)}
    | ID LP RP                                  {PRINT_NAME(Exp)}
    | Exp LB Exp RB                             {PRINT_NAME(Exp)}
    | Exp DOT ID                                {PRINT_NAME(Exp)}
    | ID                                        {PRINT_NAME(Exp)}
    | INT                                       {PRINT_NAME(Exp)}
    | FLOAT                                     {PRINT_NAME(Exp)}
    ;

Args : Exp COMMA Args                           {PRINT_NAME(Args)}
    | Exp                                       {PRINT_NAME(Args)}
    ;

%%
