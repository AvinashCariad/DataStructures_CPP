#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class Solution
{
public:

    bool isAnagram(string s, string t)
    {
        // Different lengths can never be anagrams
        if (s.length() != t.length())
            return false;

        sort(s.begin(), s.end());
        sort(t.begin(), t.end());

        return s == t;
    }
};


// main code harness:
int main(int argc, char** argv)
{
    Solution sol;

    string s = "anagram";
    string t = "nagaram";

    if (sol.isAnagram(s, t))
        cout << "Anagram\n";
    else
        cout << "Not Anagram\n";

    string s2 = "rat";
    string t2 = "car";

    if (sol.isAnagram(s2, t2))
        cout << "Anagram\n";
    else
        cout << "Not Anagram\n";

    return 0;
}
