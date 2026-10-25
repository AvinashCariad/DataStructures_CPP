#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <string>


using namespace std;


// --- Core Generic Min-Heap Template ---
template <typename T>
class MinHeap {
private:
    std::vector<T> heap;

    int parent(int i)      { return (i - 1) / 2; }
    int leftChild(int i)   { return (2 * i) + 1; }
    int rightChild(int i)  { return (2 * i) + 2; }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i); 
        }
    }

    void heapifyDown(int i) {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < heap.size() && heap[left] < heap[minIndex]) {
            minIndex = left;
        }
        if (right < heap.size() && heap[right] < heap[minIndex]) {
            minIndex = right;
        }
        if (i != minIndex) {
            std::swap(heap[i], heap[minIndex]);
            heapifyDown(minIndex);
        }
    }

public:
    void push(const T& value) {
        heap.push_back(value);      
        heapifyUp(heap.size() - 1); 
    }

    T pop() {
        if (empty()) {
            throw std::out_of_range("Heap underflow");
        }
        T minVal = heap[0];       
        heap[0] = heap.back();      
        heap.pop_back();            
        if (!empty()) {
            heapifyDown(0);         
        }
        return minVal;
    }

    T peek() const {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return heap[0];
    }

    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
};

// --- FIFO Queue Wrapper Component ---
template <typename T>
class Queue {
private:
    // Internal struct to tie the data to its arrival timestamp/order
    struct QueueElement {
        unsigned long long priority_val; // Sequential ID to preserve order
        T data;

        // Custom operators for our Min-Heap template to evaluate
        bool operator<(const QueueElement& other) const {
            return this->priority_val < other.priority_val;
        }

        bool operator>(const QueueElement& other) const {
            return this->priority_val > other.priority_val;
        }
    };

    // Unsigned 64-bit counter prevents logical boundary overflows
    unsigned long long sequence_counter_ = 0;
    
    // Leverage our underlying Min-Heap to track the smallest sequence number
    MinHeap<QueueElement> min_heap_;

public:
    // Enqueue: Add an element to the back of the queue
    void Enqueue(T data) {
        min_heap_.push(QueueElement{sequence_counter_, data});
        ++sequence_counter_; // The next item will get a larger ID
    }

    // Dequeue: Remove and return the front element of the queue
    T Dequeue() {
        if (min_heap_.empty()) {
            throw std::length_error("Queue underflow: Cannot dequeue from an empty queue");
        }
        QueueElement el = min_heap_.pop();
        return el.data;
    }

    // Front: Look at the front element without removing it
    T Front() {
        if (min_heap_.empty()) {
            throw std::length_error("Queue is empty");
        }
        return min_heap_.peek().data;
    }

    // Check if empty
    bool Empty() {
        return min_heap_.empty();
    }

    // Get current size
    size_t Size() {
        return min_heap_.size();
    }
};


// main code harness:
int main(int argc, char** argv) {
    Queue<std::string> printQueue;

    std::cout << "--- Enqueuing Print Jobs ---\n";
    printQueue.Enqueue("Document_A.pdf");
    printQueue.Enqueue("Photo_B.png");
    printQueue.Enqueue("Report_C.docx");

    std::cout << "Front of the queue: " << printQueue.Front() << "\n\n";

    std::cout << "--- Processing Queue (FIFO Order) ---\n";
    while (!printQueue.Empty()) {
        std::cout << "Processing: " << printQueue.Dequeue() << "\n";
    }

    cout << endl;

    return 0;
}
