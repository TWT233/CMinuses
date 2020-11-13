#include "traverser.h"

#include <stdio.h>

// ===============  Macros  ===============

#define ERR(n, cond)                                                    \
  do {                                                                  \
    if (cond) {                                                         \
      printf("Error type %d at Line %d: \n", n, t->d->pos->first_line); \
      return;                                                           \
    }                                                                   \
  } while (0)

#define OERR(n, cond)                                                   \
  do {                                                                  \
    if (cond)                                                           \
      printf("Error type %d at Line %d: \n", n, t->d->pos->first_line); \
  } while (0)

#define PERR(n, cond, s)                                                     \
  do {                                                                       \
    if (cond) {                                                              \
      printf("Error type %d at Line %d: %s\n", n, t->d->pos->first_line, s); \
      return;                                                                \
    }                                                                        \
  } while (0)

#ifdef DEBUGGING
#define INFO(s) \
  printf("[INFO][%d:%d]%s\n", t->d->pos->first_line, t->d->pos->first_column, s)
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

static stype* extract_stype(gtree* t) {
  sym* s;
  switch (t->d->tn) {
    case INT:
      return stype_int();
    case FLOAT:
      return stype_float();
    case STRUC:
      s = st_get(TABLE, t->d->val_str);
      return (s == NULL) ? NULL : s->type;
    default:
      break;
  }
}

static int stype_tn(stype* st) {
  switch (st->kind) {
    case T_BASIC:
      return st->basic;
    case T_ARRAY:
      return ARRAY;
    case T_STRUC:
      return STRUC;
    case T_FUNCT:
      return FUNCT;
    case T_STRUCTDEF:
      return STRUC;
    default:
      printf("ERR Type\n");
      break;
  }
}

// wanna a def
static void StructDef_helper(gtree* t, sym* cu_st) {
  char* name;
  stype* type = Specifier_stype(t_c_top(t));
  ERR(17, (type == NULL));
  gtree* raw;

  // tmp: DecList
  for (gtree* tmp = t_c_get(t, 1); tmp->d->ts[3] == 'L'; tmp = t_c_back(tmp)) {
    raw = t_c_top(tmp);  // raw: Dec
    ERR(15, (raw->len > 1));
    name = t_c_top(raw)->d->val_str;
    field* i = cu_st->type->struc;
    for (; i != NULL && i->next != NULL; i = i->next) {
      ERR(15, (strcmp(name, i->name) == 0));
    }

    if (t_c_top(raw)->d->tn == ARRAY) {  // t_c_top(raw): vardec
      stype* j = t_c_top(raw)->d->tp;
      for (; j->array.elem != NULL; j = j->array.elem)
        ;
      j->array.elem = type;
      type = t_c_top(raw)->d->tp;
    }

    if (i == NULL)
      cu_st->type->struc = field_new(name, type);
    else {
      ERR(15, (strcmp(name, i->name) == 0));
      i->next = field_new(name, type);
    }
  }
}

// wanna a def
static void CompStDefField_helper(gtree* t) {
  char* name;
  stype* type = Specifier_stype(t_c_top(t));
  ERR(17, (type == NULL));
  gtree* raw;

  for (gtree* dec_l = t_c_get(t, 1); dec_l->d->ts[3] == 'L';
       dec_l = t_c_back(dec_l)) {
    raw = t_c_top(dec_l);
    if (raw->len == 3) {
      ERR(5, !stype_is_equal(type, extract_stype(t_c_back(raw))));
    }
    name = t_c_top(raw)->d->val_str;
    ERR(3, st_get(TABLE, name) != NULL);
    if (type->kind == T_STRUCTDEF) {
      st_insert(TABLE, sym_new(name, stype_struc(type->struc), raw));
    } else {
      st_insert(TABLE, sym_new(name, type, raw));
    }
  }
}

