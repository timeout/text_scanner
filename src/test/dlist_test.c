/* 
 * file:    test.c
 * author:  joe
 * date:    20120501
 * notes:   main driver for dlist testing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "list.h"

/* function prototypes */
void printstr(node_t *n, void *arg);
int tstrcmp(node_t *n, void *arg);

void printstr(node_t *n, void *arg)
{
    char *fmt;

    fmt = (char *) arg;
    printf(fmt, (char *) get_data(n));
}

int tstrcmp(node_t *n, void *arg)
{
    char *s, *t;

    s = (char *) arg;
    t = (char *) get_data(n);

    return strcmp(s, t);
}

int main(void)
{
    int i, count;
    char *s[6] = {"This", "is", "a", "sentence.", NULL, ""};
    char *t[2] = {"really", "terribly"};
    char *q;
    node_t *head, *item, *findp;

    /* first test: add_front */
    head = NULL;

    for (i = 0; i < 6; i++) {
        item = new_node();
        item = set_data(item, (void *) s[i]);
        head = add_front(head, item);
    }

    apply(head, printstr, "'%s' ");
    printf("\n");

    /* size */
    count = 0;
    apply(head, size, &count);
    printf("size of list is: %d\n", count);

    destroy_list(head);

    /* second test: add_end */
    head = NULL;

    for (i = 0; i < 6; i++) {
        item = new_node();
        item = set_data(item, (void *) s[i]);
        head = add_end(head, item);
    }

    apply(head, printstr, "'%s' ");
    printf("\n");

    destroy_list(head);

    /* third test: insert */
    head = NULL;
    for (i = 0; i < 6; i++) {
        item = new_node();
        item = set_data(item, (void *) s[i]);
        head = add_end(head, item);
    }

    apply(head, printstr, "'%s' ");
    printf("\n");

    q = "a";              /* search query */
    findp = find(head, tstrcmp, q);

    if (findp != NULL) {
        printf("printint() found: '%s'\n", (char *) get_data(findp));
    }

    for (i = 0; i < 2; i++) {
        item = new_node();
        item = set_data(item, (void *) t[i]);
        findp = insert(findp, item);
    }
    
    apply(head, printstr, "'%s' ");
    printf("\n");

    /* size */
    count = 0;
    apply(head, size, &count);
    printf("size of list is: %d\n", count);

    destroy_list(head);

    return EXIT_SUCCESS;
}
