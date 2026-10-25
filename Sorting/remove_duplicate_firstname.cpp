/* 
	Given an array which contains names in the form of <first_name, last_name>. Remove the duplicate 
	first name entries. Second name doesn't matter for those cases.

	Input: array of names
	Output: name array without first name duplicates

	Solution:
		We sort the array acc. to first name, then again do one travesal and remove
		the duplicate first names.
		
		TC: O(nlogn)
		SC: O(1)
*/


#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Name {
    string first;
    string last;
};

// Removes first name duplicates in O(n log n) time and O(1) extra space
vector<Name> removeFirstNameDuplicates(vector<Name>& names) {
    if (names.empty()) return names;

    // 1. Sort primarily by first name so duplicates group together
    sort(names.begin(), names.end(), [](const Name& a, const Name& b) {
        if (a.first != b.first)
            return a.first < b.first;
        return a.last < b.last;
    });
    
    // 2. Linear traversal deduplication
    int write_idx = 0;
    for (size_t i = 1; i < names.size(); i++) {
        // If we find a fresh, new first name
        if (names[write_idx].first != names[i].first) {
            write_idx++;
            names[write_idx] = names[i]; // FIXED: Removed 'i++' from inside the block
        }
    }

    // Slice and return the unique subsegment
    return vector<Name>(names.begin(), names.begin() + write_idx + 1);
}

// C++ Alternative: Using the Standard Template Library (STL)
vector<Name> removeFirstNameDuplicatesSTL(vector<Name>& names) {
    if (names.empty()) return names;

    sort(names.begin(), names.end(), [](const Name& a, const Name& b) {
        return a.first < b.first;
    });

    // std::unique groups duplicates at the end and returns an iterator to the "new end"
    auto it = unique(names.begin(), names.end(), [](const Name& a, const Name& b) {
        return a.first == b.first;
    });

    // Erase the remaining duplicate elements from the vector
    names.erase(it, names.end());
    return names;
}

// Corrected stream operator definition
ostream& operator<<(ostream& out, const vector<Name>& arr) {
    for (const auto& a : arr)
        out << a.first << " " << a.last << "\n";
    return out; // FIXED: Added missing return statement
}


// main code harness:
int main(int argc, char** argv) {
    vector<Name> names = {{"jon", "snow"}, {"sam", "khanna"}, {"jon", "oliver"}, 
                          {"ravi", "snow"}, {"ravi", "raju"}};
    
    cout << "Deduplicated Names:\n";
    cout << removeFirstNameDuplicates(names);
    return 0;
}
