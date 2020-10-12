%{
#include "lex.yy.c"

int yyerror(char* msg) {
    fprintf(stderr, "error: %s\n", msg);
    return 1;
}
%}

%nonassoc INT 
%nonassoc FLOAT 
%nonassoc ID 
%nonassoc SEMI 
%nonassoc LC 
%nonassoc RC 
%nonassoc TYPE 
%nonassoc STRUCT 
%nonassoc RETURN 
%nonassoc IF 

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE 

%nonassoc WHILE 

%left COMMA 

%right ASSIGNOP 

%left OR 

%left AND 

%left RELOP 

%left PLUS MINUS 

%left STAR DIV 

%right NOT 

%left DOT 
%left LB RB 
%left LP RP 

%%

// A.1.2 High-level Definitions

Program : ExtDefList
    ;

ExtDefList : ExtDef ExtDefList
    | /* Empty */
    ;

ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;

ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;

// A.1.3 Specifiers

Specifier : TYPE
    | StructSpecifier
    ;

StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;

OptTag : ID
    | /* Empty */
    ;

Tag : ID
    ;

// A.1.4 Declarators

VarDec : ID
    | VarDec LB INT RB
    ;

FunDec : ID LP VarList RP
    | ID LP RP
    ;

VarList : ParamDec COMMA VarList
    | ParamDec
    ;

ParamDec : Specifier VarDec
    ;

// A.1.5 Statements

CompSt : LC DefList StmtList RC
    ;

StmtList : Stmt StmtList
    | /* Empty */
    ;

Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;

// A.1.6 Local Definitions

DefList : Def DefList
    | /* Empty */
    ;

Def : Specifier DecList SEMI
    ;

DecList : Dec
    | Dec COMMA DecList
    ;

Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;

// A.1.7 Expressions

Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;

Args : Exp COMMA Args
    | Exp
    ;

%%
