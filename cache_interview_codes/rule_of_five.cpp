#include <iostream>
#include <cstring>
#include <utility> // for std::swap and std::move

/*

Rule Name	Era	Required Special Functions	Core Purpose
Rule of Three	C++98 / Traditional	1. Destructor
2. Copy Constructor
3. Copy Assignment	Ensures Memory Safety by performing deep copies and avoiding double-frees.
Rule of Five	C++11 / Modern	All of the above plus:
4. Move Constructor
5. Move Assignment	Adds Performance Optimization by stealing resources from temporary objects instead of duplicating them.

*/

class DynamicBuffer {
private:
    char* data;
    size_t size;

public:
    // 0. Standard Constructor
    DynamicBuffer(const char* str = "") {
        size = std::strlen(str);
        data = new char[size + 1];
        std::strcpy(data, str);
        std::cout << "[Constructor] Allocated buffer at: " << (void*)data << "\n";
    }

    // 1. DESTRUCTOR
    // Cleans up the allocated resource when the object goes out of scope.
    ~DynamicBuffer() {
        std::cout << "[Destructor] Freeing buffer at: " << (void*)data << "\n";
        delete[] data;
    }

    // 2. COPY CONSTRUCTOR
    // Handles deep copying for pass-by-value or explicit initialization.
    DynamicBuffer(const DynamicBuffer& other) {
        size = other.size;
        data = new char[size + 1];
        std::strcpy(data, other.data);
        std::cout << "[Copy Constructor] Deep copied buffer to new address: " << (void*)data << "\n";
    }

    // 3. COPY ASSIGNMENT OPERATOR
    // Handles reassigning an existing object to another existing object (e.g., b2 = b1).
    // Uses the robust "Copy-and-Swap" idiom for strong exception safety.
    DynamicBuffer& operator=(const DynamicBuffer& other) {
        std::cout << "[Copy Assignment] Copying via copy-and-swap\n";
        if (this != &other) {
            DynamicBuffer temp(other); // Make a copy of the source data
            std::swap(data, temp.data); // Swap our internal data with the temp copy
            std::swap(size, temp.size);
        } // temp goes out of scope here, safely cleaning up our old buffer
        return *this;
    }

    // 4. MOVE CONSTRUCTOR (C++11 Rule of 5)
    // "Steals" the resources from a temporary rvalue object instead of deep copying.
    DynamicBuffer(DynamicBuffer&& other) noexcept {
        std::cout << "[Move Constructor] Stealing resources from: " << (void*)other.data << "\n";
        data = other.data;  // Point directly to the existing memory
        size = other.size;

        other.data = nullptr; // Null out the source object so its destructor doesn't free our memory
        other.size = 0;
    }

    // 5. MOVE ASSIGNMENT OPERATOR (C++11 Rule of 5)
    // Safely handles moving resources in an assignment context (e.g., b2 = std::move(b1)).
    DynamicBuffer& operator=(DynamicBuffer&& other) noexcept {
        std::cout << "[Move Assignment] Moving resources\n";
        if (this != &other) {
            delete[] data; // Clean up our own existing resource first

            data = other.data; // Steal the source resources
            size = other.size;

            other.data = nullptr; // Leave the source object in a safe, empty state
            other.size = 0;
        }
        return *this;
    }

    void print() const {
        if (data) std::cout << "Buffer Content: " << data << "\n";
        else std::cout << "Buffer Content: [Empty/Moved]\n";
    }
};


// main code harness:
int main(int argc, char** argv) {
    std::cout << "--- 1 & 2: Testing Constructor & Copy Constructor ---\n";
    DynamicBuffer b1("Hello");
    DynamicBuffer b2 = b1; // Copy Constructor

    std::cout << "\n--- 3: Testing Copy Assignment ---\n";
    DynamicBuffer b3("World");
    b3 = b1; // Copy Assignment

    std::cout << "\n--- 4: Testing Move Constructor ---\n";
    // std::move casts b2 to an rvalue, forcing C++ to use the move constructor
    DynamicBuffer b4 = std::move(b2); 
    b4.print();
    b2.print(); // b2 is now safely empty, no longer owning the pointer

    std::cout << "\n--- 5: Testing Move Assignment ---\n";
    DynamicBuffer b5("Temporary");
    b5 = std::move(b3); // Move Assignment
    
    std::cout << "\n--- Scope Ending: Destructors will clean up remaining objects ---\n";
    return 0;
}
