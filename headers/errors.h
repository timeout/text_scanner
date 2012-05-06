/* 
 * file:    errors.h
 * author:  joe
 * date:    20120429
 * notes:   error handling functions
 */


#ifndef SC_ERRORS_H
#define SC_ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

/* global variables */

/* function prototypes */
void eprintf(char *fmt, ...);
void weprintf(char *fmt, ...);
void *emalloc(size_t n);
void *erealloc(void *s, size_t n);
char *estrdup(char *s);

#endif /* SC_ERRORS_H */
