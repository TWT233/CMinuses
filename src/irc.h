#ifndef __MINUSES_IRC_H__
#define __MINUSES_IRC_H__

#include <stdlib.h>

#include "gtree.h"
#include "syntax.tab.h"

// ===============  Types  ===============

typedef struct ircb ircb;
typedef struct ircl ircl;

struct ircl {
  char* code;
  ircl* p;
  ircl* n;
};

struct ircb {
  gtree* raw;
  ircl* lines;
  ircb* p;
  ircb* n;
};

// ===============  Func Defs  ===============

ircl* new_line(char* code);

ircb* new_block(gtree* t, ircl* lines);

ircl* lines_tail(ircl* lines);

ircl* line_append(ircl* line, ircl* data);

ircb* blocks_tail(ircb* blocks);

ircb* block_append(ircb* block, ircb* data);

#endif  //__MINUSES_GTREE_H__