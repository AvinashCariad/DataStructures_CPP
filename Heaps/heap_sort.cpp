#include <iostream>
#include <vector>

// To heapify a subtree rooted with node i which is an index in arr[]. 
// n is the size of the heap.
// Algorithm ComplexityTime Complexity: \(\mathcal{O}(n \log n)\) across all cases (best, worst, and average).
// Space Complexity: \(\mathcal{O}(1)\)


// heapify:
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // left child index = 2*i + 1
    int right = 2 * i + 2; // right child index = 2*i + 2

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // If largest is not root, swap and continue heapifying
    if (largest != i) {
        std::swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// Main function to perform heap sort
void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Step 1: Build a max heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Step 2: One by one extract an element from the heap
    // We need Step 2 because building the max heap in Step 
    // 1 only partially organizes the array; it does not fully
    // sort it.In a max heap, the only guarantee is that the largest
    // element is at the root (index 0). The rest of the elements are
    // arranged by parent-child relationships, not in sequential order.Here is exactly what Step 2 does to finish the job:
    for (int i = n - 1; i > 0; i--) {
        // Move current root to the end
        std::swap(arr[0], arr[i]);

        // Call max heapify on the reduced heap - 
        heapify(arr, i, 0);
    }
}

// Helper function to print vector contents
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";
}



// main code harness:
int main(int argc, char** argv) {
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};
    
    std::cout << "Original array: ";
    printArray(arr);

    heapSort(arr);

    std::cout << "Sorted array:   ";
    printArray(arr);
    
    return 0;
}



/*

// Alternative STL approach:

#include <iostream>
#include <vector>
#include <algorithm> // Required for heap methods

int main() {
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};

    // Converts the vector into a max heap container
    std::make_heap(arr.begin(), arr.end());

    // Sorts the heap elements in ascending order
    std::sort_heap(arr.begin(), arr.end());

    return 0;
}


*/