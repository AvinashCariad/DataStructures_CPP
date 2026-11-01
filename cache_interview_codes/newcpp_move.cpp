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

*/


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
