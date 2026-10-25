#include <iostream>
#include <vector>
#include <algorithm>



// main code harness:
int main(int argc, char** argv) {
    // unique 
    std::vector<int> vec = {1, 2, 2, 3, 1, 3, 3, 4};

    // 1. Sort to bring duplicates together
    std::sort(vec.begin(), vec.end()); // vec becomes {1, 1, 2, 2, 3, 3, 3, 4}

    // 2. Apply std::unique and erase the leftovers
    auto new_end = std::unique(vec.begin(), vec.end());
    vec.erase(new_end, vec.end());      // just keep the uniques

    // Output: 1 2 3 4
    for (int x : vec) std::cout << x << " ";        // Output: 1 2 3 4


    // copy:
    std::vector<int> source = {10, 20, 30};
    std::vector<int> dest;

    // Use std::back_inserter so dest expands automatically
    std::copy(source.begin(), source.end(), std::back_inserter(dest));

    // Output: 10 20 30
    for (int x : dest) std::cout << x << " ";


    // std::unique_copy (combine the both above):
    std::vector<int> source = {1, 1, 2, 3, 3, 1}; // Note: consecutive duplicates only
    std::vector<int> dest;

    // Copies unique consecutive items into dest
    std::unique_copy(source.begin(), source.end(), std::back_inserter(dest));

    // Output: 1 2 3 1 (The final '1' stays because it wasn't consecutive to the first '1's)
    for (int x : dest) std::cout << x << " ";

    // In the C++ Standard Template Library (STL), std::transform is the equivalent of
    // a map operation in functional programming. It applies a given function
    // (like a lambda, functor, or function pointer) to a range of elements and saves
    // the results into a destination range
    std::string text = "hello world";

    // Transform every character to uppercase in-place
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return std::toupper(c);
    });

    // Transform and writing to a new container:
    // Use std::back_inserter to dynamically push elements into 'squared'
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<int> squared;
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(squared), [](int x) {
        return x * x;
    });    
    
    // Binary transform:
    std::vector<int> vec1 = {1, 2, 3, 4};
    std::vector<int> vec2 = {10, 20, 30, 40};
    std::vector<int> results;

    // Pairs up vec1[i] and vec2[i], adds them, and inserts into 'results'
    std::transform(vec1.begin(), vec1.end(), vec2.begin(), 
                   std::back_inserter(results), std::plus<int>());



}
