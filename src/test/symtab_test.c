#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "symtab.h"
#include "getline.h"

/* global variables */

/* function prototypes */
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

int st_strcmp(const void *a, const void *b)
{
    char *m, *n;

    m = (char *) a;
    n = (char *) b;

    return strcmp(m, n);
}

void *st_strdup(const void *key)
{
    char *k;

    k = (char *) key;

    return estrdup(k);
}

void st_strrel(const void *s)
{
    char *str;

    str = (char *) s;

    free(str);
}

int main()
{
    char *key, *data, *x;

    char *path;
    FILE *fin;
    char *la;

    symtab_t *table;
    hash_f hash;
    cmp_f cmp;
    keydup_f keydup;
    datadup_f datadup;
    keyrel_f keyrel;
    datarel_f datarel;

    hash = &fnv_hash;
    cmp = &st_strcmp;
    keydup = &st_strdup;
    datadup = &st_strdup;
    keyrel = &st_strrel;
    datarel = &st_strrel;

    /* Test 1 */
    key = "John";
    data = "Lives in Sydney";

    table = symtab_new(5, hash, cmp, keydup, datadup, keyrel, datarel);

    symtab_insert(table, (void *) key, (void *) data);
    x = (char *) symtab_lookup(table, (void *) key);

    printf("%s => '%s'\n", key, x);
    printf("symtab: size: %lu; max: %lu\n", symtab_size(table), 
            symtab_max(table));

    x = (char *) symtab_lookup(table, (void *) "Tom");
    printf("%s => '%s'\n", "Tom", x);

    symtab_delete(table);

    /* Test 2 */

    table = symtab_new(3571, hash, cmp, keydup, datadup, keyrel, datarel);

    path = "../src/test/data/names/male_names.txt";

    fin = fopen(path, "r");
    if (fin == NULL){
        printf("Couldn't open file: '%s'\n", path);
    }
        
    while ((la = getline(fin, REM_PUNCT)) != NULL) {

        key = data = clean();
        printf("inserting: '%s'\n", key);

        symtab_insert(table, (void *) key, (void *) data);
    }

    symtab_insert(table, (void *) "Joe", (void *) "Joe");

    x = (char *) symtab_lookup(table, (char *) "Marko");
    printf("Marko => '%s'\n", x);

    x = (char *) symtab_lookup(table, (char *) "Paul");
    printf("Paul => '%s'\n", x);

    x = (char *) symtab_lookup(table, (char *) "Joe");
    printf("Joe => '%s'\n", x);

    printf("symtab: size: %lu; max: %lu\n", symtab_size(table), 
            symtab_max(table));

    /* close file */
    fclose(fin);
    printf("file closed: '%s'\n", path);

    symtab_delete(table);

    return 0;

}
