#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_error() {
    printf("Test failed \n");
    exit(1);
}

struct AnagramHelper {
    char *word;
    int index;
};

void print_helper(struct AnagramHelper helper[], int num_words) {
    int i;
    for (i = 0; i < num_words; ++i) {
        printf("[ %s , %d] ", helper[i].word, helper[i].index);
    }
    printf("\n");
}

/* Helper function for comparing characters. Used during quicksort */
int char_cmp_function(const void *p1, const void *p2) {
    char x = *(const char*)p1;
    char y = *(const char*)p2;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Sorts the characters in a string */
void sort_word(char buffer[], int length) {
    qsort(buffer, length, sizeof(char), char_cmp_function);
}

/* Helper function for comparing words. Used during quicksort */
int helper_cmp_function(const void *p1, const void *p2) {
    const struct AnagramHelper *x = (const struct AnagramHelper*)p1;
    const struct AnagramHelper *y = (const struct AnagramHelper*)p2;
    return strcmp(x->word, y->word);
}

/* Sorts the anagram helper array based on the normalized words in it */
void sort_anagram_helper(struct AnagramHelper helper[], int num_words) {
    qsort(helper, num_words, sizeof(struct AnagramHelper), helper_cmp_function);
}

/*
word_list: list of words sorted so that anagrams occur together
num_words: Number of elements in the word_list
*/
void anagram_sort(char *word_list[], int num_words) {
    struct AnagramHelper *helper = (struct AnagramHelper*) calloc(num_words, sizeof(struct AnagramHelper));
    int i, length;
    char **scratchpad;

    for (i = 0; i < num_words; ++i) {
        length = strlen(word_list[i]);

        /* Allocate memory for the word in the helper and copy original */
        helper[i].word = (char*) calloc(length + 1, sizeof(char));
        strcpy(helper[i].word, word_list[i]);       
        
        /* First sort the characters of the word in the helper */
        sort_word(helper[i].word, length);
        
        /* Store the original index of the word in the helper */
        helper[i].index = i;
    }

    /* Sort all the words in the helper */
    sort_anagram_helper(helper, num_words);
    
    /* Reorder pointers using our safe scratchpad approach */
    scratchpad = (char**) calloc(num_words, sizeof(char *));

    for (i = 0; i < num_words; ++i) {
        int index = helper[i].index;
        scratchpad[i] = word_list[index];
    }

    for (i = 0; i < num_words; ++i) {
        word_list[i] = scratchpad[i];
    }
    
    /* Clean up memory: free sub-allocations first to prevent leaks */
    // note this is easy to forget:
    for (i = 0; i < num_words; ++i) {
        free(helper[i].word);
    }
    
    free(scratchpad);
    free(helper);
}

void print_words(char *words[], int num_words) {
    int i;
    for (i = 0; i < num_words; ++i) {
        printf("%s ", words[i]);
    }
    printf("\n");
}

void test() {
    int i;
    int num_words = 6;
    char *word_list[] = { "rat", "atm", "hill", "art", "mat", "tar"};
    
    // Corrected to reflect alphabetical outcomes of normalized words:
    // "art" -> "art", "atm"/"mat" -> "amt", "rat"/"tar" -> "art", "hill" -> "hill"
    char *expected_result[] = {"art", "rat", "tar", "atm", "mat", "hill"}; 

    printf("Before sorting: ");
    print_words(word_list, num_words);  

    anagram_sort(word_list, num_words);

    printf("After  sorting: ");
    print_words(word_list, num_words);  

    for (i = 0; i < num_words; ++i) {
        if (strcmp(word_list[i], expected_result[i]) != 0) {
            handle_error();
        }
    }
}


// main code harnes:
int main(int argc, char** argv) {
    test();
    printf("Test passed\n");
    return 0;
}




/*

































*/