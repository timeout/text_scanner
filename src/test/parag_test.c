#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "list.h"
#include "getline.h"
#include "paragraph.h"

/* global variables */

/* function prototypes */

int main()
{
    char *l = "This is a line";
    char *m = "This is    T. another  line.       of text   .";
    char *n;
    paragraph_t *p;

    /* first test */
    p = NULL;

    n = emalloc(strlen(m) + 1);
    dewhite(m, n);

    p = add_line(p, l);
    p = add_line(p, l);
    p = add_line(p, n);
    p = add_line(p, n);
    p = add_line(p, n);

    printf("\nsentencify()...\n");
    p = sentencify(p);

    pparagraph(p);

    printf("\nnumber of sentences: %u\n", sentence_total(p));

    paragraph_delete(p);

    /* second test */
    char *path;
    FILE *fin;
    char *la;

    p = NULL;

    /*path = "../../bilanz/data/bad_data.txt";*/
    /*path = "test/data/the_call_of_the_wild.txt";*/
    path = "../src/test/data/para1.txt";

    fin = fopen(path, "r");
    while ((la = getline(fin, REM_PUNCT)) != NULL) {
        p = add_line(p, clean());
    }

    /* close file */
    fclose(fin);

    p = sentencify(p);

    pparagraph(p);
    printf("\nnumber of sentences: %u\n", sentence_total(p));

    paragraph_delete(p);

    /* third test */
    path = "../src/test/data/para2.txt";

    p = NULL;
    la = NULL;

    fin = fopen(path, "r");
    while ((la = getline(fin, REM_PUNCT)) != NULL) {
        p = add_line(p, clean());
    }

    /* close file */
    fclose(fin);

    p = sentencify(p);

    pparagraph(p);
    printf("\nnumber of sentences: %u\n", sentence_total(p));

    paragraph_delete(p);

    /* fourth test */
    path = "../src/test/data/the_call_of_the_wild.txt";

    p = NULL;

    fin = fopen(path, "r");
    while ((la = getline(fin, REM_PUNCT)) != NULL) {
        p = add_line(p, clean());
    }

    /* close file */
    fclose(fin);

    p = sentencify(p);

    pparagraph(p);
    printf("\nnumber of sentences: %u\n", sentence_total(p));

    paragraph_delete(p);

    return 0;
}
