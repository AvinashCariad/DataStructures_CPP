#include <iostream>
#include <map>
#include <vector>
#include <string>

/*
In C++, a standard std::map only allows one unique value per key 
(assigning to the same key overwrites the old value). To store multiple values under the same key,
C++ offers two standard approaches:

1. std::map<Key, std::vector<Value>> (the pattern used in your grammar program).
2. std::multimap<Key, Value> (a container built specifically for duplicate keys).

Here is a complete, runnable C++ example showing both approaches side-by-side using the same "<noun>" key with three different values ("cat", "dog", "table").

*/

// main code harness:
int main(int argc, char** argv) {
    // =========================================================
    // APPROACH 1: std::map with a std::vector as the value
    // (Best when you want to pick random items by index, like c[0])
    // =========================================================
    std::map<std::string, std::vector<std::string>> grouped_map;

    // Adding multiple values to the exact same key "<noun>"
    grouped_map["<noun>"].push_back("cat");
    grouped_map["<noun>"].push_back("dog");
    grouped_map["<noun>"].push_back("table");

    grouped_map["<verb>"].push_back("jumps");
    grouped_map["<verb>"].push_back("sits");

    std::cout << "=== Approach 1: map<string, vector<string>> ===\n";
    auto it = grouped_map.find("<noun>");
    if (it != grouped_map.end()) {
        std::cout << "Key: " << it->first << " has " << it->second.size() << " values:\n";
        for (const std::string& val : it->second) {
            std::cout << "  - " << val << "\n";
        }
    }

    // =========================================================
    // APPROACH 2: std::multimap
    // (Stores separate key-value pairs that share the same key)
    // =========================================================
    std::multimap<std::string, std::string> multi_map;

    // Inserting separate pairs with the exact same key "<noun>"
    multi_map.insert({"<noun>", "cat"});
    multi_map.insert({"<noun>", "dog"});
    multi_map.insert({"<noun>", "table"});

    multi_map.insert({"<verb>", "jumps"});
    multi_map.insert({"<verb>", "sits"});

    std::cout << "\n=== Approach 2: multimap<string, string> ===\n";
    // equal_range returns a pair of iterators [start, end) for all matching keys
    auto range = multi_map.equal_range("<noun>");
    std::cout << "Key: <noun> has " << multi_map.count("<noun>") << " entries:\n";
    for (auto i = range.first; i != range.second; ++i) {
        std::cout << "  Key: " << i->first << " -> Value: " << i->second << "\n";
    }

    return 0;
}
