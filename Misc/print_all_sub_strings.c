#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
str1: string whose substrings should be printed
*/
void print_all_sub_strings(char *str1)
{
    int  i, j, pos, n;

    n = strlen(str1);
    
    /*Generate all pairs (i,j) where i <= j*/
    for (i = 0; i < n; ++i) {
        for (j = i; j < n; ++j) {
            
            /*print the substring string[i] to string[j]*/
            for (pos = i; pos <= j; ++pos)  {
                printf("%c", str1[pos]);         // note this is the most important part of the code
            }
            printf("\n");
        }
    }
}



// main code harness
int main(int argc, char** argv)
{
    print_all_sub_strings("ABCDE");

    printf("Test passed\n");
    return 0;
}

