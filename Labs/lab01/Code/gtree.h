#ifndef __MINUSES_GTREE_H__
#define __MINUSES_GTREE_H__

union data_pack {
  int t_int;
  float t_float;
  double t_double;
  char t_char;
  char* t_str;
};

struct linked_child {
  linked_child* next;
  gtree* ptr;
};

struct gtree {
  data_pack* d;
  int len;
  linked_child* c;
};

#endif  //__MINUSES_GTREE_H__