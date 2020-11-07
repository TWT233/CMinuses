#include "stype.h"

#include <stdio.h>

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

stype* type_new_int() {
  stype* r = NEW(stype);

  r->kind = T_BASIC;
  r->basic = B_INT;

  return r;
}

stype* type_new_float() {
  stype* r = NEW(stype);

  r->kind = T_BASIC;
  r->basic = B_FLOAT;

  return r;
}

stype* type_new_array(stype* elem, int size) {
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

  // ===============  Macro Undef  ===============

#undef NEW
#undef COPY