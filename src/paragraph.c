/* 
 * file:    paragraph.c
 * author:  joe
 * date:    20120501
 * notes:   paragraph
 */
#include <ctype.h>

#include "paragraph.h"
#include "list.h"
#include "errors.h"

/* global variables */
const char *terminals = ".?!";

struct word {
    char        *w;
    word_cat_t  wcat;
};

struct sentence {
    unsigned        number;
    unsigned        word_count;
    node_t          *first_word;
    node_t          *names;
    node_t          *institution;
    struct sentence *next_sentence;
};

struct paragraph {
    unsigned sentence_count;
    struct sentence *first_sentence;
};

/* function prototypes */
static node_t *new_word(char *w, word_cat_t wcat);
static sentence_t *new_sentence(void);
static paragraph_t *new_paragraph(void);
static int terminal_char(word_t *word);
static size_t add_terminal(word_t *word);
static paragraph_t *recalc(paragraph_t *paragraph);
static paragraph_t *prune(paragraph_t *paragraph);
static void delete_sentence_node(sentence_t *before, sentence_t *del);
static void psentence(node_t *first_word, void *arg);



static node_t *new_word(char *w, word_cat_t wcat)
{
    node_t *wnode;
    word_t *word;

    wnode = new_node();
    if (wnode == NULL)
        return NULL;

    word = emalloc(sizeof(word_t));
    if (word == NULL) {
       free(wnode);
       return NULL;
    }

    word->w = estrdup(w);
    word->wcat = wcat;

    return set_data(wnode, (void *) word);
}

static sentence_t *new_sentence(void)
{
    sentence_t *sentence;

    sentence = emalloc(sizeof(sentence_t));
    if (sentence == NULL)
        return NULL;

    sentence->number = 0;
    sentence->word_count = 0;
    sentence->first_word = NULL;
    sentence->names = NULL;
    sentence->institution = NULL;
    sentence->next_sentence = NULL;

    return sentence;
}

static paragraph_t *new_paragraph(void)
{
    paragraph_t *paragraph;

    paragraph = emalloc(sizeof(paragraph_t));
    if (paragraph == NULL)
        return NULL;

    paragraph->sentence_count = 0;
    paragraph->first_sentence = NULL;

    return paragraph;
}

static sentence_t *add_sentence(paragraph_t *paragraph)
{
    unsigned n;
    sentence_t *line, *it;

    line = new_sentence();
    if (paragraph->first_sentence == NULL) {
        paragraph->first_sentence = line;
        return line;
    }

    it = paragraph->first_sentence;
    for (n = 0 ; it->next_sentence != NULL; it = it->next_sentence, n++)
        ;

    it->next_sentence = line;
    it->word_count = n;

    return line;
}

paragraph_t *add_line(paragraph_t *paragraph, char *sline)
{
    int i, n;
    char *p, *q, *word;
    sentence_t *nline;
    word_cat_t cat;
    
    if (*sline == '\0')
        return paragraph;

    if (paragraph == NULL) {
        paragraph = new_paragraph();
    }

    nline = add_sentence(paragraph);

    p = q = sline;
    cat = 0;            /* TODO: regular */

    /* preconditition: word is dewhited */
    for (n = 0; n < strlen(sline) + 1; p++, n++) {
        if (*p == ' ' || *p == '\0') {      /* space */
            i = 0;
            word = emalloc(p - q + 1);      /* reserve mem for word */
            for ( ; q < p; q++) {           /* copy word into mem */
                word[i++] = *q;
            }
            q = p + 1;
            word[i] = '\0';
            nline->first_word = add_end(nline->first_word, 
                    new_word(word, cat));
        }
    }

    return paragraph;
}

static int terminal_char(word_t *word)
{
    int ret, len;
    const char *it, *str;

    str = word->w;
    
    ret  = 0;
    for (it = terminals; *it != '\0'; it++)
        if (str[strlen(str) - 1] == *it)
            ret = *it;
    
    /* TODO: -1: no end of sentence marker, but don't add period */
    if ((ret == '.') && (len = strlen(str)) > 1) {        /* not a period */
        if (isupper(str[len - 2]) != 0)                   /* C. */
            ret = -1;
        if (str[len - 2] == '.')                          /* ellipsis */
            ret = -1; 
        if (len >= 3 && str[len - 3] == '.')              /* d.h. */
            ret = -1;
    }

    return ret;
}

static size_t add_terminal(word_t *word)
{
    char *new, *old;
    size_t ret;

    old = word->w;

    ret = strlen(old) + 2;

    /* reserve memory for word + terminal character */
    new = emalloc(ret);
    strcpy(new, old);

    new[strlen(old)] = '.';
    new[strlen(old) + 1] = '\0';

    word->w = new;
    free(old);

    return ret;
}

