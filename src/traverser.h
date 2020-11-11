#ifndef __MINUSES_TRAVERSER_H__
#define __MINUSES_TRAVERSER_H__

#include "gtree.h"
#include "symtable.h"

// ===============  Macros  ===============

// ===============  Types  ===============

// ===============  Func Decs  ===============

void init_table();

sym_table* get_table();

void on_ID(gtree* t);

void on_ExpID(gtree* t);

void on_INT(gtree* t);

void on_FLOAT(gtree* t);

void on_SpecTYPE(gtree* t);

void on_SpecSTRUC(gtree* t);

void on_2OP(gtree* t);

void on_DOT(gtree* t);

void on_StructDef(gtree* t);

void on_FunDef(gtree* t);

void on_FunDec(gtree* t);

void on_FunCall(gtree* t);

// ===============  Macro Undef  ===============

#endif
