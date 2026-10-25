#include <stdlib.h>
#include <iostream>
#include <stack>
#include <exception>

using namespace std;

#define MAX_NUM_STACK_ELEMS (100)
#define MAX_VALUE   (10)
#define STATUS_OK (0)

void handle_error(void)
{
    cout << "Test failed\n";
    exit(1);
}


// add element:
void add_element(stack<int>& main_stack, stack<int>& min_stack, int data_to_add)
{
    main_stack.push(data_to_add);

    if (min_stack.empty() || data_to_add <= min_stack.top()) {
        min_stack.push(data_to_add);
    }
}

int remove_element(stack<int>& main_stack, stack<int>& min_stack)
{
    if (main_stack.empty())
        throw exception();

    int popped_element = main_stack.top();
    main_stack.pop();

    int min_val = min_stack.top();

    if (popped_element == min_val)
        min_stack.pop();

    return popped_element;
}

int find_smallest(int number_array[], int max_index)
{
    if (max_index < 0) return 0xFFFFFFF;
    int smallest = number_array[0];

    for (int i = 1; i <= max_index; ++i) {
        if (smallest > number_array[i]) {
            smallest = number_array[i];
        }
    }
    return smallest;
}

void test_stack()
{
    int min_val;
    int num_elems, j;
    int number_array[MAX_NUM_STACK_ELEMS];
    int smallest_element;

    /*Test for different sizes of the stacks*/
    for (num_elems = 0; num_elems <= MAX_NUM_STACK_ELEMS; ++num_elems) {

        // FIXED: Re-instantiate fresh stacks for every loop iteration
        stack<int> main_stack;
        stack<int> min_stack;

        for (j = 0; j < num_elems; ++j) {
            number_array[j] = rand() % MAX_VALUE;
         
            add_element(main_stack, min_stack, number_array[j]);

            min_val = min_stack.top();

            smallest_element = find_smallest(number_array, j);
            if (min_val != smallest_element) {
                handle_error();
            }
        }

        for (j = 0; j < num_elems; ++j) {
            min_val = min_stack.top();

            // Verifies the remaining sub-segment of array values matches the min_stack
            smallest_element = find_smallest(number_array, num_elems - 1 - j);
            if (min_val != smallest_element) {
                handle_error();
            }

            remove_element(main_stack, min_stack);
        }

        if (!min_stack.empty() || !main_stack.empty())
            handle_error();

        bool caught_exception = false;
        try {
            remove_element(main_stack, min_stack);
        } catch (const exception& e) { // FIXED: Catch by reference
            caught_exception = true;
        }

        if (!caught_exception)
            handle_error();
    }

    cout << "Test passed \n";
}


// main code harness:
int main(int argc, char** argv)
{
    test_stack();
    return 0;
}
