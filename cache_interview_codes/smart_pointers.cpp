#include <iostream>
#include <memory>

class Resource {
public:
    Resource()  { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed automatically\n"; }
    void doWork() { std::cout << "Unique pointer working\n"; }
};


// main code harness:
int main(int argc, char** argv) {
   /*
   A std::unique_ptr owns its resource exclusively. No two unique pointers can point to the same memory space.
   • Properties: It cannot be copied, only moved (via std::move). It has zero runtime
     overhead compared to a raw pointer.
   • Best Practice: Always use std::make_unique (C++14) to instantiate them
   
   */

    // Creating a unique_ptr
    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>();
    ptr1->doWork();

    // std::unique_ptr<Resource> ptr2 = ptr1; // ERROR: Copying is forbidden!

    // Transferring ownership using move semantics
    std::unique_ptr<Resource> ptr2 = std::move(ptr1); 
    if (!ptr1) {
        std::cout << "ptr1 is now null, ownership moved to ptr2\n";
    }

    /*
    
    • Properties: It maintains an internal reference count 
      (tracked in a separate hidden control block). Every time
       a new shared_ptr copies it, the count goes up. When a shared_ptr
       is destroyed, the count goes down. The actual heap memory is deleted
        only when the reference count drops to exactly zero.
    • Best Practice: Always use std::make_shared for speed and memory efficiency.
    
    */

    // Create a shared resource (Reference Count = 1)
    std::shared_ptr<int> shared1 = std::make_shared<int>(42);
    std::cout << "Initial Reference Count: " << shared1.use_count() << "\n";

    {
        // Copy the shared pointer (Reference Count = 2)
        std::shared_ptr<int> shared2 = shared1;
        std::cout << "Inside block Count: " << shared1.use_count() << "\n";
        std::cout << "Value via shared2: " << *shared2 << "\n";
    } // shared2 goes out of scope here (Reference Count drops back to 1)
    
    
    /*
    • Properties: It does not increment the reference count. It is primarily used to
     break cyclic dependencies (where Object A points to Object B, and Object B
     points to Object A, preventing both from ever being deleted).
    
     • Accessing Data: Because it doesn't own the object, it cannot access the data
      directly. It must be temporarily converted back into a std::shared_ptr using
      the .lock() method to ensure the object hasn't already been deleted.
    
    */
    std::weak_ptr<int> weak;
    {
        std::shared_ptr<int> shared = std::make_shared<int>(100);
        weak = shared; // weak observes shared, reference count stays at 1

        // Accessing data: lock() succeeds because 'shared' is still alive
        if (std::shared_ptr<int> tempShared = weak.lock()) {
            std::cout << "Value is still valid: " << *tempShared << "\n";
        }
    } // 'shared' goes out of scope, heap data is deleted.

    // Accessing data after deletion: lock() fails and returns nullptr
    if (std::shared_ptr<int> tempShared = weak.lock()) {
        std::cout << "This won't print.\n";
    } else {
        std::cout << "Resource is gone. weak_ptr safely detected expiration!\n";
    }


    return 0; 
} // ptr2 goes out of scope here: Resource is safely destroyed.

