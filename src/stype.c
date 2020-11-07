#include "stype.h"

#include <stdio.h>
#include <stdlib.h>

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

stype* stype_new_int() {
  stype* r = NEW(stype);

  r->kind = T_BASIC;
  r->basic = B_INT;

  return r;
}

stype* stype_new_float() {
  stype* r = NEW(stype);

  r->kind = T_BASIC;
  r->basic = B_FLOAT;

  return r;
}

stype* stype_new_array(stype* elem, int size) {
  stype* r = NEW(stype);

  r->kind = T_ARRAY;
  r->array.elem = elem;
  r->array.size = size;

  return r;
}

stype* type_new_struc(field* fl) {
  stype* r = NEW(stype);

  r->kind = T_STRUC;
  r->struc = fl;

  return r;
}

unsigned stype_is_equal(stype* a, stype* b) {
  if (a == b) return 1;
  if (a == NULL || b == NULL) return 0;
  if (a->kind != b->kind) return 0;
  if (a->kind == T_BASIC)
    return a->basic == b->basic;
  else if (a->kind == T_ARRAY)
    return stype_is_equal(a->array.elem, b->array.elem) &&
           a->array.size == a->array.size;
  else if (a->kind == T_STRUC)
    return a->struc->name == b->struc->name;
  // only determine struc is equal by name of 1st field now

  return 0;
}

field* field_new(char* name, stype* type) {
  field* r = NEW(field);
  r->name = name;
  r->type = type;
  r->next = NULL;

  return r;
}

field* field_append(field* fl, field* elem) {
  field* tail = fl;
  while (tail != NULL) {
    if (tail->next == NULL) return tail->next = elem, fl;
    tail = tail->next;
  }
  return fl;
};

void field_free(field* f) {
  if (f == NULL) return;
  free(f->name);
  stype_free(f->type);
  free(f);
}

void fl_free(field* fl) {
  if (fl == NULL) return;
  field* tmp = fl;
  while (fl = tmp != NULL) {
    tmp = tmp->next;
    field_free(fl);
  }
}

  // ===============  Macro Undef  ===============

#undef NEW
#undef COPY