#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

const int MAX_STRING_LENGTH = 100;
const int MAX_NUM_CHARACTERS = 256;

void handle_error(void) {
    std::cout << "Test failed\n";
    std::exit(1);
}

/* str1: string from which duplicate characters should be removed */
void remove_duplicates(char *str1) {
    if (!str1) return;
    int length = std::strlen(str1);
    if (length < 2) return;

    std::vector<char> was_char_observed(MAX_NUM_CHARACTERS, 0);
    
    char c = str1[0];
    was_char_observed[static_cast<unsigned char>(c)] = 1;
    int fill_pos = 1;

    for (int i = 1; i < length; ++i) {
        c = str1[i];
        /* Only if current character was not observed so far, add the current character to fill position and advance fill position */
        if (!was_char_observed[static_cast<unsigned char>(c)]) {
            str1[fill_pos] = c;
            fill_pos++;
        }
        was_char_observed[static_cast<unsigned char>(c)] = 1;
    }

    /* Insert the termination \0 at the end of str1 */
    str1[fill_pos] = '\0';
}

void test(char str1[], const char expected_result[]) {
    std::cout << str1;
    remove_duplicates(str1);
    std::cout << " => " << str1 << '\n';
    if (std::strcmp(str1, expected_result) != 0) {
        handle_error();
    }
}

int main(int argc, char** argv) {
    char str1[MAX_STRING_LENGTH];
    
    // Test 1: Empty string
    str1[0] = '\0';
    test(str1, "");

    // Test 2: Single character
    std::strcpy(str1, "a");
    test(str1, "a");

    // Test 3: Repeated characters
    std::strcpy(str1, "1aaaaaa2bbbaaaaa3cccccaaaabbb");
    test(str1, "1a2b3c");

    std::cout << "Test passed\n";
    return 0;
}
