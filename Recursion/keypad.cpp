#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Recursive helper function
void solve(string digits, string output, int index, const vector<string>& mapping) {
    // Base case: if index reaches the end of digits string, print the result
    if (index == digits.length()) {
        cout << output << " ";
        return;
    }

    // Get the characters corresponding to the current digit
    int digit = digits[index] - '0';
    string value = mapping[digit];

    // Traverse all characters for this digit and make recursive calls
    for (int i = 0; i < value.length(); i++) {
        solve(digits, output + value[i], index + 1, mapping);
    }
}



// print all the key combinations:
void printKeypadCombinations(string digits) {
    if (digits.length() == 0) return;

    // Keypad mapping from 0 to 9
    vector<string> mapping = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };

    solve(digits, "", 0, mapping);
}

int main() {
    string digits = "23";
    cout << "Combinations for " << digits << ": ";
    printKeypadCombinations(digits);
    cout << endl;
    return 0;
}

