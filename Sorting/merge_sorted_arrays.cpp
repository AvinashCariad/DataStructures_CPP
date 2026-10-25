/* 
	Given two sorted arrays. First array has extra space at the back, big enough to accomodate 
	the other array. Merge the two arrays in such a way that the resultant array is also sorted.
	Merged array should reside in the first array itself.

	Input: two int sorted arrays
	Output: array 1 which has merged values 

	Solution:
		Since extra space is at the back we can just take indices for each array pointing to the 
		last element and then start traversal backwards.
		The larger of the two arrays will put its value at the current index at the back and decrement
		its index.

		TC: O(n + m)
		SC: O(1)
*/


#include <iostream>
#include <vector>
using namespace std;

// Merges two already sorted arrays into the larger array 'a' from right to left
// TC: O(M + N), SC: O(1)
vector<int>& mergeSortedArrays(vector<int>& a, vector<int>& b,
                    const int& M, const int& N) {
    int i = M - 1;          // index of last valid element in array a
    int j = N - 1;          // last index of array b
    int write_idx = (M + N) - 1; // target writable index at the end of the data region
    
    // Process backwards from the largest values
    while (i >= 0 && j >= 0) {
        a[write_idx--] = (a[i] > b[j]) ? a[i--] : b[j--];
    }
    
    // If 'i' runs out first, fill the remaining elements from array b
    // (Note: If 'j' runs out first, elements in 'a' are already in their correct places!)
    while (j >= 0) {
        a[write_idx--] = b[j--];
    }
        
    return a; // Return by reference to avoid copying
}

// Corrected stream operator definition
ostream& operator<<(ostream& out, const vector<int>& arr) {
    for (const auto& val : arr) {
        out << val << " ";
    }
    return out; // Fixed: returned the stream reference
}


// main code harness:
int main(int argc, char** argv) {
    vector<int> a = {-22, 2, 5, 6};
    a.resize(10); // Expands array capacity to fit elements of both arrays
    vector<int> b = {-33, 0, 4, 5, 7, 8};

    // Merges b into a, and prints the updated vector 'a'
    mergeSortedArrays(a, b, 4, 6);
    
    cout << "Merged array: " << a << endl;
    return 0;
}




/*

#include <iostream>
#include <vector>
#include <algorithm>

// Merges two sorted vectors into a new sorted vector
std::vector<int> mergeSortedArrays(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::vector<int> result;
    result.reserve(arr1.size() + arr2.size()); // Optimize memory allocation - note how the memory allocation is done
    
    size_t i = 0; // Pointer for arr1
    size_t j = 0; // Pointer for arr2

    // Compare elements from both arrays and add the smaller one
    while (i < arr1.size() && j < arr2.size()) {
        if (arr1[i] <= arr2[j]) {
            result.push_back(arr1[i]);
            ++i;
        } else {
            result.push_back(arr2[j]);
            ++j;
        }
    }

    // Append any remaining elements from arr1
    while (i < arr1.size()) {
        result.push_back(arr1[i]);
        ++i;
    }

    // Append any remaining elements from arr2
    while (j < arr2.size()) {
        result.push_back(arr2[j]);
        ++j;
    }

    return result;
}



// main code harenss:
int main(int argc, char** argv) {
    std::vector<int> a = {1, 3, 5, 7};
    std::vector<int> b = {2, 4, 6, 8, 10};

    std::vector<int> merged = mergeSortedArrays(a, b);

    std::cout << "Merged Array: ";
    for (int val : merged) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}



*/



/*

std::vector<int> merged(a.size() + b.size());
std::merge(a.begin(), a.end(), b.begin(), b.end(), merged.begin());

*/