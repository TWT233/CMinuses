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

stype* stype_new_int();

stype* stype_new_float();

stype* stype_new_array(stype* elem, int size);

stype* type_new_struc(field* fl);

unsigned stype_is_equal(stype* a, stype* b);

field* field_new(char* name, stype* type);

field* field_append(field* fl, field* elem);

void field_free(field* f);

void fl_free(field* fl);

// ===============  Macro Undef  ===============

#endif  // __MINUSES_STYPE_H__