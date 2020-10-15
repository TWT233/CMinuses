#include <stdio.h>

#include "syntax.tab.h"
#define __MINUSES_GTREE_H_IMPLEMENTS__
#include "gtree.h"

extern int yyrestart(FILE*);
extern int yyparse(void);
extern int error_mark;
extern gtree* root;

size_t counter = 0;

gtree* print_gtree(gtree* t) {
  if (t == NULL || t->d->tn == -1) return NULL;
  ++counter;

  for (size_t i = 1; i < counter; i++) printf("  ");

  switch (t->d->tn) {
    case -1:
      break;
    case 0:
      printf("%s (%d)\n", t->d->ts, t->d->pos->first_line);
      break;
    case ID:
    case TYPE:
      printf("%s: %s\n", t->d->ts, t->d->val_str);
      break;
    case INT:
      printf("%s: %d\n", t->d->ts, t->d->val_int);
      break;
    case FLOAT:
      printf("%s: %lf\n", t->d->ts, t->d->val_flt);
      break;
    default:
      printf("%s\n", t->d->ts);
      break;
  }

  t_c_foreach(t, print_gtree);

  --counter;
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

  yyrestart(f);
  yyparse();

  if (!error_mark) print_gtree(root);

  t_free(root);

  return 0;
}
