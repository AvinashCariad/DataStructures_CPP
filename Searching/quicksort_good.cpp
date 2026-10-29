#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Helper to choose the median of low, middle, and high elements as the pivot.
// This prevents O(N^2) degradation on sorted or nearly sorted data.
int medianOfThree(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    
    if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    
    // Hide the chosen pivot at the 'low' index temporarily for partitioning
    swap(arr[low], arr[mid]);          // note that swap is the internal cpp routine (std::swap)
    return arr[low];
}

// Hoare's Partition Scheme
// More efficient than Lomuto's because it performs roughly three times fewer swaps on average.
int partition(vector<int>& arr, int low, int high) {
    int pivot = medianOfThree(arr, low, high);
    int i = low - 1;
    int j = high + 1;

    while (true) {
        // Move the left index right until an element >= pivot is found
        do {
            i++;
        } while (arr[i] < pivot);

        // Move the right index left until an element <= pivot is found
        do {
            j--;
        } while (arr[j] > pivot);

        // If indices cross, the partition process is complete
        if (i >= j) return j;

        swap(arr[i], arr[j]);
    }
}
// Internal recursive Quicksort function
void quicksortHelper(vector<int>& arr, int low, int high) {
    if (low < high) {
        // p is the partitioning index; elements at or left of p are <= elements right of p
        int p = partition(arr, low, high);

        // Recursively sort elements on both sides of the partition boundary
        quicksortHelper(arr, low, p);
        quicksortHelper(arr, p + 1, high);
    }
}

// Clean, user-facing driver interface function
void quicksort(vector<int>& arr) {
    if (arr.size() <= 1) return;
    quicksortHelper(arr, 0, arr.size() - 1);
}


// main code harness:
int main(int argc, char** argv) {
    vector<int> data = {24, -5, 3, 0, 100, 24, 9, -12, 55, 3};

    cout << "Original array: ";
    for (int num : data) cout << num << " ";
    cout << endl;

    quicksort(data);

    cout << "Sorted array:   ";
    for (int num : data) cout << num << " ";
    cout << endl;

    // Verify correct sorting orientation
    if (is_sorted(data.begin(), data.end())) {
        cout << "Verification: Array successfully sorted!" << endl;
    } else {
        cout << "Verification: Sorting failed." << endl;
    }

    return 0;
}
