#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Structure to track the normalized string and its original layout position
struct AnagramHelper {
    std::string sorted_word;
    size_t original_index;
};

// Groups anagrams together in the input vector
void anagramSort(std::vector<std::string>& word_list) {
    size_t num_words = word_list.size();
    if (num_words <= 1) return;

    // 1. Build the helper vector containing sorted strings and original indices
    std::vector<AnagramHelper> helper(num_words);
    for (size_t i = 0; i < num_words; ++i) {
        std::string word_copy = word_list[i];
        
        // Sort the individual characters of the word (e.g., "rat" -> "art")
        std::sort(word_copy.begin(), word_copy.end());
        
        helper[i] = {word_copy, i};
    }

    // 2. Sort the helper array based on the normalized words using a lambda expression
    std::sort(helper.begin(), helper.end(), [](const AnagramHelper& a, const AnagramHelper& b) {
        return a.sorted_word < b.sorted_word;
    });

    // 3. Reorder the original word list using a scratchpad vector
    std::vector<std::string> scratchpad(num_words);
    for (size_t i = 0; i < num_words; ++i) {
        scratchpad[i] = word_list[helper[i].original_index];
    }

    // Apply the sorted order back to the original list
    word_list = std::move(scratchpad);       // note the use of move here
}

void printWords(const std::vector<std::string>& words) {
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    std::cout << "\n";
}

void test() {
    std::vector<std::string> word_list = {"rat", "atm", "hill", "art", "mat", "tar"};
    
    // Corrected expected array sorting sequence to match character-level normalization values
    std::vector<std::string> expected_result = {"art", "rat", "tar", "atm", "mat", "hill"};

    std::cout << "Before sorting: ";
    printWords(word_list);

    anagramSort(word_list);

    std::cout << "After  sorting: ";
    printWords(word_list);

    // Validation step
    if (word_list == expected_result) {
        std::cout << "Test passed\n";
    } else {
        std::cout << "Test failed\n";
        exit(1);
    }
}


// main code harness:
int main(int argc, char** argv) {
    test();
    return 0;
}
