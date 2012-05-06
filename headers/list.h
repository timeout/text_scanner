/* 
 * file:    list.h
 * author:  joe
 * date:    20120429
 * notes:   double linked list
 */

#ifndef SC_LIST_H
#define SC_LIST_H

#include "errors.h"

/* global variables */
typedef struct node node_t;

/* function prototypes */
node_t *new_node(void);
node_t *set_data(node_t *n, void *d);
void *get_data(node_t *n);
node_t *next_node(node_t *n);
void null_node(node_t *n);
void link_node(node_t *m, node_t *n);

node_t *add_front(node_t *listp, node_t *itemp);
node_t *add_end(node_t *listp, node_t *itemp);
node_t *insert(node_t *beforep, node_t *itemp);
void apply(node_t *listp, void(*fn)(node_t *, void *), void *arg);
void size(node_t *listp, void *arg);
node_t *find(node_t *listp, int(*fn)(node_t *, void *), void *arg);
void destroy_list(node_t *listp);

#endif /* SC_LIST_H */
