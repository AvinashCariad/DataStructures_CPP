#include <iostream>
#include <vector>
#include <string>

/*
In the code above, the variables outerIt and innerIt behave like pointers conceptually, but they are actually iterator objects implemented by the standard library.
The size of these iterators depends entirely on your system's architecture (32-bit vs. 64-bit) and the specific compiler/standard library implementation you are using. On standard modern 64-bit systems, a raw pointer is 8 bytes, but C++ std::vector iterators often wrap that raw pointer inside a lightweight class wrapper.
Here is the exact breakdown for a standard 64-bit system (like GCC/Clang on Linux or macOS, and MSVC on Windows):


Target Architecture	Address Bus Width	Raw Pointer Size	outerIt Size	innerIt Size
32-bit (x86 / ARM32)	32 bits	4 bytes	4 bytes	4 bytes
64-bit (x64 / ARM64)	64 bits	8 bytes	8 bytes	8 bytes


*/

// main code harness:
int main(int argc, char** argv) {
    // A vector containing vectors of strings
    std::vector<std::vector<std::string>> grid = {
        {"Apple", "Banana", "Cherry"},
        {"Dog", "Elephant"},
        {"C++", "Java", "Python", "Go"}
    };

    // 1. Explicit Type Syntax (Traditional Way)
    std::cout << "--- Using Explicit Types ---\n";
    
    // Outer loop: iterates over the vector of vectors
    for (std::vector<std::vector<std::string>>::const_iterator outerIt = grid.cbegin(); 
         outerIt != grid.cend(); ++outerIt) {
        
        // Inner loop: *outerIt is a const std::vector<std::string>&
        for (std::vector<std::string>::const_iterator innerIt = outerIt->cbegin(); 
             innerIt != outerIt->cend(); ++innerIt) {
            
            // *innerIt is a const std::string&
            std::cout << *innerIt << " ";
        }
        std::cout << "\n";
    }

    // 2. Modern C++ Syntax (Clean & Recommended Way)
    std::cout << "\n--- Using auto Keyword (C++11 and later) ---\n";
    
    // Using auto with cbegin()/cend() automatically deduces the const_iterator type
    for (auto outerIt = grid.cbegin(); outerIt != grid.cend(); ++outerIt) {
        for (auto innerIt = outerIt->cbegin(); innerIt != outerIt->cend(); ++innerIt) {
            std::cout << *innerIt << " ";
        }
        std::cout << "\n";
    }


    // Also what would be the size of the pointers:
    std::cout << "Outer iterator size: "
              << sizeof(std::vector<std::vector<std::string>>::const_iterator)
              << " bytes\n";

    std::cout << "Inner iterator size: "
              << sizeof(std::vector<std::string>::const_iterator)
              << " bytes\n";

    std::cout << "Raw pointer size:    "
              << sizeof(void*)
              << " bytes\n";

    return 0;
}

