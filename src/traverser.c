#include "traverser.h"

#include <stdio.h>

// ===============  Macros  ===============

#define ERR(n) printf("Error type %d at Line %d: \n", n, t->d->pos->first_line)
#define PERR(n, s) \
  printf("Error type %d at Line %d: %s\n", n, t->d->pos->first_line, s)

#define SPEC_STYPE(t) \
  (t->c->ptr->d->val_str[0] == 'i' ? stype_new_int() : stype_new_float())

#define INFO(s) printf("%s\n", s)

// ===============  Var Defs  ===============

static sym_table* TABLE = NULL;

// ===============  Func Defs  ===============

void init_table() { TABLE = st_new(); }

sym_table* get_table() {
  if (TABLE == NULL) init_table();
  return TABLE;
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
  return sym_new(name, stype_new_funct(fl), NULL);
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

// ===============  Macro Undef  ===============

#undef ERR
#undef PERR
#undef SPEC_STYPE
#undef INFO