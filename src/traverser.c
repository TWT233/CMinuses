#include "traverser.h"

#include <stdio.h>

// ===============  Macros  ===============

#define ERR(cond, n)                                                    \
  do {                                                                  \
    if (cond) {                                                         \
      printf("Error type %d at Line %d: \n", n, t->d->pos->first_line); \
      return;                                                           \
    }                                                                   \
  } while (0)

#define PERR(cond, n, s)                                                     \
  do {                                                                       \
    if (cond) {                                                              \
      printf("Error type %d at Line %d: %s\n", n, t->d->pos->first_line, s); \
      return;                                                                \
    }                                                                        \
  } while (0)

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

// ===============  Helper Func  ===============

static stype* Specifier_stype(gtree* t) {
  if (t->d->tn == TYPE)
    return t->d->val_str[0] == 'i' ? stype_int() : stype_float();
  if (t->d->tn == STRUCT) {
    sym* tmp = st_get(TABLE, t->d->val_str);
    return (tmp == NULL) ? NULL : tmp->type;
  }
}

// wanna a def
static void StructDef_helper(gtree* t, sym* cu_st) {
  char* name;
  stype* type = Specifier_stype(t_c_top(t));
  ERR((type == NULL || type->kind != T_STRUCTDEF), 17);
  gtree* raw;

  for (gtree* tmp = t_c_get(t, 1); tmp->d->ts[3] == 'L'; tmp = t_c_back(tmp)) {
    raw = t_c_top(tmp);
    ERR((raw->len > 1), 15);
    name = t_c_top(raw)->d->val_str;
    field* i = cu_st->type->struc;
    for (; i != NULL && i->next != NULL; i = i->next) {
      ERR((strcmp(name, i->name) == 0), 15);
    }
    if (i == NULL)
      cu_st->type->struc = field_new(name, type);
    else {
      ERR((strcmp(name, i->name) == 0), 15);
      i->next = field_new(name, type);
    }
  }
}

static sym* fundec_2_sym(gtree* t) {
  gtree* f = t_c_get(t, 1);
  char* name = t_c_top(f)->d->val_str;
  field* fl = field_new(NULL, Specifier_stype(t_c_top(t)));

  for (gtree* tmp = t_c_get(f, 2); tmp->d->ts[0] == 'V'; tmp = t_c_back(tmp)) {
    gtree* p = t_c_top(tmp);
    fl_append(fl, field_new(t_c_top(t_c_back(p))->d->val_str,
                            Specifier_stype(t_c_top(p))));
  }
  return sym_new(name, stype_funct(fl), NULL);
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
  ERR((sid == NULL), 1);
  t->d->val_str = id->d->val_str;
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
      ERR((l->d->tn != r->d->tn), 5);
      unsigned int is_lvalue = 0;
      if (l->len == 1 && (!strcmp(t_c_top(l)->d->ts, "ID"))) is_lvalue = 1;
      if (l->len == 4 && t_c_get(l, 1)->d->tn == LB) is_lvalue = 1;
      if (l->len == 3 && t_c_get(l, 1)->d->tn == DOT) is_lvalue = 1;
      ERR((is_lvalue == 0), 6);
      t->d->tn = r->d->tn;
      break;
    }
    case AND:
    case OR:
    case RELOP: {
      ERR((l->d->tn != INT && l->d->tn != FLOAT), 7);
      ERR((r->d->tn != INT && r->d->tn != FLOAT), 7);
      t->d->tn = INT;
      break;
    }
    case PLUS:
    case MINUS:
    case STAR:
    case DIV: {
      ERR((l->d->tn != INT && l->d->tn != FLOAT), 7);
      ERR((r->d->tn != INT && r->d->tn != FLOAT), 7);
      t->d->tn = (l->d->tn == INT && r->d->tn == INT) ? INT : FLOAT;
      break;
    }
    default:
      break;
  }
}

void on_DOT(gtree* t) {
  INFO(__FUNCTION__);
  gtree* id = t_c_top(t);

  ERR((id->d->tn != STRUC), 13);
  sym* sid = st_get(TABLE, id->d->val_str);
}

void on_StructDef(gtree* t) {
  t = t_c_top(t_c_top(t));
  if (t->d->ts[0] != 'S') return;  // not struct def
  INFO(__FUNCTION__);

  sym* current = st_get(TABLE, t_c_top(t)->d->val_str);

  ERR((current != NULL), 16);

  gtree* OptTag = t_c_get(t, 1);
  char* name = (OptTag != NULL) ? OptTag->d->val_str : NULL;
  sym* cu_st = sym_new(name, stype_strucdef(NULL), t);
  st_insert(TABLE, cu_st);

  for (gtree* def_l = t_c_get(t, 3); def_l != NULL && def_l->len == 2;
       def_l = t_c_back(def_l)) {
    StructDef_helper(t_c_top(def_l), cu_st);
  }
}

void on_CompSt(gtree* t) {}

void on_FunDef(gtree* t) {
  INFO(__FUNCTION__);

  sym* dec = fundec_2_sym(t);
  sym* current = st_get(TABLE, dec->name);

  if (current != NULL) {
    ERR((current->raw != NULL), 4);
    ERR((!stype_is_equal(dec->type, current->type)), 19);
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
    ERR((!stype_is_equal(dec->type, current->type)), 19);
  } else {
    st_insert(TABLE, dec);
  }
}

void on_FunCall(gtree* t) {
  INFO(__FUNCTION__);

  char* name = t_c_top(t)->d->val_str;
  sym* current = st_get(TABLE, name);

  if (current == NULL) {
    PERR((current == NULL), 2, "undeclared function");
    return;
  }
  PERR((current->raw == NULL), 2, "undefined function");
  ERR((current->type->kind != T_FUNCT), 11);

  field* p = current->type->funct->next;
  gtree* a = t_c_get(t, 2);
  for (; a->d->ts[0] == 'A' && p != NULL; a = t_c_back(a), p = p->next) {
    gtree* e = t_c_top(a);
    PERR((e->d->tn != p->type->basic), 9, "arg type missmatch");
  }
  PERR((p != NULL || a->d->ts[0] != 'E'), 9, "arg count missmatch");
}

// ===============  Macro Undef  ===============

#undef ERR
#undef PERR
#undef SPEC_STYPE
#undef INFO