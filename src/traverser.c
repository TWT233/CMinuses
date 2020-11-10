#include "traverser.h"

#include <stdio.h>

// ===============  Macros  ===============

#define ERR(n) printf("Error type %d at Line %d: \n", n, t->d->pos->first_line)
#define PERR(n, s) \
  printf("Error type %d at Line %d: %s\n", n, t->d->pos->first_line, s)

#define INFO(s) printf("%s\n", s)

// ===============  Var Defs  ===============

static sym_table* TABLE = NULL;

// ===============  Func Defs  ===============

void init_table() { TABLE = st_new(); }

sym_table* get_table() {
  if (TABLE == NULL) init_table();
  return TABLE;
}

void on_TagID(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = t->c->ptr->d->tn;
  t->d->val_str = t->c->ptr->d->val_str;
}

void on_SpecTYPE(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = t->c->ptr->d->tn;
  t->d->val_str = t->c->ptr->d->val_str;
}

void on_SpecSTRUC(gtree* t) {
  INFO(__FUNCTION__);
  gtree* ss = t_c_top(t);
  gtree* tag = t_c_get(ss, 1);
  t->d->tn = ss->c->ptr->d->tn;
  if (tag != NULL) t->d->val_str = tag->d->val_str;
}

static stype* SPEC_STYPE(gtree* t) {
  if (t->d->tn == TYPE)
    return t->d->val_str[0] == 'i' ? stype_int() : stype_float();
  if (t->d->tn == STRUCT) return st_get(TABLE, t->d->val_str)->type;
}

static sym* fundec_2_sym(gtree* t) {
  gtree* f = t_c_get(t, 1);
  char* name = t_c_top(f)->d->val_str;
  field* fl = field_new(NULL, SPEC_STYPE(t_c_top(t)));

  for (gtree* tmp = t_c_get(f, 2); tmp->d->ts[0] == 'V'; tmp = t_c_back(tmp)) {
    gtree* p = t_c_top(tmp);
    fl_append(fl, field_new(t_c_top(t_c_back(p))->d->val_str,
                            SPEC_STYPE(t_c_top(p))));
  }
  return sym_new(name, stype_funct(fl), NULL);
}

void on_FunDef(gtree* t) {
  INFO(__FUNCTION__);

  sym* dec = fundec_2_sym(t);
  sym* current = st_get(TABLE, dec->name);

  if (current != NULL) {
    if (current->raw != NULL) ERR(4);
    if (!stype_is_equal(dec->type, current->type)) ERR(19);
  } else {
    dec->raw = t;
    st_insert(TABLE, dec);
  }
}

void on_FunDec(gtree* t) {
  INFO(__FUNCTION__);

  sym* dec = fundec_2_sym(t);
  sym* current = st_get(TABLE, dec->name);

  if (current != NULL) {
    if (!stype_is_equal(dec->type, current->type)) ERR(19);
  } else {
    st_insert(TABLE, dec);
  }
}

void on_FunCall(gtree* t) {
  INFO(__FUNCTION__);

  char* name = t_c_top(t)->d->val_str;
  sym* current = st_get(TABLE, name);

  if (current->raw == NULL) ERR(2);

  field* p = current->type->funct->next;
  gtree* a = t_c_get(t, 2);
  for (; a->d->ts[0] == 'A' && p != NULL; a = t_c_back(a), p = p->next) {
    gtree* e = t_c_top(a);
    if (e->d->tn != p->type->basic) PERR(9, "arg type missmatch");
  }
  if (p != NULL || a->d->ts[0] != 'E') PERR(9, "arg count missmatch");
}

// ===============  Macro Undef  ===============

#undef ERR
#undef PERR
#undef SPEC_STYPE
#undef INFO