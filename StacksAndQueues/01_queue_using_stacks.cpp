#include <stdlib.h>
#include <iostream>
#include <stack>
#include <exception>

using namespace std;

template <class T> 
class QueueUsingStacks 
{
private:
    stack<T> s1;
    stack<T> s2;

    // Helper method to shift elements from s1 to s2 when s2 is empty
    void fill_s2_if_empty() {
        if (s2.empty()) {
            while (!s1.empty()) {
                T e = s1.top();
                s1.pop();
                s2.push(e);
            }
        }
    }

public:
    QueueUsingStacks() {}

    void add(T new_element) {
        /* Add elements only to stack s1 */
        s1.push(new_element);
    }

    T remove() {
        fill_s2_if_empty();

        if (s2.empty())
            throw exception();
        
        /* If s2 is not empty, remove the element from the top of s2.
           This element corresponds to the head of the queue */
        T e = s2.top();
        s2.pop();
        return e;
    }

    // Returns the front element without removing it (Standard C++ style)
    T front() {
        fill_s2_if_empty();

        if (s2.empty())
            throw exception();

        return s2.top();
    }

    // Alias for front() (Common alternative naming style)
    T peek() {
        return front();
    }

    bool empty() {
        /* Queue is empty only if both stacks are empty */
        return s1.empty() && s2.empty();
    }
};

int MAX_NUM_QUEUE_ELEMS = 10;

void handle_error() 
{
    cout << "Test failed" << endl;
    exit(1);
}

void test() 
{
    int num_elems, j, expected_val;
    int output_peek, output_front, output_remove;

    /* Test for different number of elements in the queue */
    for (num_elems = 0; num_elems <= MAX_NUM_QUEUE_ELEMS; ++num_elems) {
        
        // Declared INSIDE the loop to reset state for each test run
        QueueUsingStacks<int> q; 

        /* Add all elements to the queue */
        for (j = 0; j < num_elems; ++j) {
            q.add(j);
        }

        cout << "Queue size = " << num_elems << ", Elements are : ";

        /* Remove one element at a time from the queue and print it */
        for (j = 0; j < num_elems; ++j) {
            expected_val = j;

            // Verify peek() and front() match the expected element before deletion
            output_peek = q.peek();
            output_front = q.front();
            
            if (output_peek != expected_val || output_front != expected_val) {
                handle_error();
            }

            // Perform the actual removal
            output_remove = q.remove();
            if (output_remove != expected_val) {
                handle_error();
            }
            cout << output_remove << " ";
        }

        /* Queue is now empty. Trying to peek or remove should raise an exception */
        bool caught_exception = false;
        try {
            q.remove();
        } catch (const exception& e) { // Catching by const reference prevents slicing
            caught_exception = true; 
        }
    
        if (!caught_exception)
            handle_error();

        cout << endl;
        cout << "__________________________________________" << endl;   
    }
}

// Main code harness
int main(int argc, char** argv) 
{
    test();
    cout << "All tests passed successfully!" << endl;
    return 0;
}
