#ifndef GUARD_split_h
#define GUARD_split_h

#include <vector>
#include <string>

std::vector<std::string> split(const std::string&);
#endif



// code is taken from §5.6/88
#include <iostream>
#include <vector>
#include <string>
//#include "split.h"


using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;



using std::vector;
using std::string;

vector<string> split(const string& s)
{
    vector<string> ret;
    typedef string::size_type string_size;
    string_size i = 0;

    // invariant: we have processed characters [original value of i, i)
    while (i != s.size()) {
        // ignore leading blanks
        // invariant: characters in range [original i, current i) are all spaces
        while (i != s.size() && isspace(s[i]))
            ++i;

        // find end of next word
        string_size j = i;
        // invariant: none of the characters in range [original j, current j) is a space
        while (j != s.size() && !isspace(s[j]))
            ++j;

        // if we found some nonwhitespace characters
        if (i != j) {
            // copy from s starting at i and taking j - i chars
            ret.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return ret;
}


// main code harness:
int main(int argc, char** argv)
{
    string s;
    
    // read and split each line of input
    while (getline(cin, s)) {
        vector<string> v = split(s);

        // write each word in v
        for (vector<string>::size_type i = 0; i != v.size(); ++i)
            cout << v[i] << endl;
    }
    
    return 0;
}
