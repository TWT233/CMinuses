#ifndef __MINUSES_TRAVERSER_H__
#define __MINUSES_TRAVERSER_H__

#include "gtree.h"
#include "symtable.h"

// ===============  Macros  ===============

// ===============  Types  ===============

// ===============  Func Decs  ===============

void init_table();

sym_table* get_table();

void on_TagID(gtree* t);

void on_SpecTYPE(gtree* t);

void on_FunDef(gtree* t);

void on_FunDec(gtree* t);

void on_FunCall(gtree* t);

// ===============  Macro Undef  ===============

#endif
