#include <stdio.h>

extern FILE *yyin;
extern int yylineno;
extern char *yytext;

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("usage: scanner <filename>\n");
    return 1;
  }

  if (!(yyin - fopen(argv[1], 'r'))) {
    perror(argv[1]);
    return 1;
  }

  unsigned int type = 0;
  while ((type = yylex()) != 0) {
    printf("type: 0x%x @ <%d,%s>\n", type, yylineno, yytext);
  }

  return 0;
}