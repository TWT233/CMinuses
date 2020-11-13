#include "stype.h"

#include <stdio.h>
#include <stdlib.h>

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

static stype* STYPE_INT = NULL;
static stype* STYPE_FLOAT = NULL;

stype* stype_int() {
  if (STYPE_INT == NULL) {
    STYPE_INT = NEW(stype);
    STYPE_INT->kind = T_BASIC;
    STYPE_INT->basic = INT;
  }
  return STYPE_INT;
}

stype* stype_float() {
  if (STYPE_FLOAT == NULL) {
    STYPE_FLOAT = NEW(stype);

    STYPE_FLOAT->kind = T_BASIC;
    STYPE_FLOAT->basic = FLOAT;
  }
  return STYPE_FLOAT;
}

stype* stype_array(stype* elem, int size) {
  stype* r = NEW(stype);

  r->kind = T_ARRAY;
  r->array.elem = elem;
  r->array.size = size;

  return r;
}

stype* stype_struc(field* fl) {
  stype* r = NEW(stype);

  r->kind = T_STRUC;
  r->struc = fl;

  return r;
}

stype* stype_funct(field* fl) {
  stype* r = NEW(stype);

  r->kind = T_FUNCT;
  r->funct = fl;

  return r;
}

static unsigned is_funct_sig_equal(field* a, field* b) {
  for (; a != NULL && b != NULL; a = a->next, b = b->next) {
    if (a->type->basic != b->type->basic) return 0;
  }
  return a == b;
}

unsigned stype_is_equal(stype* a, stype* b) {
  if (a == b) return 1;
  if (a == NULL || b == NULL) return 0;
  if (a->kind != b->kind) return 0;

  switch (a->kind) {
    case T_BASIC:
      return a->basic == b->basic;
    case T_ARRAY:
      return stype_is_equal(a->array.elem, b->array.elem) &&
             a->array.size == a->array.size;
    case T_STRUC:
      return a->struc->name == b->struc->name;
    case T_FUNCT:
      return is_funct_sig_equal(a->funct, b->funct);
    default:
      break;
  }
  // only determine struc is equal by name of 1st field now

  return 0;
}

void stype_free(stype* s) {
  if (s == NULL) return;
  switch (s->kind) {
    case T_BASIC:
      // keep cache, thus do not free basic types
      return;
    case T_ARRAY:
      stype_free(s->array.elem);
      break;
    case T_STRUC:
      // fl_free(s->struc);
      break;
    case T_FUNCT:
      fl_free(s->funct);
      break;
    default:
      return;
  }
  free(s);
}

field* field_new(char* name, stype* type) {
  field* r = NEW(field);
  r->name = name;
  r->type = type;
  r->next = NULL;

  return r;
}

field* fl_append(field* fl, field* elem) {
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
  while ((fl = tmp) != NULL) {
    tmp = tmp->next;
    field_free(fl);
  }
}

// ===============  Macro Undef  ===============

#undef NEW
#undef COPY