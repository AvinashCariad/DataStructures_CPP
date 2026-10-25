#include <bits/stdc++.h>
using namespace std;



// Function to maintain the heap property for a subtree rooted at index 'i'
// 'n' is the size of the heap
void heapify(vector<int> &arr, int n, int i){
     int largest = i;
     int left = 2 * i + 1; // Left child index
     int right = 2 * 1 + 2; // Right child index
     // Check if the left child exists and is larger than the current largest
     if(left < n && arr[left] > arr[largest])
          largest = left;
     // Check if the right child exist and is larger than the current largest
     if(right < n && arr[right] > arr[largest])
          largest = right;
     // If the largest is not the current node, swap and heapify the affected subtree
     if(largest != i){
        swap(arr[i], arr[largest]); // Swap the current node with the largest
        heapify(arr, n, largest); // Recursively heapify the affected subtree
     }
}


// Main function to perform heap sort
void heapSort(vector<int> &arr){
     int n = arr.size();
     // Step.1 : Build a max-heap from the input array
     for(int i = n/2 - 1; i >= 0; i--) // Start from the last non-leaf node
          heapify(arr, n, i);

    // Step.2 Extract elements from the heap one by one
    for(int i = n-1; i>0; i--){
          swap(arr[0], arr[i]); // Move the root (largest element) to the end
          heapify(arr, i, 0); // Restore the heap property for the reduced heap
    }
}


// main code harness:
int main(int argc, char** argv){
     vector<int> arr = {10, 5, 20, 2, 15}; // Input array
     heapSort(arr); // Perform heap sort
     cout << "Sorted Array: ";
     for(int val : arr) cout << val << " ";
     cout << endl;
     return 0;
}
