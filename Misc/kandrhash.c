#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

/* Data Structure for Table Entries */
struct nlist {
    struct nlist *next;     /* next entry in chain */
    char *name;             /* defined name (key) */
    char *defn;             /* replacement text (value) */
};

/* The Hash Table Array */
static struct nlist *hashtab[HASHSIZE]; 

/* ----------------------------------------------------
 * HELPER FUNCTIONS
 * ---------------------------------------------------- */

/* Helper function to duplicate strings and allocate exact memory */
char *my_strdup(char *s) {
    char *p;
    p = (char *) malloc(strlen(s) + 1); /* +1 for '\0' */
    if (p != NULL) {
        strcpy(p, s);
    }
    return p;
}

/* ----------------------------------------------------
 * CORE HASH TABLE CORE METHODS
 * ---------------------------------------------------- */

/* hash: form hash value for string s */
unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s) {
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) {
            return np; /* found */
        }
    }
    return NULL; /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;
    
    if ((np = lookup(name)) == NULL) { /* not found, create new node */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = my_strdup(name)) == NULL) {
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval]; /* insert at front of chain */
        hashtab[hashval] = np;
    } else { /* already exists, free previous definition */
        free((void *) np->defn);              // key remains the same, replace with the new value 
    }
    
    if ((np->defn = my_strdup(defn)) == NULL) {
        return NULL;
    }
    return np;
}

/* ----------------------------------------------------
 * EXTRA: UTILITY METHODS FOR MANAGEMENT
 * ---------------------------------------------------- */

/* display: print current non-empty table entries for debugging */
void display_table() {
    printf("\n--- Current Hash Table State ---\n");
    for (int i = 0; i < HASHSIZE; i++) {
        if (hashtab[i] != NULL) {
            printf("Bucket [%d]: ", i);
            struct nlist *np = hashtab[i];
            while (np != NULL) {
                printf("(%s -> %s) ", np->name, np->defn);
                np = np->next;
            }
            printf("\n");
        }
    }
    printf("--------------------------------\n\n");
}

/* free_table: clear all nodes from memory to prevent leaks */
void free_table() {
    struct nlist *np, *tmp;
    for (int i = 0; i < HASHSIZE; i++) {
        np = hashtab[i];
        while (np != NULL) {
            tmp = np->next;
            free(np->name);
            free(np->defn);
            free(np);
            np = tmp;
        }
        hashtab[i] = NULL;
    }
}

/* ----------------------------------------------------
 * MAIN EXECUTION DRIVER
 * ---------------------------------------------------- */
int main(int argc, char** argv) {
    /* 1. Insert initial configurations */
    install("MAX_WIDTH", "1920");
    install("MAX_HEIGHT", "1080");
    install("VERSION", "2.4.1");
    install("AUTHOR", "Kernighan & Ritchie");

    /* 2. Display inserted values */
    display_table();

    /* 3. Overwrite an existing configuration key */
    printf("Updating VERSION to '3.0.0'...\n");
    install("VERSION", "3.0.0");

    /* 4. Query and verify results */
    char *test_keys[] = {"VERSION", "MAX_WIDTH", "INVALID_KEY"};
    for (int i = 0; i < 3; i++) {
        struct nlist *result = lookup(test_keys[i]);
        if (result != NULL) {
            printf("Lookup Success: Key '%s' equals '%s'\n", result->name, result->defn);
        } else {
            printf("Lookup Failed: Key '%s' not found.\n", test_keys[i]);
        }
    }

    /* 5. Clean up allocated heap memory */
    free_table();
    return 0;
}

