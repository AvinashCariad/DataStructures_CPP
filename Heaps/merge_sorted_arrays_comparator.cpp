#include <iostream>
#include <vector>
#include <queue>
#include <iterator>

using namespace std;

// Clean struct with no overloaded operators needed
struct VectorIterator {
    vector<int>::const_iterator curr;
    vector<int>::const_iterator last;
};

vector<int> mergeMultipleSortedVectors(const vector<vector<int>>& arr) {
    vector<int> sorted_vector;

    // 1. Define the lambda comparator for a min-heap - note for min heap:
    auto cmp = [](const VectorIterator& a, const VectorIterator& b) {
        return *(a.curr) > *(b.curr);
    };

    // 2. Pass decltype(cmp) as the template type and cmp to the constructor
    priority_queue<VectorIterator, vector<VectorIterator>, decltype(cmp)> min_heap(cmp);       // remember this, most clean way of doing it

    // 3. Push only non-empty vectors into the heap
    for (const vector<int>& a : arr) {
        if (a.cbegin() != a.cend()) {          // check for empty arrays
            min_heap.emplace(VectorIterator{ a.cbegin(), a.cend() });
        }
    }

    // 4. Pop the smallest element and advance that vector's iterator
    while (!min_heap.empty()) {
        auto min_el = min_heap.top();
        min_heap.pop();

        sorted_vector.emplace_back(*(min_el.curr));

        auto next_it = next(min_el.curr);
        if (next_it != min_el.last) {
            min_heap.emplace(VectorIterator{ next_it, min_el.last });
        }
    }

    return sorted_vector;
}


// main code harness:
int main(int argc, char** argv) {
    vector<vector<int>> arrays = {
        {1, 5, 9, 21},
        {2, 6, 12},
        {0, 3, 7, 15, 30},
        {} // Empty vector is safely ignored
    };

    vector<int> result = mergeMultipleSortedVectors(arrays);

    cout << "Merged Output: ";
    for (int num : result) {
        cout << num << " ";
    }
    cout << "\n";

    return 0;
}
