/*
    Generic Heap class Implementation.

        For heap of size 'n':
            TC: Push: O(logn)
                Pop: O(logn)
                Heapify: O(n)
                Max/Min: O(1)
            Explanation: http://www.cs.umd.edu/~meesh/351/mount/lectures/lect14-heapsort-analysis-part.pdf
            SC: O(n) for heap
*/

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <functional>
using namespace std;

typedef enum { MIN_HEAP, MAX_HEAP } HeapType;

template<typename T>
class Heap {
private:
    vector<T> heap_;
    int capacity_;
    int num_heap_elements_;
    HeapType heap_type_;
    const int RESIZE_FACTOR = 2;
    function<bool(int, int)> comparator_;
        
public:
    // Constructor
    // creates a heap with elements from vector and the capacity 
    // is initialised to twice vector size
    Heap(vector<T> arr, HeapType type) {
        // no. of elements in heap
        num_heap_elements_ = 0;
        // min or max heap
        heap_type_ = type;

        for (const auto& a : arr)
            heap_.emplace_back(a);
        num_heap_elements_ = heap_.size();
        // update the heap capacity
        capacity_ = 2 * num_heap_elements_;
        heap_.resize(capacity_);

        // set the comparator_ acc. to heap type
        if(heap_type_ == MAX_HEAP)
            comparator_ = greater<T>();
        else
            comparator_ = less<T>();

        // now heapify the vector, starting with the first non-leaf node
        // since leaf nodes always satisfy heap property
        // start percolating down
        for (int i = (num_heap_elements_ - 1) / 2; i >= 0; i--) {
                heapifyDown(i, comparator_);
        }
    }

    // Constructor
    // creates heap with just the size and type
    Heap(int capacity, HeapType type) {
        // no. of elements in heap
        num_heap_elements_ = 0;
        // min or max heap
        heap_type_ = type;
        capacity_ = capacity;
        // set heap to current capacity
        heap_.resize(capacity_);
        // set the comparator_ acc. to heap type
        if(heap_type_ == MAX_HEAP)
            comparator_ = greater<T>();
        else
            comparator_ = less<T>();
    }

    // for pushing element to heap
    void Push(T data) {
        // when heap is full
        if (num_heap_elements_ == capacity_) {
            // resize the vector for heap
            resizeHeap();
        }

        // add the element at last
        // then heapify from there to all the way up
        ++num_heap_elements_;
        heap_[num_heap_elements_ - 1] = data;
            
        // while the parent is not satisfying the heap
        // property, swap currrent element with parent
        // till the time current element is at its correct position
        int i = num_heap_elements_ - 1;
        while (i > 0 && (!comparator_(heap_[getParentIndex(i)], heap_[i]))) {
            int parent_idx = getParentIndex(i);
            swap(heap_[parent_idx], heap_[i]);
            i = parent_idx;
        }
    }

    // for deleting element from heap
    /*
        Heap deletion: only the root element is deleted.
        We swap the root element with last element in vector, then make the
        heap size reduced by 1. Then to fix the distruption caused by
        swapping we heapify from root
    */
    T Pop() {
        // check if heap is empty or not
        if (isEmpty())
            throw length_error("Heap is empty");

        // swap the first with last 
        swap(heap_[0], heap_[num_heap_elements_ - 1]);

        T popped_data = heap_[num_heap_elements_ - 1];
        // decrease the heap elements count by 1
        --num_heap_elements_;

        // heapify down to ensure heap property is maintained
        heapifyDown(0, comparator_);

        return popped_data;
    }

    // get the root element of heap
    T Top() {
        if (isEmpty())
            throw length_error("Heap is empty");
        return heap_[0];
    }

    // checks if heap is empty
    bool isEmpty() {
        return num_heap_elements_ == 0;
    }

    // returns the index of parent
    int getParentIndex(const int& i) {
        // either heap is empty or index is out of bound
        if (isEmpty() || (i <= 0) || (i >= num_heap_elements_))
            return -1;
        return (i - 1) / 2;
    }

    // returns the index of left child
    int getLeftChildIndex(const int& i) {
        // either heap is empty or index is out of bound
        if (isEmpty() || (i < 0) || (i >= num_heap_elements_))
            return -1;
        return (2 * i + 1) < num_heap_elements_ ? (2 * i + 1) : -1;
    }

    // returns the index of right child
    int getRightChildIndex(const int& i) {
        // either heap is empty or index is out of bound
        if (isEmpty() || (i < 0) || (i >= num_heap_elements_))
            return -1;
        return (2 * i + 2) < num_heap_elements_ ? (2 * i + 2) : -1;
    }

