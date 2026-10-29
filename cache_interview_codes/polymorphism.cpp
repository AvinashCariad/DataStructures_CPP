#include <iostream>
#include <vector>
#include <memory>
#include <string>


/*


The Two Main Types of Polymorphism in C++

C++ splits polymorphism into two distinct categories based on when the decision of which function to run is made:

1. Compile-Time Polymorphism (Static Binding)

The decision about which function to execute happens while the code is being compiled. It is fast, has zero runtime overhead, and is achieved through:
• Function Overloading: Having multiple functions with the same name but different parameters.
• Operator Overloading: Giving custom meaning to standard operators (like + or <<) for user-defined classes.
• Templates: Writing generic functions or classes that adapt to different data types at compile time.

2. Runtime Polymorphism (Dynamic Binding)

The decision about which function to execute is postponed until the program is actually running. This is what most people mean when they simply say "polymorphism." It requires three specific ingredients:
1. A Base Class with a virtual function.
2. Derived Classes that override that virtual function.
3. A Base Class Pointer or Reference pointing to a derived class object

*/


// 1. Base Class (Abstract Class)
class PaymentMethod {
protected:
    double amount;

public:
    PaymentMethod(double amt) : amount(amt) {}
    virtual ~PaymentMethod() = default; // Essential for polymorphism

    // Pure virtual function acting as an interface contract
    virtual void processPayment() const = 0; 
};

// 2. Derived Class: Credit Card
class CreditCardPayment : public PaymentMethod {
private:
    std::string cardNumber;

public:
    CreditCardPayment(double amt, std::string cardNum) 
        : PaymentMethod(amt), cardNumber(cardNum) {}

    void processPayment() const override {
        std::cout << "Processing Credit Card payment of $" << amount 
                  << " using card ending in " << cardNumber.substr(cardNumber.length() - 4) << ".\n";
    }
};

// 3. Derived Class: PayPal
class PayPalPayment : public PaymentMethod {
private:
    std::string email;

public:
    PayPalPayment(double amt, std::string userEmail) 
        : PaymentMethod(amt), email(userEmail) {}

    void processPayment() const override {
        std::cout << "Processing PayPal payment of $" << amount 
                  << " for user " << email << ".\n";
    }
};

// 4. Derived Class: Bitcoin
class BitcoinPayment : public PaymentMethod {
private:
    std::string walletAddress;

public:
    BitcoinPayment(double amt, std::string wallet) 
        : PaymentMethod(amt), walletAddress(wallet) {}

    void processPayment() const override {
        std::cout << "Processing Bitcoin payment of $" << amount 
                  << " to wallet: " << walletAddress << "\n";
    }
};

// 5. Client Application / Demonstration - main code harness:
int main(int argc, char** argv) {
    // A digital wallet or shopping cart managing multiple pending transactions.
    // We use std::unique_ptr for automated memory safety, storing base class pointers.
    std::vector<std::unique_ptr<PaymentMethod>> checkoutLine;

    // The vector seamlessly accommodates different concrete classes under the uniform "PaymentMethod" type
    checkoutLine.push_back(std::make_unique<CreditCardPayment>(150.75, "1234567890123456"));
    checkoutLine.push_back(std::make_unique<PayPalPayment>(45.00, "customer@email.com"));
    checkoutLine.push_back(std::make_unique<BitcoinPayment>(1200.00, "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"));

    std::cout << "=== Processing Checkout Queue ===\n";

    // Polymorphism in action: The loop handles all objects completely uniformly
    for (const auto& payment : checkoutLine) {
        // C++ checks the actual type at runtime and executes the correct overridden method
        payment->processPayment(); 
    }

    return 0;
}

