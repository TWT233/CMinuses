#include "irc.h"

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

irc* gen_code_block(irc* c) {}

irc* new_irc(gtree* t) {
  irc* r = NEW(irc);

  r->raw = t;
  r->code_block = NULL;
  r->nest = NULL;
  r->next = NULL;

  return r;
}

irc* irc_nest_append(irc* c, irc* child) {
  if (c == NULL) return c;
  if (c->nest == NULL) {
    c->nest = child;
    return c;
  }
  irc* tmp = c->nest;
  for (; tmp->next != NULL; tmp = tmp->next)
    ;
  tmp->next = child;
  return c;
}

irc* append(irc* c, irc* next) {
  if (c == NULL) return c;
  irc* tmp = c;
  for (; tmp->next != NULL; tmp = tmp->next)
    ;
  tmp->next = next;
  return c;
}

// ===============  Macro Undef  ===============

#undef NEW
#undef COPY