    // for fixing the heap property in the heap
    /* The logic for fixing the heap property is that we
        check if the left or right child of current index is greater in
        case of max-heap or smaller in case of min-heap.
        If that is the case then to fix the heap property we swap that element
         with current parent, and then heapify from that index
    */
    template<typename Compare>
    void heapifyDown(int idx, Compare comp) {
        // assume that current parent is saisfying heap property
        // for max heap: this will contain greatest amongst parent, left and right child
        // for min_heap: this will comtain min amongst parent, left and right child
        int heap_prop_idx = idx;

        // get left and right child
        int left = getLeftChildIndex(idx);
        int right = getRightChildIndex(idx);

        // check if the child nodes are greater/lesser than parent or not
        // compare function: max-heap: if first is greater than second returns true
        // min_heap: if first is smaller than second returns true
        if (left != -1 && comp(heap_[left], heap_[heap_prop_idx])) {
            heap_prop_idx = left;
        }
        if (right != -1 && comp(heap_[right], heap_[heap_prop_idx])) {
            heap_prop_idx = right;
        }

        // now if the current parent is not satusfying heap property
        // swap it with its child that is greater/smaller than it
        if (heap_prop_idx != idx) {
            swap(heap_[heap_prop_idx], heap_[idx]);
            // fix the lower subtrees
            heapifyDown(heap_prop_idx, comp);
        }
    }

    // for expanding capacity of heap
    void resizeHeap() {
        // update current capacity
        capacity_ = capacity_ * RESIZE_FACTOR;
        heap_.resize(capacity_);
    }

    // shows heap array elements
    void showHeapArray() {
        for (int i = 0; i < num_heap_elements_; i++)
            cout << heap_[i] << " ";
        cout << endl;
    }
};


// for doing heap sort
/*
    We are doing in place sort, so we start writing from back.
    For sorting in increasing order: max_heap is used
    For sorting in decreasing order: min_heap is used
*/
template<typename T>
void heapSort(vector<T>& arr, HeapType type) {
    // heapify the  array
    Heap<T> h(arr, type);
    int write_idx = arr.size() - 1;
    int data;

    while (!h.isEmpty()) {
        // max or min data
        data = h.Pop();
        arr[write_idx--] = data;
    }
}


// main code harness:
int main(int argc, char** argv) {
    // 562, 300, 400, 29, 200, 360, 10, 4
    vector<int> arr = { 562, 300, 10, 200, 29, 360, 4, 400 };
    HeapType type = MAX_HEAP;
    Heap<int> heap(arr, type);
    cout << heap.Pop() << endl;
    heap.showHeapArray();
  
    cout << endl;
    cout << heap.Pop() << endl;
    heap.Push(21323);
    heap.showHeapArray();
    
    heapSort<int>(arr, MAX_HEAP);
    for (auto a : arr)
        cout << a << " ";

    return 0;
}



