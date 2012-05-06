/* 
 * file:    errors.c
 * author:  joe
 * date:    20120429
 * notes:   error handling functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "errors.h"

/* global variables */

/* function prototypes */

/* eprintf: print error message and exit */
void eprintf(char *fmt, ...)
{
    va_list args;

    fflush(stdout);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    if (fmt[0] != '\0' && fmt[strlen(fmt) - 1] == ':')
        fprintf(stderr, " %s", strerror(errno));
    fprintf(stderr, "\n");

    exit(EXIT_FAILURE);
}

/* weprintf: print warning message */
void weprintf(char *fmt, ...)
{
    va_list args;

    fflush(stdout);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    if (fmt[0] != '\0' && fmt[strlen(fmt) - 1] == ':')
        fprintf(stderr, " %s", strerror(errno));
    fprintf(stderr, "\n");
}

/* emalloc: malloc and report if error */
void *emalloc(size_t n)
{
    void *p;

    p = malloc(n);
    if (p == NULL)
        eprintf("malloc of %lu bytes failed:", n);

    return p;
}

/* erealloc: realloc and report if error */
void *erealloc(void *s, size_t n)
{
    void *p;

    p = realloc(s, n);
    if (p == NULL)
        eprintf("realloc of %lu bytes failed:", n);

    return p;
}

/* estrdup: duplicate a string, report if error */
char *estrdup(char *s)
{
    char *t;

    t = emalloc(strlen(s) + 1);
    if (t == NULL)
        eprintf("estrdup(\"%.20s\") failed:", s);
    strcpy(t, s);
    return t;
}
