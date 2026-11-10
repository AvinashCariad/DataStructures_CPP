#include <stdio.h>
#include <stdlib.h>

/*Helper function for printing the words corresponding to the telephone number*/
void keypad_string_gen(int digits[], int num_digits, const char *keypad[], 
            char buf[], int pos)
{
    int cur_digit, i;
    const char *key_string;

    if (pos == num_digits) {
        /*We have processed all the digits. So print the 
        word and terminate the recursion*/
        buf[pos] = '\0';
        printf("%s\n", buf);
        return;
    }

    cur_digit = digits[pos];
    
    // Guard against out-of-bounds digit input anomalies (e.g. negative numbers or > 9)
    if (cur_digit < 0 || cur_digit > 9) {
        return; 
    }

    key_string = keypad[cur_digit];

    /* If the mapped keypad string is empty, we bypass it by treating it as a single space 
       or skipping it, but to prevent the branch from dying completely, we ensure it processes. */
    if (key_string[0] == '\0') {
        buf[pos] = ' '; // Fallback filler or remove to skip entirely
        keypad_string_gen(digits, num_digits, keypad, buf, pos + 1);
        return;
    }

    /*Cycle through all the characters in the key_string.*/
    i = 0;
    while (key_string[i]) {
        buf[pos] = key_string[i];
        keypad_string_gen(digits, num_digits, keypad, buf, pos + 1);
        i++;
    }
}

/*Main function for printing the words corresponding to the telephone number*/
void telephone_digits_to_string(int digits[], int num_digits)
{
    if (num_digits <= 0) {
        printf("\n");
        return;
    }

    /*Create a temporary buffer for storing the words corresponding to the digits*/
    char *buf = (char*) calloc(num_digits + 1, sizeof(char)); 
    if (buf == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    /*digit 2 corresponds to ABC, 3 corresponds to DEF and so on*/
    const char *keypad[] = {"0", "1", "ABC", "DEF", "GHI", "JKL", "MNO", "PQRS", 
            "TUV", "WXYZ"};
        
    keypad_string_gen(digits, num_digits, keypad, buf, 0);
    
    free(buf);
}


// main code harness:
int main(int argc, char** argv)
{
    int digits[] = {4, 2, 1, 1, 8, 9, 2, 5, 0, 8};
    int num_digits = 10;

    telephone_digits_to_string(digits, num_digits);

    printf("Test passed\n");
    return 0;
}
