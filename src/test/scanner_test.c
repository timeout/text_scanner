#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "paragraph.h"
#include "getline.h"

/* global variables */

/* function prototypes */

int main()
{
    const char *article;
    char *input;
    FILE *fin;
    paragraph_t *p = NULL;
    symtab_t *mnames;

    /* get a paragraph */
    article = "../src/test/data/bad_data.txt";

    fin = fopen(article, "r");
    while ((input = getline(fin, REM_PUNCT)) != NULL) {
        p = add_line(p, clean());
    }

    /* close file */
    fclose(fin);

    p = sentencify(p);

    printf("\nnumber of sentences in %s: %u\n", article, sentence_total(p));

    /* get a symbol table */
    mnames = new_name_table("../src/test/data/names/male_names_eng.txt");

    /* debugging */
    printf("table max: %lu\n", symtab_max(mnames));
    printf("table size: %lu\n", symtab_size(mnames));
    
    /* scan the paragraph */
    p = papply(p, scan, (void *) mnames);

    paragraph_delete(p);
    symtab_delete(mnames);

    return 0;
}
