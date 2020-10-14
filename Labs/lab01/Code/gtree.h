#ifndef __MINUSES_GTREE_H__
#define __MINUSES_GTREE_H__

#include <stdlib.h>
#include <string.h>

#include "syntax.tab.h"

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

union GVAL {
  int t_int;
  float t_float;
  double t_double;
  char t_char;
  char* t_str;
};

struct data_pack {
  union GVAL* val;
  YYLTYPE* pos;
  int type;
};

struct child_linked {
  struct gtree* ptr;
  struct child_linked* next;
};

struct gtree {
  struct data_pack* d;
  int len;
  struct child_linked* c;
};

typedef union GVAL GVAL;
typedef struct data_pack data_pack;
typedef struct child_linked child_linked;
typedef struct gtree gtree;

data_pack* new_d(GVAL* val, YYLTYPE* pos, int type) {
  data_pack* ret = NEW(data_pack);
  ret->val = (val == NULL) ? NULL : COPY(GVAL, val);
  ret->pos = (pos == NULL) ? NULL : COPY(YYLTYPE, pos);
}

gtree* fill_d(gtree* t, data_pack* d) {
  if (t == NULL) return NULL;
  if (t->d == NULL) t->d = NEW(data_pack);
  if (d == NULL)
    t->d = NULL;
  else
    memcpy(t->d, d, sizeof(data_pack));
  return t;
}

gtree* free_gtree_d(gtree* t) {
  if (t == NULL || t->d == NULL) return t;
  free(t->d->val);
  free(t->d->pos);
  free(t->d);
  t->d = NULL;
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
  (t->len == 0) ? t->c : pos_c(t, t->len - 1)->next = new_c;
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

gtree* remove_c(gtree* t, size_t pos) {
  if (t == NULL || pos >= t->len) return t;
  child_linked* rmd = pos_c(t, pos);
  (pos == 0) ? t->c : pos_c(t, pos - 1)->next = rmd->next;
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

gtree* new_gtree(data_pack* d, int len) {
  gtree* ret = NEW(gtree);

  fill_d(ret, d);
  for (size_t i = 0; i < len; i++) push_c(ret, NULL);

  return ret;
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

gtree* foreach_child(gtree* t, gtree* f(gtree*)) {
  if (t == NULL) return NULL;
  child_linked* tmp = t->c;
  while (tmp != NULL) f(tmp->ptr);
  return t;
}

#undef NEW
#undef COPY

#endif  //__MINUSES_GTREE_H__