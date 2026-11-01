#include <bits/stdc++.h>
#include <iostream>
#include <vector>

using namespace std;


int op_increase (int i) { return ++i; }
int op_sum (int i, int j) { return i+j; }

std::string name;
double price;

Product(std::string n, double p) : name(n), price(p) {}


class Account {
public:
    double balance;
    Account(double b) : balance(b) {}

    // Apply interest rate in-place
    void applyInterest(double rate) {
        balance += balance * rate;
    }
};



// main code harness:
int main(int argc, char** argv) {
    vector<int> v1 = {1, 5, 6, 8};
    vector<int> v2(v1.size());
    // Illustrating back_inserter in cpp:
    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = {4, 5, 6};
    // back_inserter:
    copy(v1.begin(), v1.end(), back_inserter(v2));     // use of back_inserter

    for (auto x : v2)
        cout << x << " ";

    // Incrementing all elements of vector by 1
    transform(v1.begin(), v1.end(), v2.begin(),
              [](int a) { 
                return a + 1;
              });

    for (auto i : v2)
        cout << i << " ";
        cout << endl;
    

    //===============================================
    vector<int> first;
    vector<int> second;
    vector<int>::iterator it;

    // set some values:
    for (int i=1; i<6; i++) first.push_back (i*10); //  first: 10 20 30 40 50

    second.resize(first.size());     // allocate space
    transform (first.begin(), first.end(), second.begin(), op_increase);
                                                  // second: 11 21 31 41 51
    cout << "first contains:";
    for (it=first.begin(); it!=first.end(); ++it)
        cout << " " << *it;


    transform (first.begin(), first.end(), second.begin(), first.begin(), op_sum);
                                                  //  first: 21 41 61 81 101

    cout << "first contains:";
    for (it=first.begin(); it!=first.end(); ++it)
        cout << " " << *it;

    cout << endl;

    // A vector of Product class instances
    std::vector<Product> store = {
        {"Apple", 1.25},
        {"Banana", 0.75},
        {"Coffee", 4.50}
    };

    std::vector<double> prices;

    // Use std::transform to extract the price of each Product instance
    std::transform(
        store.begin(), 
        store.end(), 
        std::back_inserter(prices), 
        [](const Product& p) {
            return p.price; // Return a member property
        }
    );

    // Print the extracted prices
    for (double pr : prices) {
        std::cout << pr << " "; // Output: 1.25 0.75 4.5
    }


    // Another example:

    std::vector<Account> accounts = { Account(100.0), Account(200.0), Account(300.0) };

    // Apply a 5% interest rate to each Account instance in-place
    std::transform(
        accounts.begin(), 
        accounts.end(), 
        accounts.begin(), 
        [](Account acc) {
            acc.applyInterest(0.05);
            return acc;
        }
    );

    for (const auto& acc : accounts) {
        std::cout << acc.balance << " "; // Output: 105 210 315
    }


    return 0;
}