paragraph_t *sentencify(paragraph_t *paragraph)
{
    int sflag;
    sentence_t *sentence_it, *sentence_tmp;
    node_t *node_it, *start_it, *end_it, *node_tmp;
    word_t *word_it;

    sentence_it = paragraph->first_sentence;
    /* does the final line have a terminating character? */
    for ( ; sentence_it != NULL; sentence_it = sentence_it->next_sentence)
        for (node_it = sentence_it->first_word; next_node(node_it) != NULL; 
                node_it = next_node(node_it)) 
                        ;
        word_it = (word_t *) get_data(node_it);
        if (terminal_char(word_it) == 0) {
            add_terminal(word_it);
        }

    sentence_it = paragraph->first_sentence;
    for ( ; sentence_it != NULL; sentence_it = sentence_it->next_sentence) {

        node_it = sentence_it->first_word;

        sflag = 0;
        start_it = NULL;
        end_it = node_it;                   /* start of line */

        /* is there a sentence end-marker in the line? */
        for ( ; node_it != NULL; node_it = next_node(node_it)) {
            word_it = (word_t *) get_data(node_it);
            if (terminal_char(word_it) > 0) {
                /* is the end-marker at the end of the line? */
                if ((node_tmp = next_node(node_it)) != NULL) { 
                    start_it = next_node(node_it);
                    null_node(node_it);
                    break;
                } else {
                    sflag = 1;
                }
            }
        }
        /* was there a sentence ending */
        if (start_it == NULL && sflag == 0) {    /* no sentence ending */
            sentence_it->first_word = NULL;
            start_it = end_it;
        }

        if (sflag == 1)
            continue;

        /* insert start_it at start of next line */
        if (sentence_it->next_sentence != NULL) {   /* can insert */
            sentence_tmp = sentence_it->next_sentence;

            /* find the end node of the list pointed to by start_it */
            for (end_it = start_it; (node_tmp = next_node(end_it)) != NULL;
                    end_it = next_node(end_it))
                ;
            link_node(end_it, sentence_tmp->first_word);
            sentence_tmp->first_word = start_it;
        } else {
            sentence_tmp = add_sentence(paragraph);
            sentence_tmp->first_word = start_it;
        }
    }

    /* book keeping */
    paragraph = prune(paragraph);
    paragraph = recalc(paragraph);

    return paragraph;
}

static paragraph_t *recalc(paragraph_t *paragraph)
{
    unsigned index;
    sentence_t *sentence_it;
    node_t *node_it;

    index = 0;
    sentence_it = paragraph->first_sentence;
    for ( ; sentence_it != NULL; sentence_it = sentence_it->next_sentence) {
        node_it = sentence_it->first_word;

        /* calculate word_count per sentence */
        apply(node_it, size, (void *) &sentence_it->word_count);

        /* update sentence number (index) */
        sentence_it->number = index++;
    }
    paragraph->sentence_count = index;

    return paragraph;
}

unsigned sentence_index(sentence_t *sentence)
{
    return sentence->number;
}

unsigned sentence_total(paragraph_t *paragraph)
{
    return paragraph->sentence_count;
}

unsigned wordc(sentence_t *sentence)
{
    return sentence->word_count;
}

char *wort_to_string(word_t *word)
{
    return (char *) word->w;
}

static void delete_sentence_node(sentence_t *before, sentence_t *del)
{
    before->next_sentence = del->next_sentence;
    del->next_sentence = NULL;
    free(del);
}

static paragraph_t *prune(paragraph_t *paragraph)
{
    sentence_t *it, *tmp;

    tmp = it = paragraph->first_sentence;
    for ( ; it != NULL; it = tmp) {
        tmp = it->next_sentence;
        if (tmp != NULL && tmp->first_word == NULL)
            delete_sentence_node(it, tmp);
    }

    return paragraph;
}

static void psentence(node_t *first_word, void *arg)
{
    char *fmt;
    word_t *word;

    fmt = (char *) arg;

    word = (word_t *) get_data(first_word);
    printf(fmt, word->w);
}

paragraph_t *papply(paragraph_t *paragraph, void(*fn)(node_t *, void *), void *arg)
{
    sentence_t *it;
    
    it = paragraph->first_sentence;
    for ( ; it != NULL; it = it->next_sentence) {
        apply(it->first_word, fn, arg);
    }

    return paragraph;
}

void pparagraph(paragraph_t *paragraph)
{
    sentence_t *it;
    for (it = paragraph->first_sentence; it != NULL; 
            it = it->next_sentence) {
        apply(it->first_word, psentence, "%s ");
        printf("\n");
    }
}

static void delete_words(node_t *n, void *arg)
{
    word_t *tmp;
    tmp = (word_t *) get_data(n);

    /* debugging */
    /*printf("freeing: '%s'\n", tmp->w);*/

    free(tmp->w);
    free(tmp);
}

void paragraph_delete(paragraph_t *paragraph)
{
    sentence_t *sentence_it, *sentence_tmp;

    sentence_it = paragraph->first_sentence;

    for ( ; sentence_it != NULL; sentence_it = sentence_tmp) {
        sentence_tmp = sentence_it->next_sentence;
        /* free words */
        apply(sentence_it->first_word, delete_words, NULL);
        destroy_list(sentence_it->first_word);

        free(sentence_it);
    }

    free(paragraph);
}
