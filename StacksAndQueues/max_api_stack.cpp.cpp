/*
	Design a stack with max functionality. The stack should be able to return the maximum element in O(1).

	Input: numbers for stack
	Output: stack with max api

	Solution:
		To ensure we know about the max element at every instant, we use another stack. That stack tracks the current max element.
		Basically for every push we see if the current top element is greater than the element being pushed or not, if it is
		then we push, else we don't push it. Since any element which occurs after the current max, for it the max is always 
		going to the current max. 

		Now for handling duplicates we need to know their count. This can be done in two ways:
		1.Either we use a stack which int value i.e max and for we see an element which is equal to the top
			then we push it also, so we can account for duplicates. But if our array has only duplicates then it will
			take O(n).
		2. We use a struct {max, freq} as stack element. The freq part is for keeping count of duplicates. Every time a 
			duplicate comes we just decrement its count and for a new push of a duplicate increment its count. But if we have 
			all distinct elements, then we will be using twice the memory compared to way 1.

		TC: O(n), n: no. of elements
		SC: O(n) 
*/


/*
Every single operation in your class runs in \(O(1)\) (Constant Time). 
OperationTime ComplexityWhy?Push(data)\(O(1)\)Standard stack::emplace is \(O(1)\). 
Checking and updating your maxTracker_ stack only involves looking at the top element, 
which takes constant time.Pop()\(O(1)\)Standard stack::pop is \(O(1)\).
Comparing the popped value to the top of maxTracker_ and decrementing its 
frequency is a constant time operation.Max()\(O(1)\)You are simply reading maxTracker_.top().max. 
There are no loops or searches required.IsEmpty()\(O(1)\)It instantly evaluates whether the underlying stack size is zero.

*/



#include <iostream>
#include <vector>
#include <stack>
#include <stdexcept> // 💡 Added for length_error

using namespace std;

template <typename T>
class StackWithMaxAPI {
	private:
		struct MaxTracker {
			T max;
			int freq;
		};

		stack<MaxTracker> maxTracker_;
		stack<T> s_; 

	public:
		void Push(T data) {
			s_.emplace(data);

			if(maxTracker_.empty()) 
				maxTracker_.emplace(MaxTracker{data, 1});
			else if(maxTracker_.top().max <= data) {
				if(maxTracker_.top().max == data) {
					int& count = maxTracker_.top().freq;
					++count;
				}
				else {
					maxTracker_.emplace(MaxTracker{data, 1});
				}
			}			
		}

		T Pop() {
			if(s_.empty())
				throw length_error("Stack is empty");
			
			// 💡 Fixed: Changed from 'int' to 'T' to preserve the template type
			T popped_el = s_.top(); 
			s_.pop();
			
			if(popped_el == maxTracker_.top().max) {
				int& max_el_count =  maxTracker_.top().freq;
			
				if(max_el_count > 1)
					--max_el_count;
				else {
					maxTracker_.pop();
				}
			}
			return popped_el;
		}

		T Max() {
			if(IsEmpty()) {
				throw length_error("Stack is empty");
			}
			return maxTracker_.top().max;
		}

		bool IsEmpty() const {
			return s_.empty();
		}
};


// main code harness:
int main(int argc, char** argv) {
	// Now works perfectly with floating point numbers too!
	StackWithMaxAPI<double> el;
	el.Push(1.5);
	el.Push(4.7);
	el.Push(4.7);
	
	cout << "Current Max: " << el.Max() << endl; // Outputs 4.7
	el.Pop();
	cout << "Max after one pop: " << el.Max() << endl; // Still 4.7 (due to frequency tracker)
	el.Pop();
	cout << "Max after two pops: " << el.Max() << endl; // Outputs 1.5
	
	return 0;
}
