#include <bits/stdc++.h>
using namespace std;


// Define the solution class:
class Solution {
public:
    struct Name {
        bool operator==(const Name& that) const {
            return first_name == that.first_name;
        }

        bool operator<(const Name& that) const {
            if (first_name != that.first_name) {
                return first_name < that.first_name;
            }
            return last_name < that.last_name;
        }

        string first_name, last_name;
    };

    void EliminateDuplicate(vector<Name>& names) {
        sort(names.begin(), names.end());
        names.erase(unique(names.begin(), names.end()), names.end());    // unique() keeps the first occurrence for each first name
    }
};


// main code harness:
int main(int argc, char** argv) {
    vector<Solution::Name> names = {            // note how the vector of structs is formed that is internal to class
        {"John", "Smith"},
        {"John", "Adams"},
        {"Alice", "Brown"},
        {"Bob", "Clark"},
        {"Alice", "Davis"}
    };

    Solution sol;
    sol.EliminateDuplicate(names);

    for (const auto& name : names) {
        cout << name.first_name << " " << name.last_name << endl;
    }

    return 0;
}