static sym* fundec_2_sym(gtree* t) {
  char* name = t_c_get(t, 1)->d->val_str;
  field* fl = field_new(NULL, Specifier_stype(t_c_top(t)));

  for (gtree* vl = t_c_get(t, 3); vl->d->ts[0] == 'V'; vl = t_c_back(vl)) {
    gtree* p = t_c_top(vl);
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

void on_ArrayDec(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = ARRAY;
  gtree* varDec = t_c_top(t);
  if (varDec->d->tn == ARRAY) {
    t->d->tp = stype_array(varDec->d->tp, t_c_get(t, 2)->d->val_int);
  } else {
    t->d->tp = stype_array(NULL, t_c_get(t, 2)->d->val_int);
  }
  t->d->val_str = t->c->ptr->d->val_str;
}

void on_ExpID(gtree* t) {
  INFO(__FUNCTION__);
  gtree* id = t_c_top(t);
  sym* sid = st_get(TABLE, id->d->val_str);
  ERR(1, (sid == NULL));
  t->d->val_str = id->d->val_str;
  t->d->tp = sid->type;
  t->d->tn = stype_tn(t->d->tp);
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
      ERR(5, (l->d->tn != r->d->tn));
      unsigned int is_lvalue = 0;
      if (l->len == 1 && (!strcmp(t_c_top(l)->d->ts, "ID"))) is_lvalue = 1;
      if (l->len == 4 && t_c_get(l, 1)->d->tn == LB) is_lvalue = 1;
      if (l->len == 3 && t_c_get(l, 1)->d->tn == DOT) is_lvalue = 1;
      ERR(6, (is_lvalue == 0));
      t->d->tn = r->d->tn;
      break;
    }
    case AND:
    case OR:
    case RELOP: {
      ERR(7, (l->d->tn != INT && l->d->tn != FLOAT) || (l->d->tn != r->d->tn));
      t->d->tn = INT;
      break;
    }
    case PLUS:
    case MINUS:
    case STAR:
    case DIV: {
      ERR(7, (l->d->tn != INT && l->d->tn != FLOAT) || (l->d->tn != r->d->tn));
      t->d->tn = (l->d->tn == INT && r->d->tn == INT) ? INT : FLOAT;
      break;
    }
    default:
      break;
  }
}

void on_DOT(gtree* t) {
  INFO(__FUNCTION__);
  gtree* exp = t_c_top(t);

  ERR(13, (exp->d->tp == NULL || exp->d->tp->kind != T_STRUC));
  t->d->val_str = exp->d->val_str;

  gtree* f = t_c_get(t, 2);
  unsigned int is_defined = 0;
  for (field* tmp = exp->d->tp->struc; tmp != NULL; tmp = tmp->next) {
    if (strcmp(tmp->name, f->d->val_str) == 0) {
      is_defined = 1;
      t->d->tp = tmp->type;
      t->d->tn = stype_tn(t->d->tp);
    }
  }
  ERR(14, (is_defined == 0));
}

void on_StructDef(gtree* t) {
  INFO(__FUNCTION__);

  gtree* OptTag = t_c_get(t, 1);
  char* name = (OptTag != NULL) ? OptTag->d->val_str : NULL;
  sym* current = st_get(TABLE, name);
  ERR(16, (current != NULL));
  sym* cu_st = sym_new(name, stype_strucdef(NULL), t);
  st_insert(TABLE, cu_st);

  for (gtree* def_l = t_c_get(t, 3); def_l != NULL && def_l->len == 2;
       def_l = t_c_back(def_l)) {
    StructDef_helper(t_c_top(def_l), cu_st);
  }
}

void on_CompStDef(gtree* t) {
  INFO(__FUNCTION__);
  char* name;
  stype* type = Specifier_stype(t_c_top(t));
  ERR(17, (type == NULL));
  gtree* raw;

  for (gtree* dec_l = t_c_get(t, 1); dec_l->d->ts[9] == 'L';
       dec_l = t_c_back(dec_l)) {
    raw = t_c_top(dec_l);  // raw: Dec
    if (raw->len == 3) {
      ERR(5, !stype_is_equal(type, extract_stype(t_c_back(raw))));
    }
    name = t_c_top(raw)->d->val_str;
    ERR(3, st_get(TABLE, name) != NULL);
    if (type->kind == T_STRUCTDEF) {
      st_insert(TABLE, sym_new(name, stype_struc(type->struc), raw));
    } else {
      if (t_c_top(raw)->d->tn == ARRAY) {
        stype* j = t_c_top(raw)->d->tp;
        for (; j->array.elem != NULL; j = j->array.elem)
          ;
        j->array.elem = type;
        type = t_c_top(raw)->d->tp;
      }
      st_insert(TABLE, sym_new(name, type, raw));
    }
  }
}

