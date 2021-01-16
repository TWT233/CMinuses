#ifndef __MINUSES_IRC_H__
#define __MINUSES_IRC_H__

#include "gtree.h"

// ===============  Types  ===============

typedef struct irc irc;

struct irc {
  gtree* raw;
  char* code_block;
  irc* nest;
  irc* next;
};

// ===============  Func Defs  ===============

irc* gen_code_block(irc* c);

irc* new_irc(gtree* t);

irc* irc_nest_append(irc* c, irc* child);

irc* append(irc* c, irc* next);

#endif  //__MINUSES_GTREE_H__