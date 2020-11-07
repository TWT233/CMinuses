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

sym_list* sl_foreach(sym_list* sl, sym_list* f(sym_list*)) {
  if (sl == NULL) return sl;
  sym_list* r = sl;
  for (; r != NULL; r = r->next) f(r);
  return sl;
}

unsigned st_hash(char* str) {
  unsigned r = 0, i;

  for (; *str; ++str) {
    r = (r << 2) + *str;
    if (i = r & ~ST_SIZE) r = (r ^ (i >> 12)) & ST_SIZE;
  }

  return r;
}

sym_table* st_new() {
  sym_table* r = NEW(sym_table);

  r->len = ST_SIZE;
  r->table = (sym_list**)malloc(ST_SIZE * sizeof(sym_list*));
  memset(r->table, 0, ST_SIZE * sizeof(sym_list*));

  return r;
}

int st_insert(sym_table* st, char* name, stype* type) {
  unsigned hash = st_hash(name);
  if (st_get(st, name) != NULL) return 1;
  sym_list* i = sl_new(sym_new(name, type));
  i->next = st->table[hash];
  st->table[hash] = i;
  return 0;
}

int st_remove(sym_table* st, char* name) {
  unsigned hash = st_hash(name);
  sym_list* i = st->table[hash];
  sym_list* rmd = NULL;
  if (strcmp(i->ptr->name, name) == 0) {
    rmd = i;
    st->table[hash] = i->next;
  } else {
    for (; i != NULL && i->next != NULL; i = i->next) {
      if (strcmp(i->next->ptr->name, name) == 0) {
        rmd = i->next;
        i->next = i->next->next;
      }
    }
  }

  if (rmd == NULL) return 1;
  free(rmd->ptr);
  free(rmd);
}

sym* st_get(sym_table* st, char* name) {
  unsigned hash = st_hash(name);
  for (sym_list* i = st->table[hash]; i != NULL; i = i->next) {
    if (strcmp(i->ptr->name, name) == 0) return i->ptr;
  }
  return NULL;
}

