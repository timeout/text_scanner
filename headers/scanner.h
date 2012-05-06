/* 
 * file:    scanner.h
 * author:  joe
 * date:    20120505
 * notes:   scanner functions: functions for scanning a paragraph of text
 */

#ifndef SC_SCANNER_H
#define SC_SCANNER_H

#include "paragraph.h"
#include "symtab.h"

/* global variables */

/* function prototypes */
symtab_t *new_name_table(char *path);
void scan(node_t *n, void *symtab);

#endif /* SC_SCANNER_H */
