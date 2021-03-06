#include "utils.h"

#include <string.h>

int otoi(const char* str) {
  int ret = 0;

  int tmp = 1;
  for (int i = strlen(str) - 1; i >= 0; i--) {
    ret += (str[i] - '0') * tmp;
    tmp *= 8;
  }

  return ret;
}

int htoi(const char* str) {
  int ret = 0;

  int tmp = 1;
  for (int i = strlen(str) - 1; i > 1; i--) {
    ret += (str[i] - (str[i] > '9' ? ((str[i] > 'F' ? 'a' : 'A') - 10) : '0')) *
           tmp;
    tmp *= 16;
  }

  return ret;
}