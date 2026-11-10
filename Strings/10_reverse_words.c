

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING_LENGTH 100

void handle_error(void)
{
    printf("Test failed\n");
    exit(1);
}


/*Helper function which reverses a string between indexes low and high
str1: string which needs to be reversed
low: lower index of region to be reversed
high: higher index of region to be reversed
*/
void reverse_string(char str1[], int low, int high)
{
    char temp;

    while (low < high) {
        temp = str1[low];
        str1[low] = str1[high];
        str1[high] = temp;
        low++;
        high--;
    }
}

/*Helper function which checks if a character is an alphabet(a-zA-Z)*/
int is_alphabet(char c)
{
    if (c >= 'A' && c <= 'Z')
        return 1;

    if (c >= 'a' && c <= 'z')
        return 1;

    return 0;       
}


/*Main function to reverse the words in a string
str1: the string in which the words have to be reversed
*/
void reverse_words(char str1[])
{
    int length, low, high, pos;
    
    length = strlen(str1);
    if (length < 2)
        return;

    /*Reverse the entire string*/
    reverse_string(str1, 0, length - 1);

    /*Reverse the individual words in the string*/
    pos = 0;
    while (pos < length) {
        if (is_alphabet(str1[pos])) {
            low = pos;
            while (pos < length && is_alphabet(str1[pos])) {
                pos++;
            }
            high = pos - 1;
            reverse_string(str1, low, high);
        } else {
            pos++;
        }   
    }
}


void test(char str1[], char expected_result[])
{
    printf("%s ", str1);

    reverse_words(str1);

    printf("=>  %s\n", str1);

    if (strcmp(str1, expected_result) != 0)
        handle_error();

}



// main code harness:
int main(int argc, char** argv)
{
    char str1[MAX_STRING_LENGTH];

    str1[0] = 0;    
    test(str1, "");

    strcpy(str1, "a");
    test(str1, "a");

    strcpy(str1, "Hello how are you");
    test(str1, "you are how Hello");


    printf("Test passed\n");
    return 0;
}





/*

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

// Main function to reverse the words in a string using idiomatic C++
void reverse_words(std::string& str) {
    if (str.length() < 2) {
        return;
    }

    // 1. Reverse the entire string using built-in C++ standard algorithms
    std::reverse(str.begin(), str.end());

    // 2. Reverse the individual words back
    size_t pos = 0;
    size_t length = str.length();

    while (pos < length) {
        // Find the start of a word (alphabetic character)
        if (std::isalpha(static_cast<unsigned char>(str[pos]))) {
            size_t low = pos;
            
            // Scan to find the end of the word
            while (pos < length && std::isalpha(static_cast<unsigned char>(str[pos]))) {
                pos++;
            }
            size_t high = pos; // standard library ranges are [exclusive) end boundaries

            // Reverse only this word segment
            std::reverse(str.begin() + low, str.begin() + high);
        } else {
            pos++;
        }
    }
}

void handle_error() {
    std::cerr << "Test failed\n";
    std::exit(1);
}

void test(std::string str, const std::string& expected_result) {
    std::cout << "\"" << str << "\" ";

    reverse_words(str);

    std::cout << "=>  \"" << str << "\"\n";

    if (str != expected_result) {
        handle_error();
    }
}

// main code harness:
int main(int argc, char** argv) {
    std::string str1;

    str1 = "";
    test(str1, "");

    str1 = "a";
    test(str1, "a");

    str1 = "Hello how are you";
    test(str1, "you are how Hello");

    std::cout << "Test passed\n";
    return 0;
}







*/