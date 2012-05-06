/* 
 * file:    getline.h
 * author:  joe
 * date:    20120429
 * notes:   get line from file functions
 */

#ifndef SC_GETLINE_H
#define SC_GETLINE_H

#define REM_PUNCT 1
#define KEEP_PUNCT 0

/* function prototypes */
char *getline(FILE *fin, int punct);
size_t sanitize(char *from, char *to);
size_t dewhite(char *from, char *to);
char *clean(void);

#endif /* SC_GETLINE_H */
