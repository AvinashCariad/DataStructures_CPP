#include <iostream>
#include <memory>

/*

Key Takeaways

• The virtual Keyword: You only need to declare it in the base class. Derived classes automatically inherit the virtual nature of the function, though explicitly using the override keyword in C++11 is highly recommended to catch typos at compile time.
• The vptr and vtable: Under the hood, any class with a virtual function gets a hidden pointer called a vptr (virtual pointer). This pointer points to a table of function pointers called a vtable (virtual table). When animal1->makeNoise() is called, the program looks up the correct function address inside the vtable at runtime.
• Virtual Destructors: If a class has virtual functions, its destructor must be virtual. If it isn't, deleting a derived object via a base class pointer (like delete animal1; above) will cause undefined behavior and fail to clean up the derived class memory.
Would you like to look closer at how the hidden vptr and vtable affect the sizeof an object in memory, or should we talk about pure virtual functions and abstract classes?


*/
class Animal {
public:
    // A virtual function - can be overridden by derived classes
    virtual void makeNoise() const {
        std::cout << "Generic animal sound\n";
    }

    // A regular (non-virtual) function
    void sleep() const { 
        std::cout << "Animal is sleeping\n"; 
    }

    // Best practice: Always provide a virtual destructor for base classes
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    // Overriding the virtual function
    void makeNoise() const override {
        std::cout << "Woof! Woof!\n";
    }

    // Redefining a non-virtual function (Not recommended, known as function hiding)
    void sleep() const {
        std::cout << "Dog is snoring\n";
    }
};

class Cat : public Animal {
public:
    void makeNoise() const override {
        std::cout << "Meow!\n";
    }
};


// main code harness:
int main(int argc, char** argv) {
    // Create base class pointers pointing to derived class objects
    Animal* animal1 = new Dog();
    Animal* animal2 = new Cat();

    std::cout << "--- Testing Virtual Function (Runtime Polymorphism) ---\n";
    // C++ checks the actual object type at runtime and calls the correct version
    animal1->makeNoise(); // Outputs: Woof! Woof!
    animal2->makeNoise(); // Outputs: Meow!

    std::cout << "\n--- Testing Non-Virtual Function (Compile-time Binding) ---\n";
    // Because sleep() is NOT virtual, the compiler looks ONLY at the pointer type (Animal*)
    animal1->sleep(); // Outputs: Animal is sleeping (Even though it's actually a Dog!)

    // Clean up memory
    delete animal1;
    delete animal2;

    return 0;
}



/*
// A pure virtual function is a virtual function in C++ that has no implementation in the base class and is assigned = 0 in its declaration.
// It acts as a placeholder that forces any derived class to provide its own implementation of the function.


#include <iostream>

class Shape {
public:
    // Pure virtual function
    virtual void draw() const = 0; 
    
    virtual ~Shape() = default; // Good practice: virtual destructor
};

class Circle : public Shape {
public:
    // Must override the pure virtual function, or Circle is also abstract
    void draw() const override {
        std::cout << "Drawing a Circle\n";
    }
};

class Square : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a Square\n";
    }
};

int main() {
    // Shape s; // ERROR! Cannot instantiate an abstract class.

    Shape* s1 = new Circle();
    Shape* s2 = new Square();

    s1->draw(); // Outputs: Drawing a Circle
    s2->draw(); // Outputs: Drawing a Square

    delete s1;
    delete s2;
    return 0;
}









*/