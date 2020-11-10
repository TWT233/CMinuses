#include "traverser.h"

#include <stdio.h>

// ===============  Macros  ===============

#define ERR(n) printf("Error type %d at Line %d: \n", n, t->d->pos->first_line)
#define PERR(n, s) \
  printf("Error type %d at Line %d: %s\n", n, t->d->pos->first_line, s)

#define DEBUGGING

#ifdef DEBUGGING
#define INFO(s) printf("[INFO]%s\n", s)
#else
#define INFO(s)
#endif

// ===============  Var Defs  ===============

static sym_table* TABLE = NULL;

// ===============  Func Defs  ===============

void init_table() { TABLE = st_new(); }

sym_table* get_table() {
  if (TABLE == NULL) init_table();
  return TABLE;
}

// ===============  CALLBACKS  ===============

void on_ID(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = ID;
  t->d->val_str = t->c->ptr->d->val_str;
}

void on_ExpID(gtree* t) {
  INFO(__FUNCTION__);
  gtree* id = t_c_top(t);
  sym* sid = st_get(TABLE, id->d->val_str);
  if (sid == NULL) ERR(1);
  if (sid->type->kind == T_BASIC) {
    t->d->tn = (sid->type->basic == INT) ? INT : FLOAT;
  } else if (sid->type->kind == T_ARRAY) {
    t->d->tn = ARRAY;
  } else if (sid->type->kind == T_STRUC) {
    t->d->tn = STRUC;
  } else if (sid->type->kind == T_FUNCT) {
    t->d->tn = FUNCT;
  }
}

void on_INT(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = INT;
  t->d->val_int = t->c->ptr->d->val_int;
}

void on_FLOAT(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = FLOAT;
  t->d->val_flt = t->c->ptr->d->val_flt;
}

void on_SpecTYPE(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = TYPE;
  t->d->val_str = t->c->ptr->d->val_str;
}

void on_SpecSTRUC(gtree* t) {
  INFO(__FUNCTION__);
  gtree* ss = t_c_top(t);
  gtree* tag = t_c_get(ss, 1);
  t->d->tn = STRUCT;
  if (tag != NULL) t->d->val_str = tag->d->val_str;
}

void on_2OP(gtree* t) {
  INFO(__FUNCTION__);
  gtree* l = t_c_top(t);
  gtree* op = t->c->next->ptr;
  gtree* r = t->c->next->next->ptr;
  switch (op->d->tn) {
    case ASSIGNOP: {
      if (l->d->tn != l->d->tn) ERR(5);
      t->d->tn = r->d->tn;
      if (l->d->tn == INT) {
        t->d->val_int = r->d->val_int;
      } else {
        t->d->val_flt = r->d->val_flt;
      }
      break;
    }
    case AND: {
      if (l->d->tn != INT && l->d->tn != FLOAT) ERR(7);
      if (r->d->tn != INT && r->d->tn != FLOAT) ERR(7);
      t->d->tn = INT;
      t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) &&
                      (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
      break;
    }
    case OR: {
      if (l->d->tn != INT && l->d->tn != FLOAT) ERR(7);
      if (r->d->tn != INT && r->d->tn != FLOAT) ERR(7);
      t->d->tn = INT;
      t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) ||
                      (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
      break;
    }
    case RELOP: {
      if (l->d->tn != INT && l->d->tn != FLOAT) ERR(7);
      if (r->d->tn != INT && r->d->tn != FLOAT) ERR(7);
      switch (op->d->val_str[0]) {
        case '>': {
          if (op->d->val_str[1] == '=') {
            t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) >=
                            (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
          } else {
            t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) >
                            (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
          }
          break;
        }
        case '<': {
          if (op->d->val_str[1] == '=') {
            t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) <=
                            (r->d->tn == INT ? r->d->val_int : r->d->val_flt);

          } else {
            t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) <
                            (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
          }
          break;
        }
        case '=': {
          t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) ==
                          (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
          break;
        }
        case '!': {
          t->d->val_int = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) !=
                          (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
          break;
        }
        default:
          break;
      }
      t->d->tn = INT;
      break;
    }
    case PLUS: {
      if (l->d->tn != INT && l->d->tn != FLOAT) ERR(7);
      if (r->d->tn != INT && r->d->tn != FLOAT) ERR(7);
      if (l->d->tn == INT && r->d->tn == INT) {
        t->d->tn = INT;
        t->d->val_int = l->d->val_int + r->d->val_int;
      } else {
        t->d->tn = FLOAT;
        t->d->val_flt = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) +
                        (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
      }
    }
    case MINUS: {
      if (l->d->tn != INT && l->d->tn != FLOAT) ERR(7);
      if (r->d->tn != INT && r->d->tn != FLOAT) ERR(7);
      if (l->d->tn == INT && r->d->tn == INT) {
        t->d->tn = INT;
        t->d->val_int = l->d->val_int - r->d->val_int;
      } else {
        t->d->tn = FLOAT;
        t->d->val_flt = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) -
                        (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
      }
    }
    case STAR: {
      if (l->d->tn != INT && l->d->tn != FLOAT) ERR(7);
      if (r->d->tn != INT && r->d->tn != FLOAT) ERR(7);
      if (l->d->tn == INT && r->d->tn == INT) {
        t->d->tn = INT;
        t->d->val_int = l->d->val_int * r->d->val_int;
      } else {
        t->d->tn = FLOAT;
        t->d->val_flt = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) *
                        (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
      }
    }
    case DIV: {
      if (l->d->tn != INT && l->d->tn != FLOAT) ERR(7);
      if (r->d->tn != INT && r->d->tn != FLOAT) ERR(7);
      if (l->d->tn == INT && r->d->tn == INT) {
        t->d->tn = INT;
        t->d->val_int = l->d->val_int / r->d->val_int;
      } else {
        t->d->tn = FLOAT;
        t->d->val_flt = (l->d->tn == INT ? l->d->val_int : l->d->val_flt) /
                        (r->d->tn == INT ? r->d->val_int : r->d->val_flt);
      }
    }

    default:
      break;
  }
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

  if (current == NULL) {
    PERR(2, "undeclared function");
    return;
  }
  if (current->raw == NULL) PERR(2, "undefined function");
  if (current->type->kind != T_FUNCT) ERR(11);

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