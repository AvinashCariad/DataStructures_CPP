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


// main code harness:
int main(int argc, char** argv) {
    string str = "abc";
    
    // Kick off the recursion with an empty permutation prefix
    printPermutationsRecursive(str, "");

    return 0;
}