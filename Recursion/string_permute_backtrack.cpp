#include <iostream>
#include <string>
#include <utility> // For std::swap




using namespace std;

/**
 * Generates all permutations of a string in-place.
 * 
 * @param str The original string (passed by reference to avoid copying)
 * @param index The current starting index for swapping
 */
void printPermutationsInPlace(string& str, int index) {
    // Base Case: If we reach the end of the string, we found a full permutation
    if (index >= str.length()) {
        cout << str << "\n";
        return;
    }

    for (int i = index; i < str.length(); i++) {
        // Step 1: Swap the current index character with the i-th character
        swap(str[index], str[i]);

        // Step 2: Recurse for the remaining substring
        printPermutationsInPlace(str, index + 1);

        // Step 3: Backtrack (Undo the swap to restore the original string state)
        swap(str[index], str[i]);
    }
}


// main code harness:
int main(int argc, char** argv) {
    string str = "abc";
    
    // Start backtracking from index 0
    printPermutationsInPlace(str, 0);

    return 0;
}




/*   ====================Recursive Implementation of the above code========================

#include <iostream>
#include <string>

using namespace std;


 // Recursively prints all permutations of a string by building 
 // a new permutation string character by character.
 
void printPermutationsRecursive(string str, string permutation) {
    // Base Case: When the remaining string is empty, we've formed a full permutation
    if (str.length() == 0) {
        cout << permutation << "\n";
        return;
    }

    // Branching: Try picking every available character to be the next in the permutation
    for (size_t i = 0; i < str.length(); i++) {
        char currChar = str[i];
        
        // Form a new string excluding the character we just picked
        string remainingStr = str.substr(0, i) + str.substr(i + 1);
        
        // Recurse with the reduced string and updated permutation path
        printPermutationsRecursive(remainingStr, permutation + currChar);
    }
}

int main() {
    string str = "abc";
    
    // Kick off the recursion with an empty permutation prefix
    printPermutationsRecursive(str, "");

    return 0;
}





















*/