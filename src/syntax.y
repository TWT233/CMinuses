%locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lex.yy.c"
#include "gtree.h"
#include "traverser.h"

#define NEW_SMTC(n, p, ts, ...) \
  (t_c_pushs(t_new(d_new(NULL, gpos_new(&p), 0, ts)), n, ##__VA_ARGS__))

#define CALLBACK(name, p) on_##name(p)
#define NCALLBACK(name, n, p) on_##name##_##n(p)

// user defined error reporter
gtree* ERR_REP(YYLTYPE pos, const char* missing) {
  fprintf(stderr, "Error type B at Line %d: Missing \"%s\".\n", pos.first_line,
          missing);
  error_mark = 1;
  return t_new(d_new(strcpy(malloc(strlen(missing) + 1), missing),
                     gpos_new(&pos), -1, "ERROR"));
}

// disable default yyerror()
int yyerror(const char* msg) {
    return 0;
}

gtree* root = NULL;
int error_mark = 0;

%}

%union{
    void* t_g;
}


%nonassoc   <t_g>   INT
%nonassoc   <t_g>   FLOAT
%nonassoc   <t_g>   ARRAY
%nonassoc   <t_g>   STRUC                               // represent data struct, diff from STRUCT
%nonassoc   <t_g>   FUNCT
%nonassoc   <t_g>   ID 
%nonassoc   <t_g>   SEMI LC RC

%nonassoc   <t_g>   LOWER_THAN_ELSE                     // helper
%nonassoc   <t_g>   IF ELSE WHILE STRUCT RETURN TYPE    // keywords

%left       <t_g>   COMMA                               // no precedence, 
                                                        // but with associativity

%right      <t_g>   ASSIGNOP                            // precedence 8
%left       <t_g>   OR 
%left       <t_g>   AND 
%left       <t_g>   RELOP 
%left       <t_g>   PLUS MINUS 
%left       <t_g>   STAR DIV 
%right      <t_g>   NOT 
%left       <t_g>   LP RP LB RB DOT                     // precedence 1

%nterm      <t_g>   Program ExtDefList ExtDef ExtDecList Specifier
%nterm      <t_g>   StructSpecifier OptTag Tag VarDec VarList
%nterm      <t_g>   ParamDec CompSt StmtList Stmt DefList Def DecList
%nterm      <t_g>   CompStDefList CompStDef CompStDecList CompStDec FunDefSig FunDecSig
%nterm      <t_g>   Dec Exp Args

%define parse.lac full
%define parse.error verbose

%%

// A.1.2 High-level Definitions

Program : ExtDefList                            { $$ = NEW_SMTC(1,@$,"Program",$1); root = $$; }
    ;

ExtDefList : ExtDef ExtDefList                  { $$ = NEW_SMTC(2,@$,"ExtDefList",$2,$1); }
    | /* Empty */                               { $$ = NULL; }
    ;

ExtDef : Specifier ExtDecList SEMI              { $$ = NEW_SMTC(3,@$,"ExtDef",$3,$2,$1); CALLBACK(ExtDef,$$); }
    | Specifier SEMI                            { $$ = NEW_SMTC(2,@$,"ExtDef",$2,$1); }
    | FunDecSig SEMI                            { $$ = NEW_SMTC(2,@$,"ExtDef",$2,$1); }
    | FunDefSig CompSt                          { $$ = NEW_SMTC(2,@$,"ExtDef",$2,$1); CALLBACK(FunDefCompSt,$$); }
    ;

ExtDecList : VarDec                             { $$ = NEW_SMTC(1,@$,"ExtDecList",$1); }
    | VarDec COMMA ExtDecList                   { $$ = NEW_SMTC(3,@$,"ExtDecList",$3,$2,$1); }
    ;

// A.1.3 Specifiers

Specifier : TYPE                                { $$ = NEW_SMTC(1,@$,"Specifier",$1); CALLBACK(SpecTYPE,$$); }
    | StructSpecifier                           { $$ = NEW_SMTC(1,@$,"Specifier",$1); CALLBACK(SpecSTRUC,$$); }
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   { $$ = NEW_SMTC(5,@$,"StructSpecifier",$5,$4,$3,$2,$1); CALLBACK(StructDef,$$); }
    | STRUCT Tag                                { $$ = NEW_SMTC(2,@$,"StructSpecifier",$2,$1); CALLBACK(StructCall,$$); }
    ;

OptTag : ID                                     { $$ = NEW_SMTC(1,@$,"OptTag",$1); CALLBACK(ID,$$); }
    | /* Empty */                               { $$ = NULL; }
    ;

Tag : ID                                        { $$ = NEW_SMTC(1,@$,"Tag",$1); CALLBACK(ID,$$); }
    ;

// A.1.4 Declarators

VarDec : ID                                     { $$ = NEW_SMTC(1,@$,"VarDec",$1); CALLBACK(ID,$$); }
    | VarDec LB INT RB                          { $$ = NEW_SMTC(4,@$,"VarDec",$4,$3,$2,$1); CALLBACK(ArrayDec,$$); }
    ;

FunDecSig : Specifier ID LP VarList RP          { $$ = NEW_SMTC(5,@$,"FunDecSig",$5,$4,$3,$2,$1); CALLBACK(FunDecSig,$$); }
    | Specifier ID LP RP                        { $$ = NEW_SMTC(4,@$,"FunDecSig",$4,$3,$2,$1); }
    ;

FunDefSig : Specifier ID LP VarList RP          { $$ = NEW_SMTC(5,@$,"FunDefSig",$5,$4,$3,$2,$1); CALLBACK(FunDefSig,$$); }
    | Specifier ID LP RP                        { $$ = NEW_SMTC(4,@$,"FunDefSig",$4,$3,$2,$1); CALLBACK(FunDefSig,$$); }
    ;

VarList : ParamDec COMMA VarList                { $$ = NEW_SMTC(3,@$,"VarList",$3,$2,$1); }
    | ParamDec                                  { $$ = NEW_SMTC(1,@$,"VarList",$1); }
    ;

ParamDec : Specifier VarDec                     { $$ = NEW_SMTC(2,@$,"ParamDec",$2,$1); }
    ;

// A.1.5 Statements

CompSt : LC CompStDefList StmtList RC          { $$ = NEW_SMTC(4,@$,"CompSt",$4,$3,$2,$1); }
    | LC CompStDefList error RC                { $$ = NEW_SMTC(4,@$,"CompSt",$4,ERR_REP(@3,"}"),$2,$1); }
    ;

StmtList : Stmt StmtList                        { $$ = NEW_SMTC(2,@$,"StmtList",$2,$1); }
    | /* Empty */                               { $$ = NULL; }
    ;

Stmt : Exp SEMI                                 { $$ = NEW_SMTC(2,@$,"Stmt",$2,$1); }
    | error SEMI                                { $$ = NEW_SMTC(2,@$,"Stmt",$2,ERR_REP(@1,";")); }
    | CompSt                                    { $$ = NEW_SMTC(1,@$,"Stmt",$1); }
    | RETURN Exp SEMI                           { $$ = NEW_SMTC(3,@$,"Stmt",$3,$2,$1); }
    | RETURN error SEMI                         { $$ = NEW_SMTC(3,@$,"Stmt",$3,ERR_REP(@2,";"),$1); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   { $$ = NEW_SMTC(5,@$,"Stmt",$5,$4,$3,$2,$1); }
    | IF LP error RP Stmt %prec LOWER_THAN_ELSE { $$ = NEW_SMTC(5,@$,"Stmt",$5,$4,ERR_REP(@3,")"),$2,$1); }
    | IF LP Exp RP Stmt ELSE Stmt               { $$ = NEW_SMTC(7,@$,"Stmt",$7,$6,$5,$4,$3,$2,$1); }
    | IF LP error RP Stmt ELSE Stmt             { $$ = NEW_SMTC(7,@$,"Stmt",$7,$6,$5,$4,ERR_REP(@3,")"),$2,$1); }
    | WHILE LP Exp RP Stmt                      { $$ = NEW_SMTC(5,@$,"Stmt",$5,$4,$3,$2,$1); }
    | WHILE LP error RP Stmt                    { $$ = NEW_SMTC(5,@$,"Stmt",$5,$4,ERR_REP(@3,")"),$2,$1); }
    ;

// A.1.6 Local Definitions

CompStDefList : CompStDef CompStDefList         { $$ = NEW_SMTC(2,@$,"CompStDefList",$2,$1); }
    | /* Empty */                               { $$ = NULL; }
    ;

CompStDef : Specifier CompStDecList SEMI        { $$ = NEW_SMTC(3,@$,"CompStDef",$3,$2,$1); CALLBACK(CompStDef,$$); }
    | Specifier error SEMI                      { $$ = NEW_SMTC(3,@$,"CompStDef",$3,ERR_REP(@2,";"),$1); }
    ;

CompStDecList : CompStDec                       { $$ = NEW_SMTC(1,@$,"CompStDecList",$1); }
    | CompStDec COMMA CompStDecList             { $$ = NEW_SMTC(3,@$,"CompStDecList",$3,$2,$1); }
    ;

CompStDec : VarDec                              { $$ = NEW_SMTC(1,@$,"CompStDec",$1); }
    | VarDec ASSIGNOP Exp                       { $$ = NEW_SMTC(3,@$,"CompStDec",$3,$2,$1); }
    ;

DefList : Def DefList                           { $$ = NEW_SMTC(2,@$,"DefList",$2,$1); }
    | /* Empty */                               { $$ = NULL; }
    ;

Def : Specifier DecList SEMI                    { $$ = NEW_SMTC(3,@$,"Def",$3,$2,$1); }
    | Specifier error SEMI                      { $$ = NEW_SMTC(3,@$,"Def",$3,ERR_REP(@2,";"),$1); }
    ;

DecList : Dec                                   { $$ = NEW_SMTC(1,@$,"DecList",$1); }
    | Dec COMMA DecList                         { $$ = NEW_SMTC(3,@$,"DecList",$3,$2,$1); }
    ;

Dec : VarDec                                    { $$ = NEW_SMTC(1,@$,"Dec",$1); }
    | VarDec ASSIGNOP Exp                       { $$ = NEW_SMTC(3,@$,"Dec",$3,$2,$1); }
    ;

// A.1.7 Expressions

Exp : Exp ASSIGNOP Exp                          { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp AND Exp                               { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp OR Exp                                { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp RELOP Exp                             { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp PLUS Exp                              { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp MINUS Exp                             { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp STAR Exp                              { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp DIV Exp                               { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(2OP,$$); }
    | Exp DOT ID                                { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); CALLBACK(DOT,$$); }
    | LP Exp RP                                 { $$ = NEW_SMTC(3,@$,"Exp",$3,$2,$1); }
    | LP error RP                               { $$ = NEW_SMTC(3,@$,"Exp",$3,ERR_REP(@2,")"),$1); }
    | MINUS Exp                                 { $$ = NEW_SMTC(2,@$,"Exp",$2,$1); }
    | NOT Exp                                   { $$ = NEW_SMTC(2,@$,"Exp",$2,$1); }
    | INT                                       { $$ = NEW_SMTC(1,@$,"Exp",$1); CALLBACK(INT,$$); }
    | FLOAT                                     { $$ = NEW_SMTC(1,@$,"Exp",$1); CALLBACK(FLOAT,$$); }
    | ID                                        { $$ = NEW_SMTC(1,@$,"Exp",$1); CALLBACK(ExpID,$$); }
    | ID LP Args RP                             { $$ = NEW_SMTC(4,@$,"FunCall",$4,$3,$2,$1); CALLBACK(FunCall,$$); }
    | ID LP RP                                  { $$ = NEW_SMTC(3,@$,"FunCall",$3,$2,$1); CALLBACK(FunCall,$$); }
    | Exp LB Exp RB                             { $$ = NEW_SMTC(4,@$,"Exp",$4,$3,$2,$1); CALLBACK(ArrayAccess,$$); }
    | Exp LB error RB                           { $$ = NEW_SMTC(4,@$,"Exp",$4,ERR_REP(@3,"]"),$2,$1); }
    ;

Args : Exp COMMA Args                           { $$ = NEW_SMTC(3,@$,"Args",$3,$2,$1); }
    | Exp                                       { $$ = NEW_SMTC(1,@$,"Args",$1); }
    ;

%%
