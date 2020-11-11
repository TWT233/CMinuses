#ifndef __MINUSES_STYPE_H__
#define __MINUSES_STYPE_H__

#include "syntax.tab.h"

// ===============  Macros  ===============

// ===============  Types  ===============

// symbol type
typedef struct stype stype;
// field list
typedef struct field field;

struct stype {
  enum { T_BASIC, T_ARRAY, T_STRUC, T_FUNCT, T_STRUCTDEF } kind;
  union {
    int basic;
    struct {
      stype* elem;
      int size;
    } array;
    field* struc;
    field* funct;
  };
};

struct field {
  char* name;
  stype* type;
  field* next;
};

// ===============  Func Decs  ===============

stype* stype_int();

stype* stype_float();

stype* stype_array(stype* elem, int size);

stype* stype_struc(field* fl);

stype* stype_strucdef(field* fl);

stype* stype_funct(field* fl);

unsigned stype_is_equal(stype* a, stype* b);

void stype_free(stype* s);

field* field_new(char* name, stype* type);

field* fl_append(field* fl, field* elem);

void field_free(field* f);

void fl_free(field* fl);

// ===============  Macro Undef  ===============

#endif  // __MINUSES_STYPE_H__