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
#define WARN(s, ...) printf("[WARN]" s "\n", ##__VA_ARGS__)
#else
#define INFO(s)
#define WARN(s, ...)
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
  if (st == NULL) {
    WARN("NULL stype");
    return 0;
  }
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
      WARN("unknown stype");
      break;
  }
}

// wanna a def
static void StructDef_helper(gtree* t, sym* cu_st) {
  char* name;
  stype* type = t_c_top(t)->d->tp;
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
    if (type->kind == T_STRUCTDEF) type = stype_struc(type->struc);
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

static sym* fundec_2_sym(gtree* t) {
  char* name = t_c_get(t, 1)->d->val_str;
  field* fl = field_new(NULL, t_c_top(t)->d->tp);

  // vl: VarList
  for (gtree* vl = t_c_get(t, 3); vl->d->ts[0] == 'V'; vl = t_c_back(vl)) {
    gtree* p = t_c_top(vl);  // p: ParamDec
    stype* type = t_c_top(p)->d->tp;
    if (type->kind == T_STRUCTDEF) type = stype_struc(type->struc);
    fl_append(fl, field_new(t_c_top(t_c_back(p))->d->val_str, type));
  }
  return sym_new(name, stype_funct(fl), NULL);
}

static inline void set_stype(gtree* t, stype* type) {
  t->d->tp = type;
  t->d->tn = stype_tn(t->d->tp);
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
  t->d->val_str = t_c_top(t)->d->val_str;
}

void on_ExpID(gtree* t) {
  INFO(__FUNCTION__);
  gtree* id = t_c_top(t);
  t->d->val_str = id->d->val_str;
  sym* sid = st_get(TABLE, id->d->val_str);
  ERR(1, (sid == NULL));
  set_stype(t, sid->type);
}

void on_INT(gtree* t) {
  INFO(__FUNCTION__);
  set_stype(t, stype_int());
  t->d->val_int = t->c->ptr->d->val_int;
}

void on_FLOAT(gtree* t) {
  INFO(__FUNCTION__);
  set_stype(t, stype_float());
  t->d->val_flt = t->c->ptr->d->val_flt;
}

void on_SpecTYPE(gtree* t) {
  INFO(__FUNCTION__);
  t->d->tn = TYPE;
  t->d->val_str = t->c->ptr->d->val_str;
  t->d->tp = t->d->val_str[0] == 'i' ? stype_int() : stype_float();
}

void on_SpecSTRUC(gtree* t) {
  INFO(__FUNCTION__);
  gtree* ss = t_c_top(t);
  gtree* tag = t_c_get(ss, 1);
  t->d->tn = STRUCT;
  if (tag != NULL) t->d->val_str = tag->d->val_str;
  if (ss->d->tp != NULL) t->d->tp = ss->d->tp;
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
      set_stype(t, r->d->tp);
      break;
    }
    case AND:
    case OR:
    case RELOP: {
      set_stype(t, r->d->tp);
      ERR(7, (l->d->tn != INT) || (l->d->tn != r->d->tn));
      break;
    }
    case PLUS:
    case MINUS:
    case STAR:
    case DIV: {
      set_stype(t, r->d->tp);
      ERR(7, (l->d->tn != INT && l->d->tn != FLOAT) || (l->d->tn != r->d->tn));
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
      set_stype(t, tmp->type);
    }
  }
  ERR(14, (is_defined == 0));
}

void on_StructDef(gtree* t) {
  INFO(__FUNCTION__);

  gtree* OptTag = t_c_get(t, 1);
  char* name = (OptTag != NULL) ? OptTag->d->val_str : NULL;
  sym* cu_st = sym_new(name, stype_strucdef(NULL), t);

  for (gtree* def_l = t_c_get(t, 3); def_l != NULL && def_l->len == 2;
       def_l = t_c_back(def_l)) {
    StructDef_helper(t_c_top(def_l), cu_st);
  }
  t->d->tp = cu_st->type;
  ERR(16, (st_get(TABLE, name) != NULL));
  st_insert(TABLE, cu_st);
}

