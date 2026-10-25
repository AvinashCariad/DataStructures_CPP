// C++ program to demonstrate the use of comparator to sort the characters in increasing frequency.

#include <iostream>
#include <map> 
#include <algorithm>
using namespace std;

int main() {
    string s = "Heellloooo";

    // Creating a map to store the frequency of each character
    map<char, int> mp;
    
    // Counting the frequency of each character in the string
    for (int i = 0; i < s.length(); i++) {
        mp[s[i]]++;
    }

    // Lambda function 'compare' to serve as a custom comparator for sorting
    auto compare = [&](char a, char b) { return mp[a] > mp[b]; };

    // Sort the string based on character frequency using the custom comparator
    //sort(s.begin(), s.end(), compare);

    sort(s.begin(), s.end(), [&](char a, char b) { return mp[a] > mp[b]; });

    // Display the sorted string based on character frequency
    cout << s;

    return 0;
}