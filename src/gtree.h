#ifndef __MINUSES_GTREE_H__
#define __MINUSES_GTREE_H__

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "syntax.tab.h"

// ===============  Types  ===============

typedef YYLTYPE GPOS;
typedef struct data_pack data_pack;
typedef struct child_linked child_linked;
typedef struct gtree gtree;

struct data_pack {
  union {
    char* val_str;   // token raw str
    int val_int;     // token stands for int
    double val_flt;  // token stands for float
  };
  GPOS* pos;  // token position
  int tn;     // token type number
  char* ts;   // token type name str
};

struct child_linked {
  gtree* ptr;
  child_linked* next;
};

struct gtree {
  data_pack* d;
  size_t len;
  child_linked* c;
};

// ===============  Func Defs  ===============

// new a data pack
data_pack* d_new(char* val_str, GPOS* pos, int tn, char* ts);
data_pack* d_new_int(int val_int, GPOS* pos);
data_pack* d_new_flt(double val_flt, GPOS* pos);

// new a GPOS with val
GPOS* gpos_new(YYLTYPE* val);

// new a gtree
gtree* t_new(data_pack* d);

// free `t`
gtree* t_free(gtree* t);

// print `t`
gtree* t_print(gtree* t);

// get `pos`th child in `t`
gtree* t_c_get(gtree* t, size_t pos);

// get the child_linked of `t`'s `pos`th child
child_linked* t_c_pos(gtree* t, size_t pos);

// push(to head) a child to gtree
gtree* t_c_push(gtree* t, gtree* ptr);

// batch push `n` gtrees to `t->c`
gtree* t_c_pushs(gtree* t, int n, ...);

// append(to tail) a child to gtree
gtree* t_c_append(gtree* t, gtree* ptr);

// batch append `n` gtrees to `t->c`
gtree* t_c_appends(gtree* t, int n, ...);

// remove the child at `pos` in `t`
gtree* t_c_remove(gtree* t, size_t pos);

// remove `n` children from `pos` in `t`
gtree* t_c_removes(gtree* t, size_t pos, int n);

// for each child on `t` exec f()
gtree* t_c_foreach(gtree* t, gtree* f(gtree*));

// refill `t->d` with data_pack `*d`
gtree* t_d_fill(gtree* t, data_pack* d);

// free `t->d`
gtree* t_d_free(gtree* t);

#endif  //__MINUSES_GTREE_H__