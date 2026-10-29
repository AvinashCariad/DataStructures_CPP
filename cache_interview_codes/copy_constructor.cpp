#include <iostream>
#include <cstring>

class StringWrapper {
private:
    char* buffer;

public:
    // Regular Constructor
    StringWrapper(const char* str) {
        buffer = new char[std::strlen(str) + 1];
        std::strcpy(buffer, str);
        std::cout << "Object constructed at buffer address: " << (void*)buffer << "\n";
    }

    // --- WE REMOVED THE CUSTOM DEEP COPY CONSTRUCTOR ---
    // The compiler will now generate a default copy constructor.
    // It performs a SHALLOW COPY: `this->buffer = other.buffer;`

    // Destructor to clean up heap memory
    ~StringWrapper() {
        std::cout << "Destructor called for buffer address: " << (void*)buffer << "\n";
        delete[] buffer; // This line will execute twice on the same pointer!
    }

    void print() const {
        std::cout << "String value: " << buffer << "\n";
    }
};

// A function that forces a shallow copy by accepting an object by value
void simulateShallowCopyError() {
    std::cout << "\n=== Starting Simulation ===\n";
    
    StringWrapper original("Hello C++");         // this calls the constructor
    
    {
        std::cout << "\n--- Creating a shallow copy inside an inner block ---\n";
        StringWrapper copy = original; // Default shallow copy occurs here!
        
        std::cout << "Original "; original.print();
        std::cout << "Copy     "; copy.print();
        
        std::cout << "\n--- Exiting inner block (copy goes out of scope) ---\n";
    } // <-- 'copy' is destroyed here. The heap memory is deleted!

    std::cout << "\n--- Back in the outer scope --- \n";
    // CRASH / UNDEFINED BEHAVIOR:
    // 1. 'original.buffer' is now a dangling pointer pointing to deleted memory.
    // 2. When 'simulateShallowCopyError' ends, 'original' will try to delete the memory again.
    std::cout << "Attempting to access original after copy was destroyed:\n";
    original.print(); 
    
    std::cout << "\n=== Exiting Simulation (original goes out of scope) ===\n";
}


// main code harness:
int main(int argc, char** argv) {
    simulateShallowCopyError();
    return 0;
}




/*
// correct version of the code will be:

#include <iostream>
#include <cstring>

class StringWrapper {
private:
    char* buffer;

public:
    // Regular Constructor
    StringWrapper(const char* str) {
        buffer = new char[std::strlen(str) + 1];
        std::strcpy(buffer, str);
    }

    // Custom Copy Constructor (Deep Copy)
    StringWrapper(const StringWrapper& other) {
        // 1. Allocate brand new memory
        buffer = new char[std::strlen(other.buffer) + 1];
        // 2. Copy the actual data, not just the pointer address
        std::strcpy(buffer, other.buffer);
        std::cout << "Custom copy constructor called!\n";
    }

    // Destructor to clean up heap memory
    ~StringWrapper() {
        delete[] buffer;
    }
};

int main() {
    StringWrapper obj1("Hello C++");
    StringWrapper obj2 = obj1; // Invokes the deep copy constructor safely
    return 0;
}














*/