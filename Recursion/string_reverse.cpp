#include <iostream>
#include <string>
#include <utility> // For std::swap


using namespace std;

// reverse string:
string revString(string str)
{
    // Fixed Base Case: Handle both empty strings and the end of the recursion safely
    if (str.empty())
    {
        return str;
    }

    char currChar = str[0];
    
    // Extract the substring from index 1 to the end (using the substr rule we discussed!)
    string nextString = revString(str.substr(1));
    
    // Append the current character to the end of the reversed remaining string
    return nextString + currChar;
}


// Optimized:
// Pass by reference (&) to avoid creating copies of the string
void revStringInPlace(string &str, int start, int end)
{
    // Base case: Pointers have met or crossed
    if (start >= end) {
        return;
    }

    // Swap the characters at the boundary positions
    swap(str[start], str[end]);

    // Recursive step: Move boundaries inward
    revStringInPlace(str, start + 1, end - 1);
}


// main code harness:
int main(int argc, char** argv)
{
    string str = "avinash";
    string reversed = revString(str);
    //revStringInPlace(str, 0, str.length() - 1);
    cout << "Reversed string: " << reversed << endl; // Output: dcba
    cout << std::endl;

    return 0;
}
