#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASHSIZE 101
#define MAXWORD  100

/* Hash Table Structures and Declarations */
struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};

static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s);
struct nlist *lookup(char *s);
struct nlist *install(char *name, char *defn);
int undef(char *name);
void free_table(void);

/* Preprocessor Input Parsing Declarations */
int getword(char *word, int lim);
int get_line_rest(char *buf, int lim);



/* getword: fetch next word or character from standard input */
int getword(char *word, int lim)
{
    int c;
    char *w = word;

    /* Read lookahead character */
    c = getchar();
    if (c == EOF) {
        return EOF;
    }

    *w++ = c;

    /* If it is not alphanumeric or an underscore/hashtag, return it as a single token */
    if (!isalnum(c) && c != '_' && c != '#') {
        *w = '\0';
        return c;
    }

    /* Keep accumulating characters up to the buffer limit */
    for ( ; --lim > 1; w++) {
        c = getchar();
        if (!isalnum(c) && c != '_') {
            ungetc(c, stdin); /* Push non-word token boundaries back to input stream */
            break;
        }
        *w = c;
    }
    *w = '\0';
    return word[0];
}

/* get_line_rest: swallow spaces and capture the remainder of a #define string line */
int get_line_rest(char *buf, int lim) 
{
    int c;
    char *b = buf;

    /* Skip leading whitespace tabs/spaces immediately following the macro name */
    while ((c = getchar()) == ' ' || c == '\t')
        ;

    if (c == EOF || c == '\n') {
        return 0;
    }

    *b++ = c;

    /* Read until a newline or end-of-file boundary breaks the definition assignment */
    while (--lim > 1 && (c = getchar()) != EOF && c != '\n') {
        *b++ = c;
    }
    *b = '\0';
    return 1;
}

/* --- Core K&R Hash Table Implementations --- */

unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

struct nlist *lookup(char *s)
{
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) {
            return np;
        }
    }
    return NULL;
}

struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) {
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL) {
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else {
        free((void *) np->defn);
    }
    
    if ((np->defn = strdup(defn)) == NULL) {
        return NULL;
    }
    return np;
}

void free_table(void) 
{
    struct nlist *np, *next_node;
    int i;
    for (i = 0; i < HASHSIZE; i++) {
        np = hashtab[i];
        while (np != NULL) {
            next_node = np->next;
            free(np->name);
            free(np->defn);
            free(np);
            np = next_node;
        }
        hashtab[i] = NULL;
    }
}



int main(void) 
{
    char word[MAXWORD];
    struct nlist *np;

    printf("--- K&R Macro Preprocessor Running Mini-Simulation ---\n");
    printf("Type lines of code below. Example:\n");
    printf("  #define MAX 100\n");
    printf("  #define GREETING Hello_World\n");
    printf("  MAX points to GREETING\n\n");

    /* Main processing loop */
    while (getword(word, MAXWORD) != EOF) {
        if (strcmp(word, "#define") == 0) {
            char name[MAXWORD];
            char defn[MAXWORD];

            /* 1. Read the macro name */
            if (getword(name, MAXWORD) != EOF && isalpha(name[0])) {
                /* 2. Read the remainder of the line as the macro text value */
                if (get_line_rest(defn, MAXWORD)) {
                    install(name, defn);
                }
            }
        } 
        else if (isalpha(word[0])) {
            /* Look up alphanumeric tokens to see if they match stored macros */
            if ((np = lookup(word)) != NULL) {
                printf("%s", np->defn);
            } else {
                printf("%s", word);
            }
        } 
        else {
            /* Pass single characters like symbols, whitespace, and newlines straight to stdout */
            printf("%s", word);
        }
    }

    free_table();
    return 0;
}