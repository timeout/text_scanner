/* 
 * file:    test.c
 * author:  joe
 * date:    20120502
 * notes:   main driver for getline testing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "getline.h"

/* function prototypes */

int main()
{
    char *path;
    FILE *fin;
    char *la, *lb, *lc;

    path = "../src/test/data/names/male_names.txt";

    /* test 1 */
    fin = fopen(path, "r");
    while ((la = getline(fin, 0)) != NULL) {
        printf("getline(): '%s'\n", la);

        lb = emalloc(strlen(la) + 1);
        sanitize(la, lb);
        printf("sanitize(): '%s'\n", lb);

        lc = emalloc(strlen(lb) + 1);
        printf("size of lb: %lu\n", dewhite(lb, lc));
        printf("dewhite(): '%s'\n", lc);

        free(lb);
        free(lc);
        lc = lb = NULL;
    }

    /* close file */
    fclose(fin);

    /* test 2 */
    fin = fopen(path, "r");
    while ((la = getline(fin, REM_PUNCT)) != NULL) {
        printf("getline(): '%s'\n", la);

        printf("cleaned: '%s'\n", clean());
    }

    /* close file */
    fclose(fin);


    return EXIT_SUCCESS;
}
