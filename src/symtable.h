#ifndef __MINUSES_HTABLE_H__
#define __MINUSES_HTABLE_H__

#include <stdlib.h>

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

void sl_free(sym_list* sl);

unsigned st_hash(char* str);

sym_table* st_new();

int st_insert(sym_table* st, char* name, stype* type);

int st_remove(sym_table* st, char* name);

sym* st_get(sym_table* st, char* name);

// ===============  Macro Undef  ===============

#endif  //__MINUSES_HTABLE_H__