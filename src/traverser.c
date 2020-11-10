#include "traverser.h"

#include <stdio.h>

// ===============  Macros  ===============


// ===============  Var Defs  ===============

static sym_table* TABLE = NULL;

// ===============  Func Defs  ===============

void init_table() { TABLE = st_new(); }

sym_table* get_table() {
  if (TABLE == NULL) init_table();
  return TABLE;
}



// ===============  Macro Undef  ===============

