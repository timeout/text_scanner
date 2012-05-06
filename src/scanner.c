/* 
 * file:    scanner.c
 * author:  joe
 * date:    20120505
 * notes:   scanner functions: functions for scanning a paragraph of text
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "symtab.h"

#include "scanner.h"
#include "paragraph.h"
#include "getline.h"

/* global variables */

/* function prototypes */
static unsigned fnv_hash(const void *key);
static int st_strcmp(const void *a, const void *b);
static void *st_strdup(const void *key);
static void st_strrel(const void *s);

static unsigned line_count(FILE *fin);
static size_t csprimes(unsigned n);



static unsigned fnv_hash(const void *key)
{
    size_t len, i;
    char *w;
    unsigned h;

    w = (char *) key;
    len = strlen(w);

    h = 2166136261;
    for (i = 0; i < len; i++)
        h = ( h * 16777619 ) ^ w[i];

    return h;
}

static int st_strcmp(const void *a, const void *b)
{
    char *m, *n;

    m = (char *) a;
    n = (char *) b;

    return strcmp(m, n);
}

static void *st_strdup(const void *key)
{
    char *k;

    k = (char *) key;

    return estrdup(k);
}

static void st_strrel(const void *s)
{
    char *str;

    str = (char *) s;

    free(str);
}

static unsigned line_count(FILE *fin)
{
    unsigned n = 0;
    int c;

    while ((c = getc(fin)) != EOF) {
        if (c == '\n')
            n++;
    }

    return n;
}

static size_t csprimes(unsigned n)
{
    int i;
    size_t arr;

    for (i = 1, arr = 1; arr < n; i++) {
        arr = (i * i) + (i + 1) * (i + 1);
    }

    return arr;
}

symtab_t *new_name_table(char *path)
{
    unsigned linec;
    char *la, *key, *data;
    FILE *fin;

    symtab_t *table;
    size_t size;
    hash_f hash = &fnv_hash;
    cmp_f cmp = &st_strcmp;
    keydup_f keydup = &st_strdup;
    datadup_f datadup = &st_strdup;
    keyrel_f keyrel = &st_strrel;
    datarel_f datarel = &st_strrel;

    /* get a file pointer */
    fin = fopen(path, "r");
    if (fin == NULL)
        printf("Counldn't open file: '%s'\n.", path);

    /* get size of table */
    linec = (line_count(fin) * 3) / 2;
    size = csprimes(linec);

    table = symtab_new(size, hash, cmp, keydup, datadup, keyrel, datarel);

    if (table == NULL)
        printf("Coundn't create table:\n");

    /* populate table with list of names from path */
    rewind(fin);
    while ((la = getline(fin, REM_PUNCT)) != NULL) {
        key = data = clean();
        symtab_insert(table, (void *) key, (void *) data);
    }
    /* close the file */
    fclose(fin);

    return table;
}

void scan(node_t *n, void *symtab)
{
    symtab_t *symtable;
    word_t *word;
    char *key;

    word = (word_t *) get_data(n);

    symtable = (symtab_t *) symtab;
    key = (char *) symtab_lookup(symtable, (void *) wort_to_string(word));

    if (key != NULL)
        printf("found: '%s'\n", key);
}

