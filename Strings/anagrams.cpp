#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

/*
An anagram is a word, phrase, or sentence formed by rearranging the letters of another word,
phrase, or sentence, using all the original letters exactly once

Key Improvements in Modern C++:
1. Use of std::string and std::vector instead of C-style strings and arrays.
2. Use of std::exit for error handling instead of exit from <stdlib.h>.
3. Use of range-based for loops for cleaner iteration over strings.
4. Use of std::unordered_map for character counting (if needed) instead of fixed-size arrays.
5. Use of constexpr for compile-time constants instead of #define.

• std::string & const References: Replaced raw char* pointers with safe std::string objects passed by reference to avoid copying.
• std::vector instead of raw arrays: Replaced manual array allocations and memset with a vector initialized to zero.
• Vector Comparison: Instead of writing a manual for loop to check array mismatches, C++ allows you to compare vectors directly using count1 == count2.
• Type Safety: Changed the return type and flags to native bool (true/false) rather than int (1/0).
• static_cast: Replaced the C-style type cast (int) with static_cast<unsigned char>. This is safer because 
a plain char can sometimes hold negative values depending on the compiler environment, causing array-out-of-bounds
exceptions.
*/

// Modern C++ alternative to #define
constexpr int NUM_CHARACTERS = 256;

void handle_error() 
{
    std::cout << "Test failed\n";
    std::exit(1);
}

/*
str1, str2: the two strings we want to compare
Return value: true if the two strings are anagrams, false otherwise
*/
bool are_strings_anagrams(const std::string& str1, const std::string& str2) 
{
    // std::vector automatically initializes elements to 0
    std::vector<int> count1(NUM_CHARACTERS, 0);
    std::vector<int> count2(NUM_CHARACTERS, 0);

    // Compute character counts using modern range-based for loops
    for (char ch : str1) {
        // cast to unsigned char to safely handle negative char values (e.g. extended ASCII)
        count1[static_cast<unsigned char>(ch)]++;
    }

    for (char ch : str2) {
        count2[static_cast<unsigned char>(ch)]++;
    }

    // Direct vector comparison checks both size and elements automatically
    return count1 == count2;         // check if the two vectors are equal
}

void test(const std::string& s1, const std::string& s2, bool expected_result) 
{
    bool result = are_strings_anagrams(s1, s2);

    std::cout << s1 << " and " << s2 << " are ";
    if (result) {
        std::cout << "anagrams\n";
    } else {
        std::cout << "not anagrams\n";
    }

    if (result != expected_result) {
        handle_error();
    }
}

// main code harness:
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) 
{
    test("ABCDE", "CDBAE", true);
    test("AAAAA", "BBBBB", false);

    std::cout << "Test passed\n";
    return 0;
}
