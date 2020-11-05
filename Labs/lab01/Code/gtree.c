#include "gtree.h"

#include <stdio.h>

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

GPOS* gpos_new(YYLTYPE* val) {
  return ((GPOS*)memcpy(malloc(sizeof(GPOS)), val, sizeof(GPOS)));
}

data_pack* d_new(char* val_str, GPOS* pos, int tn, char* ts) {
  data_pack* ret = NEW(data_pack);
  ret->val_str = val_str;
  ret->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  ret->tn = tn;
  ret->ts = ts;
  return ret;
}

data_pack* d_new_int(int val_int, GPOS* pos) {
  data_pack* ret = NEW(data_pack);
  ret->val_int = val_int;
  ret->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  ret->tn = INT;
  ret->ts = "INT";
  return ret;
}

data_pack* d_new_flt(double val_flt, GPOS* pos) {
  data_pack* ret = NEW(data_pack);
  ret->val_flt = val_flt;
  ret->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  ret->tn = FLOAT;
  ret->ts = "FLOAT";
  return ret;
}

gtree* t_d_free(gtree* t) {
  if (t == NULL || t->d == NULL) return t;
  if (t->d->tn != INT && t->d->tn != FLOAT) free(t->d->val_str);
  free(t->d->pos);
  free(t->d);
  t->d = NULL;
  return t;
}

gtree* t_d_fill(gtree* t, data_pack* d) {
  if (t == NULL) return NULL;
  if (t->d != NULL) t_d_free(t);
  t->d = (d == NULL) ? NULL : COPY(data_pack, d);
  return t;
}

inline child_linked* t_c_pos(gtree* t, size_t pos) {
  if (t == NULL || pos >= t->len) return NULL;
  child_linked* ret = t->c;
  for (size_t i = 0; i < pos; i++) ret = ret->next;
  return ret;
}

gtree* t_c_get(gtree* t, size_t pos) {
  child_linked* c = t_c_pos(t, pos);
  return (c == NULL) ? NULL : c->ptr;
}

gtree* t_c_append(gtree* t, gtree* ptr) {
  child_linked* new_c = NEW(child_linked);

  new_c->ptr = ptr;
  new_c->next = NULL;
  if (t->len == 0)
    t->c = new_c;
  else
    t_c_pos(t, t->len - 1)->next = new_c;
  ++t->len;

  return t;
}

gtree* t_c_push(gtree* t, gtree* ptr) {
  if (t == NULL) return NULL;

  child_linked* new_c = NEW(child_linked);

  new_c->ptr = ptr;
  new_c->next = t->c;
  t->c = new_c;
  ++t->len;

  return t;
}

gtree* t_new(data_pack* d) {
  gtree* ret = NEW(gtree);

  t_d_fill(ret, d);
  ret->len = 0;
  ret->c = NULL;

  return ret;
}

gtree* t_c_foreach(gtree* t, gtree* f(gtree*)) {
  if (t == NULL) return NULL;
  child_linked* tmp = t->c;
  while (tmp != NULL) {
    f(tmp->ptr);
    tmp = tmp->next;
  }
  return t;
}

gtree* t_free(gtree* t) {
  if (t == NULL) return NULL;
  t_d_free(t);
  t_c_foreach(t, t_free);

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

gtree* t_c_pushs(gtree* t, int n, ...) {
  va_list vl;
  va_start(vl, n);
  for (size_t i = 0; i < n; i++) t_c_push(t, va_arg(vl, gtree*));
  return t;
}

gtree* t_c_appends(gtree* t, int n, ...) {
  va_list vl;
  va_start(vl, n);
  for (size_t i = 0; i < n; i++) t_c_append(t, va_arg(vl, gtree*));
  return t;
}

gtree* t_c_remove(gtree* t, size_t pos) {
  if (t == NULL || pos >= t->len) return t;
  child_linked* rmd = t_c_pos(t, pos);
  if (pos == 0)
    t->c = rmd->next;
  else
    t_c_pos(t, pos - 1)->next = rmd->next;
  --t->len;
  t_free(rmd->ptr);
  free(rmd);
  return t;
}

gtree* t_c_removes(gtree* t, size_t pos, int n) {
  if (t == NULL || pos >= t->len) return t;
  for (size_t i = 0; i < n && pos < t->len; i++) t_c_remove(t, pos);
  return t;
}

static int counter = 0;

gtree* t_print(gtree* t) {
  if (t == NULL || t->d->tn == -1) return NULL;
  ++counter;

  for (size_t i = 1; i < counter; i++) printf("  ");

  switch (t->d->tn) {
    case -1:
      break;
    case 0:
      printf("%s (%d)\n", t->d->ts, t->d->pos->first_line);
      break;
    case ID:
    case TYPE:
      printf("%s: %s\n", t->d->ts, t->d->val_str);
      break;
    case INT:
      printf("%s: %d\n", t->d->ts, t->d->val_int);
      break;
    case FLOAT:
      printf("%s: %lf\n", t->d->ts, t->d->val_flt);
      break;
    default:
      printf("%s\n", t->d->ts);
      break;
  }

  t_c_foreach(t, t_print);

  --counter;
  return t;
}

// ===============  Macro Undef  ===============

#undef NEW
#undef COPY