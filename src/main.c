#include <stdio.h>

#include "gtree.h"
#include "syntax.tab.h"
#include "traverser.h"

extern int yyrestart(FILE*);
extern int yyparse(void);
extern gtree* root;

gtree* fun_check(gtree* t) {
  if (t == NULL) return t;
  sym* s = NULL;
  if (strcmp(t->d->ts, "FunCall") == 0) {
    s = st_get(get_table(), t_c_top(t)->d->val_str);
  } else if (strcmp(t->d->ts, "FunDecSig") == 0) {
    s = st_get(get_table(), t_c_get(t, 1)->d->val_str);
  }
  if (s == NULL) return t_c_foreach(t, fun_check);
  if (s->raw == NULL) {
    printf("Error type %d at Line %d: %s\n", 18, t->d->pos->first_line,
           "undefined function");
    return t;
  }
  return t;
}

int main(int argc, char** argv) {
  if (argc <= 1) {
    printf("usage: scanner <filename>\n");
    return 1;
  }

  FILE* f = fopen(argv[1], "r");
  if (!f) {
    perror(argv[1]);
    return 1;
  }

  init_table();
  yyrestart(f);
  yyparse();

  fun_check(root);

  t_free(root);

  return 0;
}
