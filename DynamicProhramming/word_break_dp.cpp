/**
 * @file
 * @brief [Word Break Problem](https://leetcode.com/problems/word-break/)
 */

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace dynamic_programming {
namespace word_break {

/**
 * @brief Function that checks if the string passed in param is present in the unordered_set
 */
bool exists(const std::string &str, const std::unordered_set<std::string> &strSet) {
    return strSet.find(str) != strSet.end();
}

/**
 * @brief Checks if the string can be segmented starting from position 'pos'
 * @details Passed dp by reference directly rather than raw pointer for cleaner modern syntax
 */
bool check(const std::string &s, const std::unordered_set<std::string> &strSet,
           int pos, std::vector<int> &dp) {
    if (pos == s.length()) {
        return true;
    }

    // -1 represents unvisited/uncomputed positions
    if (dp[pos] != -1) {
        return dp[pos] == 1;
    }

    std::string wordTillNow = ""; 

    for (int i = pos; i < s.length(); i++) {
        // OPTIMIZATION: Appending char directly instead of generating a 1-char std::string object
        wordTillNow += s[i];  

        if (exists(wordTillNow, strSet) && check(s, strSet, i + 1, dp)) {
            dp[pos] = 1;
            return true;
        }
    }
    
    dp[pos] = 0;  
    return false;     
}

/**
 * @brief Word Break solver initializer
 */
bool wordBreak(const std::string &s, const std::vector<std::string> &wordDict) {
    // Collect words into an unordered_set for O(1) lookups - note: Using std::unordered_set instead of std::set for average O(1) lookup time
    std::unordered_set<std::string> strSet(wordDict.begin(), wordDict.end());    
    
    // Initialize memoization array: -1 means unvisited
    std::vector<int> dp(s.length(), -1);        // check how the dp has been initialized

    return check(s, strSet, 0, dp);
}

}  // namespace word_break
}  // namespace dynamic_programming

/**
 * @brief Test implementations
 */
static void test() {
    const std::string s = "applepenapple";
    const std::vector<std::string> wordDict = {"apple", "pen"};

    assert(dynamic_programming::word_break::wordBreak(s, wordDict));

    std::cout << std::boolalpha 
              << "Test assertion output: " 
              << dynamic_programming::word_break::wordBreak(s, wordDict)
              << "\nTest implementation passed!\n";
}

/**
 * @brief Main function
 */
int main(int argc, char** argv) {
    test();  

    const std::string s = "applepenapple";
    const std::vector<std::string> wordDict = {"apple", "pen"};

    std::cout << "Main execution output: " 
              << dynamic_programming::word_break::wordBreak(s, wordDict)
              << std::endl;
              
    return 0;
}
