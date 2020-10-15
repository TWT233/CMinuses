#ifndef __MINUSES_GTREE_H__
#define __MINUSES_GTREE_H__

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "syntax.tab.h"

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

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
  int len;
  child_linked* c;
};

// ===============  Func Defs  ===============

// new a gtree
gtree* new_gtree(data_pack* d);

// new a data pack
data_pack* new_d(char* val_str, GPOS* pos, int tn, char* ts);
data_pack* new_d_int(int val_int, GPOS* pos);
data_pack* new_d_flt(double val_flt, GPOS* pos);

// push(from head) a child to gtree
gtree* push_c(gtree* t, gtree* ptr);

// append(to tail) a child to gtree
gtree* append_c(gtree* t, gtree* ptr);

// remove the child at `pos` in `t`
gtree* remove_c(gtree* t, size_t pos);

// remove `n` children from `pos` in `t`
gtree* erase_c(gtree* t, int pos, int n);

// fill `t->d` with data_pack `*d`
gtree* fill_d(gtree* t, data_pack* d);

// for each child on `t` exec f()
gtree* foreach_child(gtree* t, gtree* f(gtree*));

// free `t->d`
gtree* free_gtree_d(gtree* t);

// free `t`
gtree* free_gtree(gtree* t);

// get `pos`th child in `t`
gtree* get_c(gtree* t, int pos);

// batch append `n` gtrees to `src` as children
gtree* merge_gtree_append(gtree* src, int n, ...);

// batch push `n` gtrees to `src` as children
gtree* merge_gtree_push(gtree* src, int n, ...);

// new a GPOS with val
GPOS* new_gpos(YYLTYPE* val);

// get the child_linked of `t`'s `pos`th child
child_linked* pos_c(gtree* t, int pos);

#ifndef __MINUSES_GTREE_H_IMPLEMENTS__
#define __MINUSES_GTREE_H_IMPLEMENTS__

// ===============  Functions  ===============

GPOS* new_gpos(YYLTYPE* val) {
  return ((GPOS*)memcpy(malloc(sizeof(GPOS)), val, sizeof(GPOS)));
}

data_pack* new_d(char* val_str, GPOS* pos, int tn, char* ts) {
  data_pack* ret = NEW(data_pack);
  ret->val_str = val_str;
  ret->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  ret->tn = tn;
  ret->ts = ts;
  return ret;
}

data_pack* new_d_int(int val_int, GPOS* pos) {
  data_pack* ret = NEW(data_pack);
  ret->val_int = val_int;
  ret->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  ret->tn = INT;
  ret->ts = "INT";
  return ret;
}

data_pack* new_d_flt(double val_flt, GPOS* pos) {
  data_pack* ret = NEW(data_pack);
  ret->val_flt = val_flt;
  ret->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  ret->tn = FLOAT;
  ret->ts = "FLOAT";
  return ret;
}

gtree* free_gtree_d(gtree* t) {
  if (t == NULL || t->d == NULL) return t;
  if (t->d->tn != INT && t->d->tn != FLOAT) free(t->d->val_str);
  free(t->d->pos);
  free(t->d);
  t->d = NULL;
  return t;
}

gtree* fill_d(gtree* t, data_pack* d) {
  if (t == NULL) return NULL;
  if (t->d != NULL) free_gtree_d(t);
  t->d = (d == NULL) ? NULL : COPY(data_pack, d);
  return t;
}

inline child_linked* pos_c(gtree* t, int pos) {
  if (t == NULL || pos >= t->len) return NULL;
  child_linked* ret = t->c;
  for (size_t i = 0; i < pos; i++) ret = ret->next;
  return ret;
}

gtree* get_c(gtree* t, int pos) {
  child_linked* c = pos_c(t, pos);
  return (c == NULL) ? NULL : c->ptr;
}

gtree* append_c(gtree* t, gtree* ptr) {
  child_linked* new_c = NEW(child_linked);

  new_c->ptr = ptr;
  new_c->next = NULL;
  if (t->len == 0)
    t->c = new_c;
  else
    pos_c(t, t->len - 1)->next = new_c;
  ++t->len;

  return t;
}

gtree* push_c(gtree* t, gtree* ptr) {
  if (t == NULL) return NULL;

  child_linked* new_c = NEW(child_linked);

  new_c->ptr = ptr;
  new_c->next = t->c;
  t->c = new_c;
  ++t->len;

  return t;
}

gtree* new_gtree(data_pack* d) {
  gtree* ret = NEW(gtree);

  fill_d(ret, d);
  ret->len = 0;
  ret->c = NULL;

  return ret;
}

gtree* foreach_child(gtree* t, gtree* f(gtree*)) {
  if (t == NULL) return NULL;
  child_linked* tmp = t->c;
  while (tmp != NULL) {
    f(tmp->ptr);
    tmp = tmp->next;
  }
  return t;
}

gtree* free_gtree(gtree* t) {
  if (t == NULL) return NULL;
  free_gtree_d(t);
  foreach_child(t, free_gtree);

  child_linked* rmd = t->c;
  child_linked* next = (rmd == NULL) ? NULL : rmd->next;
  while (next != NULL) {
    free(rmd);
    rmd = next;
    next = rmd->next;
  }
  free(rmd);
  free(t);
  return NULL;
}

gtree* merge_gtree_push(gtree* src, int n, ...) {
  va_list vl;
  va_start(vl, n);
  for (size_t i = 0; i < n; i++) push_c(src, va_arg(vl, gtree*));
  return src;
}

gtree* merge_gtree_append(gtree* src, int n, ...) {
  va_list vl;
  va_start(vl, n);
  for (size_t i = 0; i < n; i++) append_c(src, va_arg(vl, gtree*));
  return src;
}

gtree* remove_c(gtree* t, size_t pos) {
  if (t == NULL || pos >= t->len) return t;
  child_linked* rmd = pos_c(t, pos);
  if (pos == 0)
    t->c = rmd->next;
  else
    pos_c(t, pos - 1)->next = rmd->next;
  --t->len;
  free_gtree(rmd->ptr);
  free(rmd);
  return t;
}

gtree* erase_c(gtree* t, int pos, int n) {
  if (t == NULL || pos >= t->len) return t;
  for (size_t i = 0; i < n && pos < t->len; i++) remove_c(t, pos);
  return t;
}

#endif

// ===============  Macro Undef  ===============

#undef NEW
#undef COPY

#endif  //__MINUSES_GTREE_H__