void on_StructCall(gtree* t) {
  gtree* tag = t_c_get(t, 1);
  sym* st_type = st_get(TABLE, tag->d->val_str);
  ERR(17, st_type == NULL);
  t->d->tp = st_type->type;
}

void on_CompStDef(gtree* t) {
  INFO(__FUNCTION__);
  char* name;
  stype* type = t_c_top(t)->d->tp;
  if (type == NULL) return;
  gtree* raw;

  for (gtree* dec_l = t_c_get(t, 1); dec_l->d->ts[9] == 'L';
       dec_l = t_c_back(dec_l)) {
    raw = t_c_top(dec_l);  // raw: CompStDec
    name = t_c_top(raw)->d->val_str;
    ERR(3, st_get(TABLE, name) != NULL);
    if (type->kind == T_STRUCTDEF) type = stype_struc(type->struc);
    if (t_c_top(raw)->d->tn == ARRAY) {
      stype* j = t_c_top(raw)->d->tp;
      for (; j->array.elem != NULL; j = j->array.elem)
        ;
      j->array.elem = type;
      type = t_c_top(raw)->d->tp;
    }
    st_insert(TABLE, sym_new(name, type, raw));
    if (raw->len == 3) {
      ERR(5, !stype_is_equal(type, extract_stype(t_c_back(raw))));
    }
  }
}

void on_ExtDef(gtree* t) {
  INFO(__FUNCTION__);
  char* name;
  stype* type = t_c_top(t)->d->tp;
  gtree* raw;

  for (gtree* dec_l = t_c_get(t, 1); dec_l->d->ts[6] == 'L';
       dec_l = t_c_back(dec_l)) {
    raw = t_c_top(dec_l);
    if (raw->len == 3) {
      ERR(5, !stype_is_equal(type, extract_stype(t_c_back(raw))));
    }
    name = t_c_top(raw)->d->val_str;
    ERR(3, st_get(TABLE, name) != NULL);
    if (type->kind == T_STRUCTDEF) type = stype_struc(type->struc);
    st_insert(TABLE, sym_new(name, type, raw));
  }
}

void on_ArrayAccess(gtree* t) {
  INFO(__FUNCTION__);
  ERR(10, t_c_top(t)->d->tn != ARRAY);
  set_stype(t, t_c_top(t)->d->tp->array.elem);
  ERR(12, (t_c_get(t, 2)->d->tn != INT));
}

static void do_on_Stmt(gtree* t, stype* dec) {
  if (t == NULL) return;
  if (t->d->ts != NULL && (strcmp(t->d->ts, "Stmt") == 0)) {
    if (t_c_top(t)->d->tn == RETURN) {
      gtree* exp = t_c_get(t, 1);
      if (!stype_is_equal(dec, exp->d->tp)) {
        printf("Error type %d at Line %d: \n", 8, t->d->pos->first_line);
        return;
      }
    }
  }
  c_list* tmp = t->c;
  while (tmp != NULL) {
    do_on_Stmt(tmp->ptr, dec);
    tmp = tmp->next;
  }
}

void on_FunDefCompSt(gtree* t) {
  INFO(__FUNCTION__);
  gtree* CompSt = t_c_back(t);
  stype* dec = Specifier_stype(t_c_top(t_c_top(t)));

  for (gtree* StmtList = t_c_get(CompSt, 2);
       StmtList != NULL && StmtList->len == 2; StmtList = t_c_back(StmtList)) {
    gtree* Stmt = t_c_top(StmtList);
    do_on_Stmt(Stmt, dec);
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
  set_stype(t, current->type->funct->type);

  field* p = current->type->funct->next;
  gtree* a = t_c_get(t, 2);
  for (; a->d->ts[0] == 'A' && p != NULL; a = t_c_back(a), p = p->next) {
    PERR(9, !stype_is_equal(t_c_top(a)->d->tp, p->type), "arg type missmatch");
  }
  PERR(9, (p != NULL || a->d->ts[0] != 'E'), "arg count missmatch");
}

// ===============  Macro Undef  ===============

#undef ERR
#undef PERR
#undef SPEC_STYPE
#undef INFO