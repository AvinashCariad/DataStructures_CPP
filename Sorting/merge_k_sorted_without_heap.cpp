#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

// Merges two single sorted arrays into one sorted array
// TC: O(len1 + len2), SC: O(len1 + len2) for the temporary result
std::vector<int> mergeTwoArrays(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::vector<int> merged;
    merged.reserve(arr1.size() + arr2.size()); // Optimize memory allocations

    size_t i = 0, j = 0;
    while (i < arr1.size() && j < arr2.size()) {
        if (arr1[i] <= arr2[j]) {
            merged.push_back(arr1[i++]);
        } else {
            merged.push_back(arr2[j++]);
        }
    }

    // Append any remaining elements
    while (i < arr1.size()) merged.push_back(arr1[i++]);
    while (j < arr2.size()) merged.push_back(arr2[j++]);

    return merged;
}

// Merges K sorted arrays using Divide and Conquer
// TC: O(N * log K), SC: O(N) space to hold the working pairs
std::vector<int> mergeKSortedArrays(std::vector<std::vector<int>> arrays) {
    if (arrays.empty()) return {};
    
    // Repeat until only 1 array is left in our collection
    while (arrays.size() > 1) {
        std::vector<std::vector<int>> next_round_arrays;
        
        // Merge arrays in pairs: (0,1), (2,3), etc.
        for (size_t i = 0; i < arrays.size(); i += 2) {
            if (i + 1 < arrays.size()) {
                next_round_arrays.push_back(mergeTwoArrays(arrays[i], arrays[i + 1]));
            } else {
                // If there is an odd array left out, pass it to the next round
                next_round_arrays.push_back(arrays[i]);
            }
        }
        // Update arrays list with the newly merged pairs
        arrays = std::move(next_round_arrays);
    }

    return arrays[0];
}




void printArray(const std::vector<int>& arr) {
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

void test_01(size_t num_arrays, size_t num_elements) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 99);

    std::vector<std::vector<int>> arrays(num_arrays, std::vector<int>(num_elements));
    std::vector<int> expected_result;

    for (size_t i = 0; i < num_arrays; ++i) {
        for (size_t j = 0; j < num_elements; ++j) {
            arrays[i][j] = dist(gen);
            expected_result.push_back(arrays[i][j]);
        }
        std::sort(arrays[i].begin(), arrays[i].end());
        std::cout << "Array " << i << ": ";
        printArray(arrays[i]);
    }

    std::vector<int> result = mergeKSortedArrays(arrays);

    std::cout << "Output  : ";
    printArray(result);

    std::sort(expected_result.begin(), expected_result.end());

    if (result == expected_result) {
        std::cout << "Test passed\n";
    } else {
        std::cout << "Test failed\n";
        exit(1);
    }
}

int main(int argc, char** argv) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> array_dist(2, 5);
    std::uniform_int_distribution<size_t> element_dist(1, 10);

    for (int i = 0; i < 5; ++i) {
        size_t num_arrays = array_dist(gen);
        size_t num_elements = element_dist(gen);
        
        test_01(num_arrays, num_elements);
        std::cout << "\n_________________________________________________\n\n";
    }

    return 0;
}
