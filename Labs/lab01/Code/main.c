#include <stdio.h>

#include "syntax.tab.h"
#define __MINUSES_GTREE_H_IMPLEMENTS__
#include "gtree.h"

extern int yyrestart(FILE*);
extern int yyparse(void);
extern gtree* root;

size_t counter = 0;

int str2int(const char* str) {
  int ret = 0;

  return ret;
}

gtree* print_gtree(gtree* t) {
  if (t == NULL) return NULL;
  ++counter;

  for (size_t i = 1; i < counter; i++) printf("  ");

  switch (t->d->tn) {
    case 0:
      printf("%s (%d)\n", t->d->ts, t->d->pos->first_line);
      break;
    case ID:
    case TYPE:
      printf("%s: %s\n", t->d->ts, t->d->val);
      break;
    case INT:
      printf("%s: %d\n", t->d->ts, atoi(t->d->val));
      // TODO: ADD OCT/HEX HANDLING
      break;
    case FLOAT:
      printf("%s: %lf\n", t->d->ts, atof(t->d->val));
      break;
    default:
      printf("%s\n", t->d->ts);
      break;
  }

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
