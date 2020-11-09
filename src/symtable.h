#ifndef __MINUSES_HTABLE_H__
#define __MINUSES_HTABLE_H__

#include <stdlib.h>

#include "gtree.h"
#include "stype.h"

// ===============  Macros  ===============

#define ST_SIZE 0x3ff

// ===============  Types  ===============

typedef struct sym_list sym_list;
typedef struct sym sym;
typedef struct sym_table sym_table;

// linked symbol entries
struct sym_list {
  sym* ptr;
  sym_list* next;
};

// symbol
struct sym {
  char* name;
  stype* type;
  gtree* raw;
};

// symbol table
struct sym_table {
  sym_list** table;
  size_t len;
};

// ===============  Func Decs  ===============

// new a `sym`bol with `name` and `type`
sym* sym_new(char* name, stype* type, gtree* raw);

// free a `sym`
void sym_free(sym* ptr);

// new a `sym_list entry` points to `ptr`
sym_list* sl_new(sym* ptr);

//  do `f()` on each entry in `sl`
sym_list* sl_foreach(sym_list* sl, sym_list* f(sym_list*));

// free a `sym_list`
void sl_free(sym_list* sl);

// hash `str` to fit `sym_table`
unsigned st_hash(char* str);

/**
 * @brief new a `sym_table`
 *
 * @return a pointer links to the `sym_table` alloced & inited
 *
 * sym_table structure:
 *
 *  +-------------+       +------------------+
 *  |  sym_table  |  +--->|0|1|2|3|4|..|len-1| hash_table
 *  |-----+-------|  |    +----+-------------+
 *  | len | table +--+         |
 *  +-----+-------+            v        linked_list(sym_list)
 *                          +------+-----+       +------+-----+
 *                          | next | sym |   +-->| next | sym |   +-->NULL
 *                          +--+---+-----+   |   +--+---+-----+   |
 *                             |             |      |             |
 *                             +-------------+      +-------------+
 *
 */
sym_table* st_new();

/**
 * @brief insert an entry to `st`
 *
 * @return 0 indicates no error, 1 when sym with `name` already exist(will not
 * insert)
 */
int st_insert(sym_table* st, sym* sym);

/**
 * @brief remove the sym entry with `name`
 *
 * @return 0 indicates no error, 1 if sym with `name` not exist
 */
int st_remove(sym_table* st, char* name);

/**
 * @brief get the sym entry with `name`
 *
 * @return pointer to the entry, NULL if not found
 */
sym* st_get(sym_table* st, char* name);

/**
 * @brief free `st`
 */
void st_free(sym_table* st);

// ===============  Macro Undef  ===============

#endif  //__MINUSES_HTABLE_H__