/*

//Here is a complete, production-ready implementation of a Max-Heap in C++ using a standard 
//std::vector to dynamically manage the underlying array.This implementation includes the 
//essential operations: Insertion (push), Extraction (pop max), and Heapify (both upward and downward siftings) to maintain the structural and heap properties.


//Here is a complete, production-ready implementation of a Max-Heap in C++ using a standard std::vector to dynamically
//manage the underlying array.This implementation includes the essential operations: 
//Insertion (push), Extraction (pop max), and Heapify (both upward and downward siftings) to maintain the structural and heap properties.




/*=======================================================MAX HEAP===================================================
#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

class MaxHeap {
private:
    std::vector<int> heap;

    // Helper formulas for 0-based indexing mapping
    int parent(int i)      { return (i - 1) / 2; }
    int leftChild(int i)   { return (2 * i) + 1; }
    int rightChild(int i)  { return (2 * i) + 2; }

    // Moves the element at index i up to restore the heap property
    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)] < heap[i]) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i); // Move to the parent's index
        }
    }

    // Moves the element at index i down to restore the heap property
    void heapifyDown(int i) {
        int maxIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);

        // Check if left child is larger than current max
        if (left < heap.size() && heap[left] > heap[maxIndex]) {
            maxIndex = left;
        }

        // Check if right child is larger than current max
        if (right < heap.size() && heap[right] > heap[maxIndex]) {
            maxIndex = right;
        }

        // If the parent is not the largest, swap and recurse downward
        if (i != maxIndex) {
            std::swap(heap[i], heap[maxIndex]);
            heapifyDown(maxIndex);
        }
    }

public:
    // Insert a new element into the heap
    void push(int value) {
        heap.push_back(value);      // Step 1: Insert at the end of the array
        heapifyUp(heap.size() - 1); // Step 2: Bubble up to restore max-heap order
    }

    // Remove and return the maximum element (the root)
    int pop() {
        if (empty()) {
            throw std::out_of_range("Heap underflow: Cannot pop from an empty heap");
        }

        int maxVal = heap[0];       // Step 1: Save the root value
        heap[0] = heap.back();      // Step 2: Move the last element to the root
        heap.pop_back();            // Step 3: Remove the last duplicate element

        if (!empty()) {
            heapifyDown(0);         // Step 4: Bubble down to restore max-heap order
        }

        return maxVal;
    }

    // Get the maximum element without removing it
    int peek() const {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return heap[0];
    }

    // Check if the heap is empty
    bool empty() const {
        return heap.empty();
    }

    // Get the current size of the heap
    size_t size() const {
        return heap.size();
    }

    // Print the internal array representation of the heap
    void printHeap() const {
        std::cout << "Heap Array: [ ";
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << "]\n";
    }
};

// main code harness:
int main(int argc, char** argv) {
    MaxHeap maxHeap;

    std::cout << "--- Inserting Elements ---\n";
    maxHeap.push(10);
    maxHeap.push(7);
    maxHeap.push(6);
    maxHeap.push(5);
    maxHeap.push(4);
    maxHeap.push(3);
    
    maxHeap.printHeap(); // Expected structured layout matching the tree hierarchy

    std::cout << "\nInserting a large value (15) to trigger heapifyUp:\n";
    maxHeap.push(15);
    maxHeap.printHeap();

    std::cout << "\n--- Extracting Max Elements (Sorting behavior) ---\n";
    while (!maxHeap.empty()) {
        std::cout << "Popped Max: " << maxHeap.pop() << " -> New ";
        maxHeap.printHeap();
    }

    return 0;
}


/*==============================================================MIN HEAP===================================================


#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

class MinHeap {
private:
    std::vector<int> heap;

    // Helper formulas for 0-based indexing mapping
    int parent(int i)      { return (i - 1) / 2; }
    int leftChild(int i)   { return (2 * i) + 1; }
    int rightChild(int i)  { return (2 * i) + 2; }

    // Moves the element at index i up to restore the min-heap property
    void heapifyUp(int i) {
        // Changed condition: Loop continues if parent is LARGER than the current child
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i); 
        }
    }

    // Moves the element at index i down to restore the min-heap property
    void heapifyDown(int i) {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);

        // Changed condition: Check if left child is SMALLER than current min
        if (left < heap.size() && heap[left] < heap[minIndex]) {
            minIndex = left;
        }

        // Changed condition: Check if right child is SMALLER than current min
        if (right < heap.size() && heap[right] < heap[minIndex]) {
            minIndex = right;
        }

        // If the parent is not the smallest, swap and recurse downward
        if (i != minIndex) {
            std::swap(heap[i], heap[minIndex]);
            heapifyDown(minIndex);
        }
    }

public:
    // Insert a new element into the heap
    void push(int value) {
        heap.push_back(value);      
        heapifyUp(heap.size() - 1); 
    }

    // Remove and return the minimum element (the root)
    int pop() {
        if (empty()) {
            throw std::out_of_range("Heap underflow: Cannot pop from an empty heap");
        }

        int minVal = heap[0];       // Save the minimum root value
        heap[0] = heap.back();      // Move the last element to the root
        heap.pop_back();            // Remove the last duplicate element

        if (!empty()) {
            heapifyDown(0);         // Bubble down to restore min-heap order
        }

        return minVal;
    }

    // Get the minimum element without removing it
    int peek() const {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return heap[0];
    }

    // Check if the heap is empty
    bool empty() const {
        return heap.empty();
    }

    // Get the current size of the heap
    size_t size() const {
        return heap.size();
    }

    // Print the internal array representation of the heap
    void printHeap() const {
        std::cout << "Heap Array: [ ";
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << "]\n";
    }
};

int main() {
    MinHeap minHeap;

    std::cout << "--- Inserting Elements ---\n";
    minHeap.push(10);
    minHeap.push(7);
    minHeap.push(15);
    minHeap.push(3);
    minHeap.push(5);
    
    minHeap.printHeap(); // The minimum element (3) will be at index 0

    std::cout << "\nInserting a very small value (1) to trigger heapifyUp:\n";
    minHeap.push(1);
    minHeap.printHeap();

    std::cout << "\n--- Extracting Min Elements (Sorting behavior) ---\n";
    while (!minHeap.empty()) {
        std::cout << "Popped Min: " << minHeap.pop() << " -> New ";
        minHeap.printHeap();
    }

    return 0;
}









*/