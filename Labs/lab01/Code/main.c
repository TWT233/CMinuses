#include <stdio.h>

#define __MINUSES_GTREE_H_IMPLEMENTS__
#include "gtree.h"

extern int yyrestart(FILE*);
extern int yyparse(void);
extern gtree* root;

size_t counter = 0;

gtree* print_gtree(gtree* t) {
  if (t == NULL) return NULL;
  ++counter;

  for (size_t i = 1; i < counter; i++) printf("  ");
  printf("%s (%d)\n", (t->d->type == 0) ? t->d->name : t->d->val,
         t->d->pos->first_line);
  foreach_child(t, print_gtree);

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

  print_gtree(root);

  free_gtree(root);

  return 0;
}
