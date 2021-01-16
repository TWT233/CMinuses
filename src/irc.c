#include "irc.h"

// ===============  Macros  ===============

#define NEW(type) ((type*)malloc(sizeof(type)))
#define COPY(type, val) ((type*)memcpy(NEW(type), val, sizeof(type)))

// ===============  Func Defs  ===============

ircl* new_line(char* code) {
  ircl* r = NEW(ircl);

  r->code = code;
  r->p = NULL;
  r->n = NULL;

  return r;
}

ircb* new_block(gtree* t, ircl* lines) {
  ircb* r = NEW(ircb);

  r->raw = t;
  r->lines = lines;
  r->p = NULL;
  r->n = NULL;

  return r;
}

ircl* lines_tail(ircl* lines) {
  if (lines == NULL) return lines;

  ircl* r = lines;
  while (r->n != NULL) r = r->n;
  return r;
}

ircl* line_append(ircl* line, ircl* data) {
  if (line == NULL) return line;
  if (data == NULL) return line;
  if (line->n == NULL) {
    line->n = data;
    data->p = line;
  }

  ircl* tail = lines_tail(data);
  tail->n = line->n;
  line->n->p = tail;
  line->n = data;

  return line;
}

ircb* blocks_tail(ircb* blocks) {
  if (blocks == NULL) return blocks;

  ircb* r = blocks;
  while (r->n != NULL) r = r->n;
  return r;
}

ircb* block_append(ircb* block, ircb* data) {
  if (block == NULL) return block;
  if (data == NULL) return block;
  if (block->n == NULL) {
    block->n = data;
    data->p = block;
  }

  ircb* tail = blocks_tail(data);
  tail->n = block->n;
  block->n->p = tail;
  block->n = data;

  return block;
}

// ===============  Macro Undef  ===============

#undef NEW
#undef COPY