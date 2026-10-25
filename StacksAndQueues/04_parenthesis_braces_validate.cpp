#include <stdlib.h>
#include <iostream>
#include <stack>

using namespace std;

void handle_error(void)
{
    cout << "Test failed\n";
    exit(1);
}

// validate nesting:
int validate_nesting(const char *str1) {
    stack<char> s;
    int i = 0;

    while (str1[i]) {
        char c, top_char;
        
        c = str1[i];
        if (c == '{' || c == '[' || c == '(') {
            s.push(c);
        } else if (c == '}' || c == ']' || c == ')') {
            if (s.empty())
                return 0;

            top_char = s.top();
            if (c == '}' && top_char != '{')
                return 0;
            else if (c == ']' && top_char != '[')
                return 0;
            else if (c == ')' && top_char != '(')
                return 0;

            s.pop();
        } else {
            return 0;
        }
        ++i;
    }

    if (!s.empty())
        return 0;

    return 1;
}

void test(const char str1[], int expected_result)
{
    int result = validate_nesting(str1);

    if (result)
        cout << "\"" << str1 << "\" => is balanced " << endl;
    else 
        cout << "\"" << str1 << "\" => is NOT balanced " << endl;

    if (result != expected_result)
        handle_error();
}


// main code harness:
int main(int argc, char** argv)
{
    // Original test cases
    test("[{()}]", 1);
    test("[{]", 0);

    // Critical edge cases
    test("", 1);          // Empty string (should be balanced)    - this is a good used case
    test("(", 0);         // Single open bracket (unbalanced)
    test("]", 0);         // Single close bracket (unbalanced)
    test("([)]", 0);       // Correct types but interleaved incorrectly
    test("abc", 0);        // Non-bracket characters (should fail per your current logic)

    cout << "\nAll tests passed safely!" << endl;
    return 0;
}
