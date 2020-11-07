#ifndef __MINUSES_STYPE_H__
#define __MINUSES_STYPE_H__

// ===============  Macros  ===============

// ===============  Types  ===============

// symbol type
typedef struct stype stype;
// field list
typedef struct field field;

struct stype {
  enum { T_BASIC, T_ARRAY, T_STRUC } kind;
  union {
    enum { B_INT, B_FLOAT } basic;
    struct {
      stype* elem;
      int size;
    } array;
    field* struc;
  };
};

struct field {
  char* name;
  stype* type;
  field* next;
};

// ===============  Func Decs  ===============

stype* type_new_int();

stype* type_new_float();

stype* type_new_array(stype* elem, int size);

// ===============  Macro Undef  ===============
#endif  // __MINUSES_STYPE_H__