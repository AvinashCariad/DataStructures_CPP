#include <iostream>
#include <vector>
#include <random>
#include <utility>
using namespace std;

// Bubble Sort
// TC: O(n^2), Best Case: O(n)
vector<int> bubbleSort(vector<int> arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        bool sorted = true; // FIXED: Reset flag at the start of EVERY pass
        for (size_t j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                sorted = false; 
            }
        }
        if (sorted) return arr; 
    }
    return arr;
}

// Selection Sort
// TC: O(n^2)
vector<int> selectionSort(vector<int> arr) {
    if (arr.empty()) return arr;
    for (size_t i = 0; i < arr.size() - 1; i++) {
        size_t min_el = i;
        for (size_t j = i + 1; j < arr.size(); j++) {
            if (arr[j] < arr[min_el])
                min_el = j;
        }
        swap(arr[i], arr[min_el]);
    }
    return arr;
}

// Insertion Sort
// TC: O(n^2), Best Case: O(n)
vector<int> insertionSort(vector<int> arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        size_t j = i;
        while (j > 0 && arr[j-1] > arr[j]) {
            swap(arr[j-1], arr[j]);
            --j;
        }
    }
    return arr;
}


////////////////////////////////////////QUICK AND MERGE SORT///////////////////////////////

//Neither algorithm is universally "better"; rather, Quicksort is the king of arrays and primitive
//data, while Mergesort is the king of linked lists, stable data, and massive datasets


// Partition helper for Quick Sort
int partitionArray(vector<int>& arr, int pivot_idx, int l, int h) {
    int pivot_value = arr[pivot_idx];
    swap(arr[h], arr[pivot_idx]);
    int small = l;
    
    for (int i = l; i < h; i++) {
        if (arr[i] < pivot_value) {
            swap(arr[i], arr[small]);
            ++small;
        }
    }
    swap(arr[h], arr[small]);
    return small;
}

// Quick Sort Helper
void quickSortHelper(vector<int>& arr, int l, int h, default_random_engine& gen) {
    if (l < h) { // FIXED: Correct boundary check
        int pivot_idx = uniform_int_distribution<int>{l, h}(gen);
        int partition_idx = partitionArray(arr, pivot_idx, l, h);
        
        quickSortHelper(arr, l, partition_idx - 1, gen);
        quickSortHelper(arr, partition_idx + 1, h, gen);
    }
}

// Quick Sort
// TC: Avg O(n log n), Worst O(n^2)
vector<int> quickSort(vector<int> arr) {
    if (arr.empty()) return arr;
    default_random_engine gen((random_device())());       // note how the gen is generated 
    quickSortHelper(arr, 0, arr.size() - 1, gen);
    return arr;
}

// Merges the subarrays for Merge Sort
void mergeArrays(vector<int>& arr, int l, int mid, int h) { // FIXED: Passed arr by reference
    int n1 = mid - l + 1;
    int n2 = h - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// Merge Sort Helper
void mergeSortHelper(vector<int>& arr, int l, int h) {
    if (l < h) { // FIXED: Tight base-case guard to prevent recursion loop
        int mid = l + (h - l) / 2;
        mergeSortHelper(arr, l, mid);       // FIXED: Included midpoint inside left segment
        mergeSortHelper(arr, mid + 1, h);   // FIXED: Right segment starts right after mid
        mergeArrays(arr, l, mid, h);        // FIXED: Combined bounds cleanly
    }
}

// Merge Sort
// TC: O(n log n), SC: O(n) - Merge is better than quicksort for the worst case scenario
vector<int> mergeSort(vector<int> arr) {
    if (arr.empty()) return arr;
    mergeSortHelper(arr, 0, arr.size() - 1);
    return arr;
}

// Fixed stream operator definition with proper return signature
ostream& operator<<(ostream& out, const vector<int>& arr) {
    for (const auto& d : arr)
        out << d << " ";
    out << "\n";
    return out;
}


// main code harness:
int main(int argc, char** argv) {
    vector<int> arr = {0, -2, -3, 6, 3, 2, 17, 2, 7, -4, 9};
    
    cout << "Original:\n"        << arr;
    cout << "Bubble Sort:\n"     << bubbleSort(arr);
    cout << "Selection Sort:\n"  << selectionSort(arr);
    cout << "Insertion Sort:\n"  << insertionSort(arr);
    cout << "Quick Sort:\n"      << quickSort(arr);
    cout << "Merge Sort:\n"      << mergeSort(arr);
    
    return 0;
}




/*

















*/