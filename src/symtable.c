#include "symtable.h"

#include <stdio.h>
#include <string.h>

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

sym* sym_new(char* name, stype* type) {
  sym* r = NEW(sym);
  r->name = name;
  r->type = type;
  return r;
}

void sym_free(sym* ptr) {
  if (ptr == NULL) return;
  free(ptr->name);
  free(ptr->type);
}

sym_list* sl_new(sym* ptr) {
  sym_list* r = NEW(sym_list);
  r->next = NULL;
  r->ptr = ptr;
  return r;
}

