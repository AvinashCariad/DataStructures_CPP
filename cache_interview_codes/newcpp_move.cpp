#include <iostream>
#include <string>
#include <utility> // Required for std::move

/*
• The target type must support move semantics: If you use std::move on a type that
doesn't have a move constructor (like a plain int or a custom class without one),
the compiler will silently fall back to a standard copy operation.

• const objects cannot be moved: If you attempt to std::move a const std::string, 
it cannot be modified to transfer its pointers. The compiler will safely, but slowly, 
fall back to a copy.

• Post-move state is unspecified: After an object is moved from, it remains in a valid
but unspecified state. You should never read from or use its value unless you re-assign
a new value to it first.

• Don't std::move return values: Avoid doing return std::move(local_var); inside functions.
This actively destroys Return Value Optimization (RVO), a compiler optimization that
eliminates the move/copy entirely by constructing the object directly in the destination
memory

By default, copying a large object duplicates everything in memory, which is slow. 
Moving cuts the expensive copy out of the equation. Instead of copying the data, the
new object simply steals (takes ownership of) the existing pointer from the temporary object,
and zeroes out the temporary object's pointer so it cleanly dies empty

• std::move doesn't actually move anything: It is essentially a cast that changes an
 lvalue (an object with a name) into an rvalue (an anonymous temporary reference). 
 It simply grants permission to the compiler to call the Move Constructor instead of the
Copy Constructor.

• Zeroing out is required: Inside the move constructor, you must set the old object’s pointer
 to nullptr. If you forget, the temporary object's destructor will call delete[] on the exact
memory you just hijacked, crashing your program or corrupting data.

• Rule of Thumb: Use std::move when passing large data structures (like standard vectors, maps,
 or strings) into functions or assigning them, provided you no longer plan to use the source 
 object afterward.

*/

#include <iostream>
#include <cstring>
#include <utility> // For std::move

class DynamicString {
private:
    char* m_Buffer;     // pointer to array of buffer - way to represent string
    size_t m_Size;

public:
    // Standard Constructor
    DynamicString(const char* string) {
        std::cout << "[Created] Allocating buffer via standard constructor\n";
        m_Size = strlen(string);
        m_Buffer = new char[m_Size + 1];
        memcpy(m_Buffer, string, m_Size + 1);      // use memcpy 
    }

    // Destructor
    ~DynamicString() {
        std::cout << "[Destroyed] Cleaning up buffer\n";
        delete[] m_Buffer;
    }

    // 1. COPY CONSTRUCTOR (The Old, Slow Way)
    // Allocates new heap memory and copies every individual character.
    DynamicString(const DynamicString& other) {
        std::cout << "[Copied] Deep copy allocation! (Expensive)\n";
        m_Size = other.m_Size;
        m_Buffer = new char[m_Size + 1];
        memcpy(m_Buffer, other.m_Buffer, m_Size + 1);
    }

    // 2. MOVE CONSTRUCTOR (The Fast, Efficient Way)
    // Uses an rvalue reference (&&). It intercepts pointers rather than copying characters.
    DynamicString(DynamicString&& other) noexcept {
        std::cout << "[Moved] Stealing pointer ownership! (Fast & Cheap)\n";
        
        // Step 1: Copy the memory address pointer and size directly
        m_Buffer = other.m_Buffer;
        m_Size = other.m_Size;

        // Step 2: Clear the source object's pointers
        // If we don't do this, 'other' will delete our new buffer when its destructor runs!
        other.m_Buffer = nullptr;
        other.m_Size = 0;
    }

    // print functionality:
    void Print() const {
        if (m_Buffer) {
            std::cout << m_Buffer << "\n";
        } else {
            std::cout << "(empty)\n";
        }
    }
};


// main code harness: - test the code here:
int main(int argc, char** argv) {
    std::cout << "--- Creating Initial Object ---\n";
    DynamicString original("Hello World, this is a very long string stored on the heap!");

    std::cout << "\n--- Triggering a Copy ---\n";
    DynamicString copyTarget = original; // Triggers Copy Constructor

    std::cout << "\n--- Triggering a Move via std::move ---\n";
    // std::move explicitly tells the compiler: "I am done with 'original'. Treat it as a temporary rvalue."
    DynamicString moveTarget = std::move(original); // Triggers Move Constructor

    std::cout << "\n--- Verification Status ---\n";
    std::cout << "original: ";  original.Print();  // Outputs: (empty) because its memory was stolen
    std::cout << "moveTarget: "; moveTarget.Print(); // Outputs: Hello World...
    
    std::cout << "\n--- Destruction Phase ---\n";
    // another example on cpp:
    std::string str1 = "Hello World, this is a very long string!";
    
    // std::move(str1) changes the type to std::string&&
    // This triggers the move constructor of std::string
    std::string str2 = std::move(str1);    // moves everything from str1 to str2 

    std::clog << "str2: " << str2 << "\n"; 
    // str1 is now in a "valid but unspecified" state. Usually, it's empty.
    std::clog << "str1: " << str1 << "\n";

    return 0;
}


/*
// main code harness:
int main(int argc, char** argv) {
    std::string str1 = "Hello World, this is a very long string!";
    
    // std::move(str1) changes the type to std::string&&
    // This triggers the move constructor of std::string
    std::string str2 = std::move(str1);    // moves everything from str1 to str2 

    std::clog << "str2: " << str2 << "\n"; 
    // str1 is now in a "valid but unspecified" state. Usually, it's empty.
    std::clog << "str1: " << str1 << "\n"; 
}

*/
