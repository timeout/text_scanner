/* 
 * file:    getline.c
 * author:  joe
 * date:    20120429
 * notes:   get line from file functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>

#include "getline.h"
#include "errors.h"

#define WHITE " \t"
#define PUNCT "`',()[]{}<>_«»\";:$%^&*+@#|\\/"

/* global variables */

static char *line = NULL;
static char *sline = NULL;
static int maxline = 0;

/* function prototypes */
static void reset(void);
static int eol(FILE *fin, int c);
static int is_punct(char *p);
static int is_white(char *p);
static char *ssanitize(char *str);

char *getline(FILE *fin, int punct)
{
    int i, c;
    char *newl, *snewl;

    if (line == NULL) {
        maxline = 1;
        line = malloc(maxline);
        if (line == NULL) {
            reset();
            return NULL;
        }
    }

    for (i = 0; (c = getc(fin)) != EOF && !eol(fin, c); i++) {
        if (i >= maxline - 1) {
            maxline <<= 1;
            newl = erealloc(line, maxline);
            snewl = erealloc(sline, maxline);
            if (newl == NULL || snewl == NULL) {
                reset();
                return NULL;
            }
            line = newl;
            sline = snewl;
        }

        line[i] = c;
    }
    line[i] = '\0';

    if (punct == REM_PUNCT)
        line = ssanitize(line);

    if (*line == '\0') sline = line;
    else dewhite(line, sline);

    return (c == EOF && i == 0) ? NULL : line;
}

/* reset: set variables back to starting values */
static void reset(void)
{
    free(line);
    line = NULL;
    maxline = 0;
}

/* eol: check for and consume \r, \n, \r\n, or EOF */
static int eol(FILE *fin, int c)
{
    int e;

    e = (c == '\r' || c == '\n');
    if (c == '\r') {
        c = getc(fin);
        if (c != '\n' && c != EOF)
            ungetc(c, fin);
    }
    return e;
}

/* is_punct: is character white space? */
static int is_punct(char *p)
{
    char *punct = PUNCT;
    char *q;

    for (q = punct; *q != '\0'; q++) {
        if (*q == *p)
            return 0;
    }

    return *p;
}


/* sanitize: remove punctuation marks not relevant to sentence endings */
size_t sanitize(char *from, char *to)
{
    size_t i;
    char *it;

    /* empty string */
    if (strlen(from) == 0)
        return 0;

    i = 0;
    for (it = from; *it != '\0'; it++) {
        if (is_punct(it) != 0)
            to[i++] = *it;
        else
            to[i++] = ' ';
    }

    to[i] = '\0';

    return i;
}

static char *ssanitize(char *str)
{
    char *it;

    for (it = str; *it != '\0'; it++) {
        if (is_punct(it) == 0)
            *it = ' ';
    }

    return str;
}

/* is_white: is character white space? */
static int is_white(char *p)
{
    char *white = WHITE;
    char *q;

    for (q = white; *q != '\0'; q++) {
        if (*q == *p)
            return 0;
    }

    return *p;
}

/* dewhite: remove excessive white space */
size_t dewhite(char *from, char *to)
{
    int flag;
    size_t i;
    char *it;

    /* empty string */
    if (strlen(from) == 0)
        return 0;

    i = 0;

    /* remove white spaces before text */
    for (it = from; (is_white(it) == 0) && (*it != '\0'); it++)
        ;

    /* are we at the end of the line? */
    if (*it == '\0') {
        to[i] = '\0';
        return 0;
    }

    /* remove successive whitespaces in line */
    flag = 0;
    /* find next white space or EOL */
    for ( ; *it != '\0'; it++) {
        if (is_white(it) != 0) {
            flag = 1;
            to[i++] = *it;
        } else if (flag == 1) {
            to[i++] = *it;
            flag = 0;
        } 
    }

    /* remove trailing white space */
    if (is_white(&to[i - 1]) == 0) {
        to[i - 1] = '\0';
        return i - 1;
    }

    to[i] = '\0';
    return i;
}

char *clean(void)
{
    return sline;
}

