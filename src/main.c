#include <stdio.h>

#include "gtree.h"
#include "syntax.tab.h"

extern int yyrestart(FILE*);
extern int yyparse(void);
extern int error_mark;
extern gtree* root;

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

  if (!error_mark) t_print(root);

  t_free(root);

  return 0;
}
