#include <stdio.h>

extern int yyrestart(FILE*);
extern int yyparse(void);

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

  return 0;
}