/* 
 * file:    symtab.h
 * author:  joe
 * date:    20120429
 * notes:   symbol table functions
 */

#ifndef SC_SYMTAB_H
#define SC_SYMTAB_H

#include <stdio.h>

#include "list.h"

/* global variables */
typedef struct symtab symtab_t;

typedef unsigned (*hash_f) (const void *key);
typedef int (*cmp_f) (const void *a, const void *b);
typedef void *(*keydup_f) (const void *key);
typedef void *(*datadup_f) (const void *data);
typedef void (*keyrel_f) (const void *key);
typedef void (*datarel_f) (const void *data);

/* function prototypes */
symtab_t *symtab_new(size_t size, hash_f hash, cmp_f cmp, keydup_f keydup,
        datadup_f datadup, keyrel_f keyrel, datarel_f datarel);
void symtab_delete(symtab_t *symtab);
void *symtab_lookup(symtab_t *symtab, void *key);
int symtab_insert(symtab_t *symtab, void *key, void *data);
size_t symtab_size(symtab_t *symtab);
size_t symtab_max(symtab_t *symtab);

#endif  /* SC_SYMTAB_H */
