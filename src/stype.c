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

  // ===============  Macro Undef  ===============

#undef NEW
#undef COPY