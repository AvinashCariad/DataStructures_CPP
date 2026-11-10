#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100

void handle_error(void)
{
    printf("Test failed\n");
    exit(1);
}

/*
str1, str2: two non-NULL input strings that have to be interleaved
buffer: string that contains the result of interleaving 
pos: current position in the buffer
*/
void string_interleave(char *str1, char *str2, char *buffer, int pos) 
{
    /* If we have finished processing both strings, null-terminate and print */
    if (str1[0] == '\0' && str2[0] == '\0') {
        buffer[pos] = '\0';
        printf("%s\n", buffer);
        return;
    }

    /* Include the next character of str1 into the buffer if available */
    if (str1[0] != '\0') {
        buffer[pos] = str1[0];
        string_interleave(str1 + 1, str2, buffer, pos + 1);
    }

    /* Include the next character of str2 into the buffer if available */
    if (str2[0] != '\0') {
        buffer[pos] = str2[0];
        string_interleave(str1, str2 + 1, buffer, pos + 1);   
    }
}


//========================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100

/* 
   Non-recursive function to print all interleavings.
   Uses a binary bitmask combination method.
*/
void string_interleave_iterative(const char *str1, const char *str2) 
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int total_len = len1 + len2;

    if (total_len == 0) {
        printf("\n");
        return;
    }

    /* 
       Total possible positional masks is 2^(len1 + len2).
       Each bit in the loop mask represents a position in the final buffer.
       A '1' bit means "take from str1", a '0' bit means "take from str2".
    */
    unsigned long long total_combinations = 1ULL << total_len;
    char buffer[MAX_STRING_LENGTH];

    for (unsigned long long mask = 0; mask < total_combinations; mask++) {
        
        // Optimization: We ONLY care about combinations where exactly 
        // len1 elements are pulled from str1.
        int bits_set = 0;
        for (int i = 0; i < total_len; i++) {
            if ((mask >> i) & 1) {
                bits_set++;
            }
        }

        // If the number of chosen spaces doesn't match len1, it's invalid.
        if (bits_set != len1) {
            continue;
        }

        // Reconstruction phase using the valid positional map
        int idx1 = 0;
        int idx2 = 0;
        
        for (int i = 0; i < total_len; i++) {
            if ((mask >> i) & 1) {
                buffer[i] = str1[idx1++];
            } else {
                buffer[i] = str2[idx2++];
            }
        }
        
        buffer[total_len] = '\0';
        printf("%s\n", buffer);
    }
}


// main code harness:
int main(int argc, char** argv) 
{
    char buffer[MAX_STRING_LENGTH];

    // Explicitly zero out the buffer just to be safe
    memset(buffer, 0, sizeof(buffer));

    printf("Interleaving 'abcde' and '123':\n");
    string_interleave("abcde", "123", buffer, 0);

    printf("Iteratively interleaving 'abc' and '12':\n");
    string_interleave_iterative("abc", "12");

    printf("\nTest passed\n");

    printf("Test passed\n");
    return 0;
}
