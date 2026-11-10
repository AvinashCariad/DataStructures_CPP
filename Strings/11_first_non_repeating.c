

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_error()
{
    printf("Error occured\n");
    exit(1);
}


#define MAX_NUM_CHARS   (256)



/*
str1: non-NULL character array to be searched
Return value: first unique character if it exists, '\0' otherwise
*/
char find_first_unique_char(char *str1)
{
    int i, count[MAX_NUM_CHARS];
    int length = strlen(str1);
    char cur_char, first_unique_char;

    for (i = 0; i < MAX_NUM_CHARS; ++i)
        count[i] = 0;

    /*count the number of occurences of each character*/
    for (i = 0; i < length; ++i) {
        cur_char = str1[i];
        count[(int)cur_char]++;
    }

    /*traverse str1 and find first character which occurs only once*/
    first_unique_char = 0;
    for (i = 0; i < length; ++i) {
        cur_char = str1[i];
        if (count[(int)cur_char] == 1) {
            first_unique_char = cur_char;
            break;
        }
    }

    return first_unique_char;
}


void test(char *str1, char expected_result)
{
    char result = find_first_unique_char(str1);
    
    printf("First unique in \"%s\" is %c\n", str1, result); 

    if (result != expected_result)
        handle_error();
}


// main code harness:
int main(int argc, char** argv)
{

    test("aabbbccccddeffffgg", 'e');
    test("abcdefab", 'c');

    printf("Test passed\n");
    return 0;
}
