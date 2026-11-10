#include <iostream>
#include <string_view>
#include <cctype>
#include <cstdlib>

// Terminate the program cleanly with a failure message
void handle_error() {
    std::cerr << "Test failed\n";
    std::exit(EXIT_FAILURE);
}

/*
 * Counts the number of alphabetical words in a string.
 * A new word is detected whenever a non-alphabet character transitions to an alphabet character.
 */
int count_words(std::string_view str) {
    int num_words = 0;
    bool is_prev_char_alphabet = false;

    for (const char c : str) {
        // Use standard library check for alphabetical characters (cast to unsigned char safely)
        bool is_cur_char_alphabet = std::isalpha(static_cast<unsigned char>(c));       // Check if the current character is an alphabet

        if (!is_prev_char_alphabet && is_cur_char_alphabet) {
            ++num_words;
        }

        is_prev_char_alphabet = is_cur_char_alphabet;
    }

    return num_words;
}

// Test harness to validate results
void test(std::string_view str, int expected_result) {
    int result = count_words(str);

    if (result != expected_result) {
        handle_error();
    }

    std::cout << "Word count = " << result << ", for string: \"" << str << "\"\n";
}


// main code harness:
int main(int argc, char** argv) {
    test("hello", 1);
    test("", 0);
    test("   hello,how are    you doing?", 5);

    std::cout << "Test passed\n";
    return EXIT_SUCCESS;
}
