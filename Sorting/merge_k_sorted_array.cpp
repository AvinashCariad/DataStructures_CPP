#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>

// Structure to track the value, which array it 
// came from, and its current index inside that array
// this is Important the way heap node is constructed
struct HeapNode {
    int value;
    size_t array_no;
    size_t element_idx;
};

// Custom comparator to turn std::priority_queue into a Min-Heap
struct CompareNode {
    bool operator()(const HeapNode& a, const HeapNode& b) {
        return a.value > b.value; // Greater-than puts the smallest value at the top
    }
};

// Merges K sorted arrays using a Min-Heap (std::priority_queue)
// TC: O(N * K * log K), SC: O(K)
std::vector<int> mergeKSortedArrays(const std::vector<std::vector<int>>& arrays) {
    std::vector<int> result;
    if (arrays.empty()) return result;

    // Define the Min-Heap
    std::priority_queue<HeapNode, std::vector<HeapNode>, CompareNode> min_heap;        // form the min-heap here

    // 1. Push the first element of each non-empty array into the heap
    for (size_t i = 0; i < arrays.size(); ++i) {
        if (!arrays[i].empty()) {
            min_heap.push({arrays[i][0], i, 0});
        }
    }

    // 2. Process elements until the heap is empty
    while (!min_heap.empty()) {
        // Extract the absolute smallest element at the top
        HeapNode current = min_heap.top();
        min_heap.pop();

        result.push_back(current.value);

        // If the source array has more elements, push its next element into the heap
        size_t next_idx = current.element_idx + 1;
        if (next_idx < arrays[current.array_no].size()) {
            min_heap.push({arrays[current.array_no][next_idx], current.array_no, next_idx});
        }
    }

    return result;
}

void printArray(const std::vector<int>& arr) {
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

// test:
void test_01(size_t num_arrays, size_t num_elements) {

    // generate random numbers:
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 99);

    std::vector<std::vector<int>> arrays(num_arrays, std::vector<int>(num_elements));     // vector of vectors
    std::vector<int> expected_result;

    // Generate and sort random arrays
    for (size_t i = 0; i < num_arrays; ++i) {
        for (size_t j = 0; j < num_elements; ++j) {
            arrays[i][j] = dist(gen);
            expected_result.push_back(arrays[i][j]);
        }
        std::sort(arrays[i].begin(), arrays[i].end());
        std::cout << "Array " << i << ": ";
        printArray(arrays[i]);
    }

    // Merge using our Min-Heap function
    std::vector<int> result = mergeKSortedArrays(arrays);

    std::cout << "Output  : ";
    printArray(result);

    // Generate expected answer using standard sort
    std::sort(expected_result.begin(), expected_result.end());

    // Validation
    if (result == expected_result) {
        std::cout << "Test passed\n";
    } else {
        std::cout << "Test failed\n";
        exit(1);
    }
}

// main code harness:
int main(int argc, char** argv) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> array_dist(2, 5);      // 2 to 5 arrays
    std::uniform_int_distribution<size_t> element_dist(1, 10);  // 1 to 10 elements per array

    for (int i = 0; i < 5; ++i) {
        size_t num_arrays = array_dist(gen);
        size_t num_elements = element_dist(gen);
        
        test_01(num_arrays, num_elements);
        std::cout << "\n_________________________________________________\n\n";
    }

    return 0;
}
