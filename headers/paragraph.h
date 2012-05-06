/* 
 * file:    paragraph.h
 * author:  joe
 * date:    20120501
 * notes:   paragraph
 */

#ifndef SC_PARAGRAPH_H
#define SC_PARAGRAPH_H

#include "list.h"
#include "errors.h"

/* global variables */
typedef enum word_cat {
    reg,        /* regular word */
    male,       /* male name */
    female,     /* female name */
    unisex,     /* uni-sex name */
    corp,       /* private corporation */
    pub_inst    /* public instituton */
} word_cat_t;

typedef struct word word_t;
typedef struct sentence sentence_t;
typedef struct paragraph paragraph_t;

/* function prototypes */
paragraph_t *add_line(paragraph_t *paragraph, char *sline);
paragraph_t *sentencify(paragraph_t *paragraph);
paragraph_t *papply(paragraph_t *paragraph, 
        void(*fn)(node_t *, void *), void *arg);

void pparagraph(paragraph_t *paragraph);

unsigned sentence_index(sentence_t *sentence);
unsigned sentence_total(paragraph_t *paragraph);
unsigned wordc(sentence_t *sentence);

char *wort_to_string(word_t *word);

void paragraph_delete(paragraph_t *paragraph);

#endif /* SC_PARAGRAPH_H */
