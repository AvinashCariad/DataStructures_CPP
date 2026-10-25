/*

1. Time ComplexityBecause this design uses an unoptimized push / costly pop strategy, 
the operations break down as follows: push(T new_element): \(O(1)\)Adding an element simply 
appends it directly to the end of q1. This is a constant-time operation. 
pop(): \(O(K)\) (where \[K\] is the number of elements currently in the stack)To get 
to the last element inserted (the top of the stack), the algorithm must shift \(K-1\) 
elements out of q1 and push them into q2 before it can extract the final element.
If you perform \[N\] sequential pops on a stack of size \[N\], the total time 
spent is \(N + (N-1) + (N-2) + \dots + 1\), leading to a worst-case total time of 
\(O(N^2)\). empty(): \(O(1)\)Checking if the two queues are empty takes constant time. 


*/


#include <stdlib.h>
#include <iostream>
#include <queue>
#include <exception>

using namespace std;

template <class T> class StackUsingQueues 
{
    private:
    queue<T> *q1;
    queue<T> *q2;

    public:
    StackUsingQueues() { 
        q1 = new queue<T>();
        q2 = new queue<T>();
    }

    ~StackUsingQueues() {
        delete q1;
        delete q2;
    }

    bool empty() {
        /*Stack is empty only if both queues are empty*/
        return q1->empty() && q2->empty();
    }

    void push(T new_element) {
        /*Add element to the end of queue q1*/
        q1->push(new_element);
    }

    T pop() {
        /*If stack is empty, then throw an exception*/
        if (empty())
            throw exception();

        T e;
        /*Remove all elements from q1 and add it to q2 except the last item*/
        while (q1->size() > 1) {
            e = q1->front();
            q1->pop();
            q2->push(e);
        }

        /*Remove the last element in q1. It will contain the top of stack*/
        e = q1->front();
        q1->pop();

        /*Swap the queues q1 and q2*/
        // Think why this is needed
        queue<T> *temp = q1;
        q1 = q2;
        q2 = temp;

        return e; /*Return the top of the stack*/
    }
};

int MAX_NUM_ELEMS = 10;

void handle_error() 
{
    cout << "Test failed" << endl;
    exit(1);
}

void test() 
{
    int num_elems, j, expected_val;
    int output;

    /*Test for different number of elements in the stack*/
    for (num_elems = 0; num_elems <= MAX_NUM_ELEMS; ++num_elems) {

        // FIXED: Declared inside the loop to clear state every test iteration
        StackUsingQueues<int> s;

        /*Add all the elements to the stack*/
        for (j = 0; j < num_elems; ++j) {
            s.push(j);
        }

        cout << "Stack size = " << num_elems << ", Elements are : " << endl;

        /*Remove one element at a time from the stack and print it*/
        for (j = 0; j < num_elems; ++j) {
            output = s.pop();

            expected_val = num_elems - 1 - j;
            if (output != expected_val) {
                handle_error();
            }
            cout << output << endl;
        }

        /*Stack is now empty. Trying to remove an element now raises an exception*/
        bool caught_exception = false;
        try {
            s.pop();
        } catch (const exception& e) { // FIXED: Catch by reference to avoid slicing bugs
            caught_exception = true; 
        }
    
        if (!caught_exception)
            handle_error();

        cout << "__________________________________________" << endl;   
    }
}



// main code harness:
int main(int argc, char** argv) 
{
    test();
    cout << "Test passed " << endl;
    return 0;
}
