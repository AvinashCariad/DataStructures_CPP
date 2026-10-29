#include <iostream>
#include <utility> // for std::swap and std::move
#include <initializer_list>


/*
 
• Memory Preservation: The Move Constructor and Move Assignment do zero element-wise calculations. Rather than iterating across thousands of items in an array, they swap one single 8-byte pointer (data), turning O(N) allocation operations into instantaneous O(1) executions.


• The nullptr Clean: Setting other.data = nullptr inside the move operations is critical. C++ specifies that calling delete[] nullptr; is a completely safe, harmless no-op. Without nullifying the pointer, the temporary object's destructor would immediately destroy the memory block you just transferred.
 
 * */


class MyVector {
private:
    int* data;          // Pointer to the dynamic array
    size_t capacity;    // Allocated storage space
    size_t size;        // Current number of elements inside

public:
    // 0. Constructor (with Initializer List support)
    MyVector(std::initializer_list<int> list) {
        size = list.size();
        capacity = size;
        data = new int[capacity];
        
        size_t idx = 0;
        for (int val : list) {
            data[idx++] = val;
        }
        std::cout << "[Constructor] Allocated array at: " << (void*)data << " (Size: " << size << ")\n";
    }

    // 1. DESTRUCTOR
    // Cleans up the allocated continuous block of memory.
    ~MyVector() {
        std::cout << "[Destructor] Freeing array at: " << (void*)data << "\n";
        delete[] data;
    }

    // 2. COPY CONSTRUCTOR
    // Performs a deep copy of the underlying array buffer.
    MyVector(const MyVector& other) {
        size = other.size;
        capacity = other.capacity;
        data = new int[capacity]; // Allocate a fresh independent block of memory
        
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i]; // Copy elements over one by one
        }
        std::cout << "[Copy Constructor] Deep copied array to new address: " << (void*)data << "\n";
    }

    // 3. COPY ASSIGNMENT OPERATOR
    // Uses the "Copy-and-Swap" idiom to safely replace data with a temporary deep copy.
    MyVector& operator=(const MyVector& other) {
        std::cout << "[Copy Assignment] Copying via copy-and-swap\n";
        if (this != &other) {
            MyVector temp(other); // Leverage the copy constructor to duplicate the data safely
            
            std::swap(data, temp.data); // Swap internal pointers and properties
            std::swap(size, temp.size);
            std::swap(capacity, temp.capacity);
        } // 'temp' goes out of scope here and automatically cleans up our old memory buffer
        return *this;
    }

    // 4. MOVE CONSTRUCTOR
    // Fast action: Steals the raw pointer from a temporary array without copying elements.
    MyVector(MyVector&& other) noexcept {
        std::cout << "[Move Constructor] Stealing pointer from: " << (void*)other.data << "\n";
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        // Reset the source vector so its destructor doesn't clear the memory we just stole
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // 5. MOVE ASSIGNMENT OPERATOR
    // Replaces our current vector by grabbing the pointer from another vector in an assignment.
    MyVector& operator=(MyVector&& other) noexcept {
        std::cout << "[Move Assignment] Moving array pointers\n";
        if (this != &other) {
            delete[] data; // Clean up our own existing allocated array first

            data = other.data; // Take ownership of the source vector's pointers
            size = other.size;
            capacity = other.capacity;

            // Reset the source vector to a safe, empty state
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    void print() const {
        if (!data || size == 0) {
            std::cout << "Vector: [Empty/Moved]\n";
            return;
        }
        std::cout << "Vector elements: [ ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << "]\n";
    }
};


// main code harness:
int main(int argc, char** argv) {
    std::cout << "--- 1 & 2: Testing Constructor & Copy Constructor ---\n";
    MyVector v1 = {10, 20, 30};
    MyVector v2 = v1; // Invokes Copy Constructor

    std::cout << "\n--- 3: Testing Copy Assignment ---\n";
    MyVector v3 = {99, 100};
    v3 = v1; // Invokes Copy Assignment

    std::cout << "\n--- 4: Testing Move Constructor ---\n";
    // std::move converts v2 into an rvalue, prompting C++ to execute the move constructor instead of deep copy
    MyVector v4 = std::move(v2); 
    v4.print();
    v2.print(); // v2 is now empty, fully gutted of its pointer safely

    std::cout << "\n--- 5: Testing Move Assignment ---\n";
    MyVector v5 = {4, 5, 6};
    v5 = std::move(v3); // Invokes Move Assignment
    v5.print();
    
    std::cout << "\n--- Scope Ending: Destructors will clean up remaining objects ---\n";
    return 0;
}

