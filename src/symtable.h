#ifndef __MINUSES_HTABLE_H__
#define __MINUSES_HTABLE_H__

#include "stype.h"

// ===============  Macros  ===============

#define ST_SIZE 0x3ff

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

sym* sym_new(char* name, stype* type);

void sym_free(sym* ptr);

sym_list* sl_new(sym* ptr);

sym_list* sl_foreach(sym_list* sl, sym_list* f(sym_list*));

unsigned st_hash(char* str);

// ===============  Macro Undef  ===============

#endif  //__MINUSES_HTABLE_H__