void on_ExtDef(gtree* t) {
  INFO(__FUNCTION__);
  char* name;
  stype* type = Specifier_stype(t_c_top(t));
  ERR(17, (type == NULL));
  gtree* raw;

  for (gtree* dec_l = t_c_get(t, 1); dec_l->d->ts[6] == 'L';
       dec_l = t_c_back(dec_l)) {
    raw = t_c_top(dec_l);
    if (raw->len == 3) {
      ERR(5, !stype_is_equal(type, extract_stype(t_c_back(raw))));
    }
    name = t_c_top(raw)->d->val_str;
    ERR(3, st_get(TABLE, name) != NULL);
    if (type->kind == T_STRUCTDEF) {
      st_insert(TABLE, sym_new(name, stype_struc(type->struc), raw));
    } else {
      st_insert(TABLE, sym_new(name, type, raw));
    }
  }
}

void on_ArrayAccess(gtree* t) {
  INFO(__FUNCTION__);
  ERR(10, t_c_top(t)->d->tn != ARRAY);
  ERR(12, (t_c_get(t, 2)->d->tn != INT));
  int pointer = t_c_get(t, 2)->d->val_int;
  t->d->tp = t_c_top(t)->d->tp->array.elem;
  t->d->tn = stype_tn(t->d->tp);
}

void on_FunDefCompSt(gtree* t) {
  INFO(__FUNCTION__);
  gtree* CompSt = t_c_back(t);
  stype* dec = Specifier_stype(t_c_top(t_c_top(t)));

  for (gtree* StmtList = t_c_get(CompSt, 2);
       StmtList != NULL && StmtList->len == 2; StmtList = t_c_back(StmtList)) {
    gtree* Stmt = t_c_top(StmtList);
    if (t_c_top(Stmt)->d->tn == RETURN) {
      gtree* exp = t_c_get(Stmt, 1);
      ERR(8, !stype_is_equal(dec, extract_stype(exp)));
    }
  }
}

void on_FunDecSig(gtree* t) {
  INFO(__FUNCTION__);

  sym* dec = fundec_2_sym(t);
  sym* current = st_get(TABLE, dec->name);

  if (current != NULL) {
    ERR(19, (!stype_is_equal(dec->type, current->type)));
  } else {
    st_insert(TABLE, dec);
  }
}

void on_FunDefSig(gtree* t) {
  INFO(__FUNCTION__);

  sym* dec = fundec_2_sym(t);
  sym* current = st_get(TABLE, dec->name);

  if (current != NULL) {
    OERR(4, (current->raw != NULL));
    OERR(19, (!stype_is_equal(dec->type, current->type)));
  } else {
    dec->raw = t;
    st_insert(TABLE, dec);
  }

  gtree* raw = t_c_get(t, 3);
  field* fl = dec->type->funct->next;
  for (; fl != NULL; fl = fl->next) {
    st_insert(TABLE, sym_new(fl->name, fl->type, t_c_top(raw)));
    raw = t_c_back(raw);
  }
}

void on_FunCall(gtree* t) {
  INFO(__FUNCTION__);

  char* name = t_c_top(t)->d->val_str;
  sym* current = st_get(TABLE, name);

  if (current == NULL) {
    PERR(2, (current == NULL), "undeclared function");
    return;
  }
  PERR(2, (current->raw == NULL), "undefined function");
  ERR(11, (current->type->kind != T_FUNCT));

  field* p = current->type->funct->next;
  gtree* a = t_c_get(t, 2);
  for (; a->d->ts[0] == 'A' && p != NULL; a = t_c_back(a), p = p->next) {
    gtree* e = t_c_top(a);
    PERR(9, (e->d->tn != p->type->basic), "arg type missmatch");
  }
  PERR(9, (p != NULL || a->d->ts[0] != 'E'), "arg count missmatch");
  t->d->tp = current->type->funct->type;
  t->d->tn = stype_tn(t->d->tp);
}

// ===============  Macro Undef  ===============

#undef ERR
#undef PERR
#undef SPEC_STYPE
#undef INFO