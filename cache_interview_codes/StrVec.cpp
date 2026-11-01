#ifndef STRVEC_H
#define STRVEC_H

#define _MUTABLE_ALLOCATOR // Compatibility fix for some environments
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <initializer_list>
#include <algorithm>

// The following code has been genearted from Google AI
// g++ -std=c++11 main.cpp StrVec.cpp -o strvec_demo
// ./strvec_demo
// compare this code with Vec.h

class StrVec {
public:
    // Constructors & Destructor
    StrVec();                               // Default constructor
    StrVec(std::initializer_list<std::string>); // Initializer-list constructor
    StrVec(const StrVec&);                  // Copy constructor
    StrVec(StrVec&&) noexcept;               // Move constructor
    StrVec& operator=(const StrVec&);       // Copy assignment
    StrVec& operator=(StrVec&&) noexcept;    // Move assignment
    ~StrVec();                              // Destructor

    // Public Member Functions
    void push_back(const std::string&);     // Copy push_back
    void push_back(std::string&&);          // Move push_back
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    std::string* begin() const { return elements; }
    std::string* end() const { return first_free; }
    
    // Subscript operators
    std::string& operator[](size_t n) { return elements[n]; }
    const std::string& operator[](size_t n) const { return elements[n]; }

private:
    static std::allocator<std::string> alloc; // Allocates the elements
    
    // Utilities used by the copy control members
    void chk_n_alloc() { if (size() == capacity()) reallocate(); }
    std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
    void free();            // Destroy elements and deallocate space
    void reallocate();      // Get more space and move existing elements

    std::string *elements;   // Pointer to the first element in the array
    std::string *first_free; // Pointer to the first free element in the array
    std::string *cap;        // Pointer to one past the end of the array
};

#endif

//#include "StrVec.h"


// Static member definition
std::allocator<std::string> StrVec::alloc;

// Default Constructor
StrVec::StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}

// Initializer-list Constructor
StrVec::StrVec(std::initializer_list<std::string> il) {
    auto newdata = alloc_n_copy(il.begin(), il.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

// Copy Constructor
StrVec::StrVec(const StrVec &s) {
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

// Move Constructor (noexcept is crucial for vector performance)
StrVec::StrVec(StrVec &&s) noexcept 
    : elements(s.elements), first_free(s.first_free), cap(s.cap) {
    // Leave s in a state where it is safe to destroy
    s.elements = s.first_free = s.cap = nullptr;
}

// Destructor
StrVec::~StrVec() { 
    free(); 
}

// Copy Assignment Operator
StrVec& StrVec::operator=(const StrVec &rhs) {
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

// Move Assignment Operator
StrVec& StrVec::operator=(StrVec &&rhs) noexcept {
    // Direct protection against self-assignment
    if (this != &rhs) {
        free(); // Get rid of existing elements
        elements = rhs.elements; // Take over resources
        first_free = rhs.first_free;
        cap = rhs.cap;
        // Reset rhs
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}

// Push Back (Copy version)
void StrVec::push_back(const std::string &s) {
    chk_n_alloc(); // Ensure space exists
    alloc.construct(first_free++, s);
}

// Push Back (Move version)
void StrVec::push_back(std::string &&s) {
    chk_n_alloc();
    alloc.construct(first_free++, std::move(s));
}

// Helper: Allocate space and copy a range
std::pair<std::string*, std::string*> 
StrVec::alloc_n_copy(const std::string *b, const std::string *e) {
    auto data = alloc.allocate(e - b);
    return {data, std::uninitialized_copy(b, e, data)};
}

// Helper: Free resources
void StrVec::free() {
    if (elements) {
        // Destroy the old elements in reverse order
        for (auto p = first_free; p != elements; /* empty */) {
            alloc.destroy(--p);
        }
        alloc.deallocate(elements, cap - elements);
    }
}

// Helper: Reallocate capacity (doubling strategy)
void StrVec::reallocate() {
    // Compute new capacity (double size, or 1 if empty)
    auto newcapacity = size() ? 2 * size() : 1;
    auto first = alloc.allocate(newcapacity);
    
    // Move elements from old space to new space
    auto dest = first;
    auto elem = elements;
    for (size_t i = 0; i != size(); ++i) {
        alloc.construct(dest++, std::move(*elem++));
    }
    
    free(); // Free old space now that items are moved
    
    // Update data structures to point to new space
    elements = first;
    first_free = dest;
    cap = elements + newcapacity;
}


// Helper function to print the current state of a StrVec
void printVec(const std::string& label, const StrVec& vec) {
    std::cout << label << " | Size: " << vec.size() 
              << ", Capacity: " << vec.capacity() << " | Elements: [ ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << "\"" << *it << "\" ";
    }
    std::cout << "]\n";
}


// main code harness:
int main(int argc, char** argv) {
    std::cout << "--- 1. Testing Default Constructor & push_back ---\n";
    StrVec vec1;
    printVec("vec1 (init)", vec1);

    // Testing dynamic reallocation (doubling strategy)
    vec1.push_back("Hello");
    vec1.push_back("World");
    vec1.push_back("C++");
    vec1.push_back("Primer");
    printVec("vec1 (after push_backs)", vec1);

    std::cout << "\n--- 2. Testing Initializer List Constructor ---\n";
    StrVec vec2 = {"Barbara", "Moo", "Stanley", "Lippman"};
    printVec("vec2", vec2);

    std::cout << "\n--- 3. Testing Copy Semantics (Deep Copy) ---\n";
    StrVec vec3 = vec2; // Copy constructor
    vec3[0] = "Dr.";    // Modify copy to prove independence
    printVec("vec2 (original)", vec2);
    printVec("vec3 (copied & mutated)", vec3);

    std::cout << "\n--- 4. Testing Move Semantics (Resource Transfer) ---\n";
    printVec("vec1 before move", vec1);
    
    // Move constructor
    StrVec vec4 = std::move(vec1); 
    std::cout << ">> Moved vec1 into vec4\n";
    printVec("vec4 (destination)", vec4);
    printVec("vec1 (source - now safe/empty)", vec1);

    std::cout << "\n--- 5. Testing Assignment Operators ---\n";
    StrVec vec5;
    vec5 = vec2; // Copy assignment
    printVec("vec5 (after copy assignment)", vec5);

    StrVec vec6;
    vec6 = std::move(vec2); // Move assignment
    std::cout << ">> Move assigned vec2 into vec6\n";
    printVec("vec6 (destination)", vec6);
    printVec("vec2 (source - now safe/empty)", vec2);

    return 0;
}
