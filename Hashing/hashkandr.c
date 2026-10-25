#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

/* The node structure inside the bucket list */
struct nlist {
    struct nlist *next; /* next entry in chain */
    char *name;         /* defined name (key) */
    char *defn;         /* replacement text (value) */
};

/* The central pointer block array */
static struct nlist *hashtab[HASHSIZE]; 

/* Function prototypes */
unsigned hash(char *s);
struct nlist *lookup(char *s);
struct nlist *install(char *name, char *defn);
void free_table(void);


/* hash: form hash value for string s */
unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) {
            return np; /* found */
        }
    }
    return NULL; /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) { /* not found */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL) {
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { /* already there */
        free((void *) np->defn); /* free previous defn */
    }
    
    if ((np->defn = strdup(defn)) == NULL) {
        return NULL;
    }
    return np;
}

// Exercise 6-5 (undef): Write a function undef that will 
// remove a name and definition from the table maintained 
// by lookup and install

/* undef: remove a name and definition from the table */
int undef(char *name)
{
    struct nlist *np;
    struct nlist *prev = NULL;
    unsigned hashval = hash(name);

    /* Traverse the linked list at the specific hash bucket */
    for (np = hashtab[hashval]; np != NULL; prev = np, np = np->next) {
        if (strcmp(name, np->name) == 0) {
            /* Found it! Unlink the node */
            if (prev == NULL) {
                /* Node is at the head of the bucket list */
                hashtab[hashval] = np->next;
            } else {
                /* Node is inside or at the end of the bucket list */
                prev->next = np->next;
            }

            /* Free the dynamically allocated strings and structure memory */
            free((void *) np->name);
            free((void *) np->defn);
            free((void *) np);
            return 1; /* Successfully removed */
        }
    }
    return 0; /* Key was not found in the table */
}



/* Helper function to clear allocated memory and avoid leaks */
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


// main code harness:
int main(int argc, char** argv) 
{
    /* Insert some macro definitions */
    install("MAX", "100");
    install("PI", "3.14159");
    install("STATUS", "SUCCESS");
    
    /* Overwrite an existing definition */
    install("MAX", "200");

    /* Search for definitions using lookup */
    struct nlist *item;
    
    char *search_keys[] = {"MAX", "PI", "MIN"};
    int i;
    
    for (i = 0; i < 3; i++) {
        item = lookup(search_keys[i]);
        if (item != NULL) {
            printf("Found definition -> %s: %s\n", item->name, item->defn);
        } else {
            printf("Key '%s' is not defined.\n", search_keys[i]);
        }
    }

    free_table();
    return 0;
}
