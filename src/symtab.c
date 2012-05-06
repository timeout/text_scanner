/* 
 * file:    list.h
 * author:  joe
 * date:    20120429
 * notes:   symbol table. Based on http://www.eternallyconfuzzled.com
 *          (!) generic hash table needs to be able to access a key as a
 *          particular element in the data to be hashed. Can't base the
 *          hash table on existing list class.
 */

#include <stdio.h>

#include "symtab.h"
#include "errors.h"

/* global variables */
typedef struct symtab_node {
    void                *key;
    void                *data;
    struct symtab_node  *next;
} symtab_node_t;

struct symtab {
    symtab_node_t   **table;    /* hash table */
    size_t          size;       /* current item count */
    size_t          max;        /* maximum table size */
    hash_f          hash;       /* hash function */
    cmp_f           cmp;        /* data comparison function */
    keydup_f        keydup;     /* key duplication function */
    datadup_f       datadup;    /* data duplication function */
    keyrel_f        keyrel;     /* key release function */
    datarel_f       datarel;    /* data release function */
};

/* function prototypes */
static symtab_node_t *new_sym_node(void *key, void *data, symtab_node_t *next)
{
    symtab_node_t *n;

    n = emalloc(sizeof(symtab_node_t));

    if (n == NULL)
        return NULL;

    n->key = key;
    n->data = data;
    n->next = next;

    return n;
}

symtab_t *symtab_new(size_t size, hash_f hash, cmp_f cmp, keydup_f keydup,
        datadup_f datadup, keyrel_f keyrel, datarel_f datarel)
{
    symtab_t *symtab;
    size_t i;

    symtab = emalloc(sizeof(symtab_t));
    if (symtab == NULL)
        return NULL;

    symtab->table = emalloc(size * sizeof(*symtab->table));
    if (symtab->table == NULL) {
        free(symtab);
        return NULL;
    }

    for (i = 0; i < size; i++)
        symtab->table[i] = NULL;

    symtab->size = 0;
    symtab->max = size;
    symtab->hash = hash;
    symtab->cmp = cmp;
    symtab->keydup = keydup;
    symtab->datadup = datadup;
    symtab->keyrel = keyrel;
    symtab->datarel = datarel;

    return symtab;
}

void symtab_delete(symtab_t *symtab)
{
    size_t index;
    symtab_node_t *it, *tmp;

    for (index = 0; index < symtab->max; index++) {
        if (symtab->table[index] == NULL)
            continue;

        it = symtab->table[index];
        for ( ; it != NULL; it = tmp) {
            tmp = it->next;
            symtab->keyrel (it->key);
            symtab->datarel (it->data);
            free(it);
        }
    }

    free(symtab->table);
    free(symtab);
}

void *symtab_lookup(symtab_t *symtab, void *key)
{
    unsigned index;
    symtab_node_t *it;

    index = symtab->hash(key) % symtab->max;
    
    if (symtab->table[index] != NULL) {
        it = symtab->table[index];
        for ( ; it != NULL; it = it->next) {
            if (symtab->cmp (key, it->key) == 0)
                return it->data;
        }
    }

    return NULL;
}

int symtab_insert(symtab_t *symtab, void *key, void *data)
{
    unsigned index;
    void *dupkey, *dupdata;
    symtab_node_t *entry;

    index = symtab->hash (key) % symtab->max;

    if (symtab_lookup(symtab, key) != NULL)
        return 0;

    dupkey = symtab->keydup (key);      /* dupkey is NULL? */
    dupdata = symtab->datadup (data);

    entry = new_sym_node(dupkey, dupdata, NULL);

    if (entry == NULL)
        return 0;

    entry->next = symtab->table[index];
    symtab->table[index] = entry;

    ++symtab->size;

    return 1;
}

size_t symtab_size(symtab_t *symtab)
{
    return symtab->size;
}

size_t symtab_max(symtab_t *symtab)
{
    return symtab->max;
}
