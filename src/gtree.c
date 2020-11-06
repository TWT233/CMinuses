#include "gtree.h"

#include <stdio.h>

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

GPOS* gpos_new(YYLTYPE* val) {
  return ((GPOS*)memcpy(malloc(sizeof(GPOS)), val, sizeof(GPOS)));
}

d_pack* d_new(char* val_str, GPOS* pos, int tn, char* ts) {
  d_pack* r = NEW(d_pack);
  r->val_str = val_str;
  r->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  r->tn = tn;
  r->ts = ts;
  return r;
}

d_pack* d_new_int(int val_int, GPOS* pos) {
  d_pack* r = NEW(d_pack);
  r->val_int = val_int;
  r->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  r->tn = INT;
  r->ts = "INT";
  return r;
}

d_pack* d_new_flt(double val_flt, GPOS* pos) {
  d_pack* r = NEW(d_pack);
  r->val_flt = val_flt;
  r->pos = (pos == NULL) ? NULL : COPY(GPOS, pos);
  r->tn = FLOAT;
  r->ts = "FLOAT";
  return r;
}

gtree* t_d_free(gtree* t) {
  if (t == NULL || t->d == NULL) return t;
  if (t->d->tn != INT && t->d->tn != FLOAT) free(t->d->val_str);
  free(t->d->pos);
  free(t->d);
  t->d = NULL;
  return t;
}

gtree* t_d_fill(gtree* t, d_pack* d) {
  if (t == NULL) return NULL;
  if (t->d != NULL) t_d_free(t);
  t->d = (d == NULL) ? NULL : COPY(d_pack, d);
  return t;
}

inline c_list* t_c_pos(gtree* t, size_t pos) {
  if (t == NULL || pos >= t->len) return NULL;
  c_list* r = t->c;
  for (size_t i = 0; i < pos; i++) r = r->next;
  return r;
}

gtree* t_c_get(gtree* t, size_t pos) {
  c_list* c = t_c_pos(t, pos);
  return (c == NULL) ? NULL : c->ptr;
}

gtree* t_c_append(gtree* t, gtree* ptr) {
  c_list* new_c = NEW(c_list);

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

  c_list* new_c = NEW(c_list);

  new_c->ptr = ptr;
  new_c->next = t->c;
  t->c = new_c;
  ++t->len;

  return t;
}

gtree* t_new(d_pack* d) {
  gtree* r = NEW(gtree);

  t_d_fill(r, d);
  r->len = 0;
  r->c = NULL;

  return r;
}

gtree* t_c_foreach(gtree* t, gtree* f(gtree*)) {
  if (t == NULL) return NULL;
  c_list* tmp = t->c;
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

  c_list* rmd = t->c;
  c_list* next = (rmd == NULL) ? NULL : rmd->next;
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
  c_list* rmd = t_c_pos(t, pos);
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