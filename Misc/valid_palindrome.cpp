#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class Solution
{
public:

    bool isPalindrome(string s)
    {
        int left = 0;
        int right = s.length() - 1;

        while (left < right)
        {
            // Skip non-alphanumeric characters
            while (left < right &&
                   !isalnum(static_cast<unsigned char>(s[left])))     // note the casting with char
            {
                left++;
            }

            while (left < right &&
                   !isalnum(static_cast<unsigned char>(s[right])))    // note the casting here 
            {
                right--;
            }

            // Compare ignoring case
            if (tolower(static_cast<unsigned char>(s[left])) !=
                tolower(static_cast<unsigned char>(s[right])))
            {
                return false;
            }

            left++;
            right--;
        }

        return true;
    }
};


// main code harness:
int main(int argc, char** argv)
{
    Solution sol;

    string s1 = "A man, a plan, a canal: Panama";
    string s2 = "race a car";
    string s3 = "Madam";
    string s4 = "12321";
    string s5 = "No 'x' in Nixon";

    cout << boolalpha;

    cout << "\"" << s1 << "\" -> "
         << sol.isPalindrome(s1) << endl;

    cout << "\"" << s2 << "\" -> "
         << sol.isPalindrome(s2) << endl;

    cout << "\"" << s3 << "\" -> "
         << sol.isPalindrome(s3) << endl;

    cout << "\"" << s4 << "\" -> "
         << sol.isPalindrome(s4) << endl;

    cout << "\"" << s5 << "\" -> "
         << sol.isPalindrome(s5) << endl;

    return 0;
}
