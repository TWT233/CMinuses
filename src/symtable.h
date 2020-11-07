#ifndef __MINUSES_HTABLE_H__
#define __MINUSES_HTABLE_H__

#include "stype.h"

// ===============  Macros  ===============

// ===============  Types  ===============

typedef struct sym_list sym_list;
typedef struct sym sym;
typedef struct sym_table sym_table;

// linked symbol entries
struct sym_list {
  sym* ptr;
  sym_list* next;
};

// symbol
struct sym {
  char* name;
  stype* type;
};

// symbol table
struct sym_table {
  sym_list** table;
  size_t len;
};

// ===============  Func Decs  ===============

// ===============  Macro Undef  ===============

#endif  //__MINUSES_HTABLE_H__