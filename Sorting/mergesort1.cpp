#include <iostream>
#include <vector>


/*
• Time Complexity: \(O(n \log n)\) across all cases (best, worst, and average) because the array is always split evenly and takes linear time to merge.
• Space Complexity: O(n) due to the temporary sub-vectors created during the merge step.
• Stability: It is a stable sorting algorithm, meaning elements with equal values maintain their relative order.

*/


using namespace std;

// Merges two sorted subarrays into a single sorted section
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors to hold the split data
    std::vector<int> L(n1);
    std::vector<int> R(n2);

    // Copy data to temporary vectors
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    // Merge the temporary vectors back into the original array
    int i = 0;     // Initial index of first subarray
    int j = 0;     // Initial index of second subarray
    int k = left;  // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Main function that implements Merge Sort recursively
void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        // Same as (left + right) / 2, but prevents overflow for large indices
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Helper function to print the array
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";
}


// main code harness:
int main(int argc, char** argv) {
    std::vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    
    std::cout << "Original array: ";
    printArray(arr);

    // Perform merge sort on the entire vector bounds
    mergeSort(arr, 0, arr.size() - 1);

    std::cout << "Sorted array:   ";
    printArray(arr);

    return 0;
}

