/* 
 * file:    list.c
 * author:  joe
 * date:    20120429
 * notes:   double linked list
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/* global variables */
struct node {
    void *data;
    struct node *next;
    struct node *prev;
};

/* function prototypes */

/* node: create an empty node */
node_t *new_node()
{
    node_t *n;

    n = emalloc(sizeof(node_t));
    n->next = NULL;
    n->prev = NULL;
    n->data = NULL;

    return n;
}

/* data: add data to a node */
node_t *set_data(node_t *n, void *d)
{
    n->data = d;

    return n;
}

void link_node(node_t *m, node_t *n)
{
    m->next = n;
}

/* ndata: get a pointer to a node't data. */
void *get_data(node_t *n)
{
    return n->data;
}

/* nnode: get the next node. */
node_t *next_node(node_t *n)
{
    return n->next;
}

/* null_node: set node's next pointer to null */
void null_node(node_t *n)
{
    n->next = NULL;
}

/* add_front: add an item to the front of a list */
node_t *add_front(node_t *listp, node_t *itemp)
{
    if (listp == NULL)
        return itemp;
        
    itemp->next = listp;
    listp->prev = itemp;

    return itemp;
}

/* add_end: add an item to the end of a list */
node_t *add_end(node_t *listp, node_t *itemp)
{
    node_t *p;

    if (listp == NULL)
        return itemp;

    /* find the last node */
    for (p = listp; p->next != NULL; p = p->next)
        ;

    /* add node */
    p->next = itemp;
    itemp->prev = p;

    return listp;
}

/* insert: insert an item into a list return pointer to inserted item */
node_t *insert(node_t *beforep, node_t *itemp)
{
    if (beforep == NULL)
        return itemp;

    itemp->next = beforep->next;
    itemp->prev = beforep;
    beforep->next = itemp;
    itemp->next->prev = itemp;

    return itemp;
}

/* apply: execute function for each element of listp */
void apply(node_t *listp, void(*fn)(node_t *, void *), void *arg)
{
    for ( ; listp != NULL; listp = listp->next) {
        (*fn)(listp, arg);
    }
}

/* size: count the number of nodes from listp */
void size(node_t *listp, void *arg)
{
    unsigned *ip;

    ip = (unsigned *) arg;
    (*ip)++;
}

/* find: find a node in the list */
node_t *find(node_t *listp, int(*fn)(node_t *, void *), void *arg)
{
    for ( ; listp != NULL; listp = listp->next)
        if((*fn)(listp, arg) == 0)
            return listp;

    return listp;
}

/* destroy_list: destroys the list, data should be destroyed beforehand */
void destroy_list(node_t *listp)
{
    node_t *n;

    for ( ; listp != NULL; listp = n) {
        n = listp->next;
        free(listp);
    }
}
