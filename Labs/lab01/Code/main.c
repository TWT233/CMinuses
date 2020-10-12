#include <stdio.h>

#include "tokens.h"

extern FILE *yyin;
extern int yylineno;
extern char *yytext;
extern int yylex(void);

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("usage: scanner <filename>\n");
    return 1;
  }

  if (!(yyin = fopen(argv[1], "r"))) {
    perror(argv[1]);
    return 1;
  }

  unsigned int type = 0;
  while ((type = yylex()) != 0) {
    printf("0x%08x %s\t%s\t%d\t\"%s\"\n", type, get_g(type), get_c(type),
           yylineno, yytext);
  }

  return 0;
}