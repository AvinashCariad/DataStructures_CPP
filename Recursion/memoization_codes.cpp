#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

//A naive recursive Fibonacci takes \(O(2^n)\) time because it recalculates the same subproblems repeatedly.
// Storing results in a std::vector reduces the time complexity to O(n).
long long fib(int n, vector<long long>& memo) {
    // Base cases
    if (n <= 1) return n;

    // 1. Check if the result is already cached
    if (memo[n] != -1) {
        return memo[n];
    }

    // 2. Compute, store in cache, and return
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo);
    return memo[n];
}



// When a recursive function depends on two changing parameters—such as (row, col)
// coordinates on a grid—use a 2D vector to cache the state. This drops the runtime from exponential to O(rows × cols).

// Count paths from (r, c) to bottom-right corner moving only Right or Down
int countPaths(int r, int c, int rows, int cols, vector<vector<int>>& memo) {
    // Out of bounds
    if (r >= rows || c >= cols) return 0;

    // Reached destination
    if (r == rows - 1 && c == cols - 1) return 1;

    // Return cached value if already computed
    // Not visited- already initialized to -1
    if (memo[r][c] != -1) {
        return memo[r][c];
    }

    // Cache sum of moving down and moving right
    memo[r][c] = countPaths(r + 1, c, rows, cols, memo) +
                 countPaths(r, c + 1, rows, cols, memo);
    return memo[r][c];
}



// Example: Recursive Word Break check with string memoization
bool canConstruct(const string& target, const vector<string>& wordBank, unordered_map<string, bool>& memo) {
    if (target.empty()) return true;

    // 1. Look up the current target string in the hash map
    if (memo.count(target)) {          // looks for keys in the map
        return memo[target];
    }

    for (const string& word : wordBank) {
        // Check if target starts with 'word'
        if (target.rfind(word, 0) == 0) {
            string suffix = target.substr(word.length());
            if (canConstruct(suffix, wordBank, memo)) {
                memo[target] = true;
                return true;
            }
        }
    }

    memo[target] = false;
    return false;
}



// main code harness:
int main(int argc, char** argv) {

    int n = 45;
    // Initialize cache with -1 (meaning "uncomputed")
    vector<long long> memo(n + 1, -1);

    cout << "Fib(" << n << ") = " << fib(n, memo) << "\n";
    

    int rows = 15, cols = 15;
    vector<vector<int>> memo2(rows, vector<int>(cols, -1));

    cout << "Unique paths: " << countPaths(0, 0, rows, cols, memo2) << "\n";
    

    unordered_map<string, bool> memo1;
    vector<string> words = {"e", "ee", "eee", "eeee", "eeeee"};
    string target = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeef"; // Worst-case backtracking trap

    cout << boolalpha << "Can construct: " << canConstruct(target, words, memo1) << "\n";
    return 0;
}
