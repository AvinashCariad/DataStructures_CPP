#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

int longestDictWordLength(const unordered_set<string>& dict) {
    int max_length = 0;
    for(const auto& word: dict) {
        max_length = max(max_length, (int)word.size());
    }
    return max_length;
}

vector<string> canStringBeDecomposed(string str, const unordered_set<string>& dict) {
    int n = str.size();
    if (n == 0) return {};

    // dp[i] is true if str[0...i-1] can be segmented
    vector<bool> dp(n + 1, false);
    dp[0] = true; // Base case: empty string is valid

    // parent_length[i] stores the length of the word that ends at index i-1
    vector<int> parent_length(n + 1, -1);
    
    int longest_word_len = longestDictWordLength(dict);
    
    // Outer loop moves across the length of the prefix evaluated
    for(int i = 1; i <= n; i++) {
        // Optimization: Look back at most 'longest_word_len' steps
        int start_j = max(0, i - longest_word_len);
        
        for(int j = i - 1; j >= start_j; j--) {
            if (dp[j]) {
                string suffix = str.substr(j, i - j);
                if (dict.find(suffix) != dict.end()) {
                    dp[i] = true;
                    parent_length[i] = i - j; // Track the word length
                    break; // Move to next i once a valid split path is established
                }
            }
        }
    }

    // If the entire string cannot be segmented, return an empty vector
    if (!dp[n]) return {};

    // Reconstruct the words from back to front
    vector<string> words;
    int curr = n;
    while(curr > 0) {
        int word_len = parent_length[curr];
        words.push_back(str.substr(curr - word_len, word_len));
        curr -= word_len;
    }

    // Reverse the vector to restore original sentence word order
    reverse(words.begin(), words.end());
    return words;
}

// Fixed: Added 'return out;' and passed vector by const reference
ostream& operator<<(ostream& out, const vector<string>& arr) {
    for(const auto& a: arr)
        out << a << " ";
    out << endl;
    return out; 
}


// main code harness:
int main(int argc, char** argv) {
    unordered_set<string> dict = {"mobile","samsung","sam","sung", 
                            "man","mango","icecream","and", 
                             "go","i","like","ice","cream", "jam", 
                             "painting", "paint","inga","park", "lake"};
    string str = "ilikepaintinga";

    // Outputs the correct LIFO matched result: i like paint inga 
    cout << canStringBeDecomposed(str, dict);    
    return 0;
}

