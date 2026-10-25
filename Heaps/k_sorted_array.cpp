/* 
	Given an array where the elements are k sorted. It means that each element is at most k places
	away from its sorted position. Sort such an array.

	Input: vector of int
	Output: 1d sorted vector
	
	Solution:
		Since each element is at most k places away, that means the first element is also atmost k places
		away, so we atleast need to get the first k elements. So we use a min-heap of size 'k'. We push the first
		'k' elements in it, when we are at k+1th element we can just pop the min element and push to the 0th index.
		Similarly we do the same for the rest. When the heap is empty we are done.

		TC: O(nlogk), k: heap size, n: no. of elements
		SC: O(k) for heap
*/



#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // for std::min

using namespace std;

// sorts a k-sorted array
vector<int>& kSortedData(vector<int>& arr, int k) { // Changed return to a reference to save copying
	if (arr.empty()) return arr;

	// Use a min-heap
	priority_queue<int, vector<int>, greater<int>> min_heap;

	// Insert the first k + 1 elements (or less if the array is tiny)
	int n = arr.size();
	int initial_elements = min(k + 1, n);
	
	for (int i = 0; i < initial_elements; i++) {
		min_heap.emplace(arr[i]);
	}
	
	int write_idx = 0;
	int i = initial_elements;
	
	// Process the rest of the array
	while (!min_heap.empty()) {
		arr[write_idx++] = min_heap.top();
		min_heap.pop();

		// Push the next element into the window if it exists
		if (i < n) {
			min_heap.emplace(arr[i++]);
		}
	}

	return arr;
}

// Fixed stream overloading
template<typename T>
ostream& operator<<(ostream& out, const vector<T>& arr) { // Added const&
	for (const auto& a : arr)
		out << a << " "; // Changed cout to out
	out << "\n";
	return out;          // Added missing return statement
}

int main() {
	vector<int> arr = {3, -1, 2, 6, 24, 15, 28};
	int k = 2;
						
	kSortedData(arr, k);
	cout << arr; // Correctly outputs: -1 2 3 6 15 24 28 
	return 0;
}





/*
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <functional>
using namespace std;

// sorts k sorted array
vector<int> kSortedData(vector<int>& arr, int k) {
	// make a min-heap
	priority_queue<int, vector<int>, greater<>> min_heap;

	// insert first k elements
	for(int i = 0; i < k; i++) 
		min_heap.emplace(arr[i]);
	
	int write_idx = 0;
	int i = k;
	// now do for the rest of the elements
	while(!min_heap.empty()) {
		arr[write_idx++] = min_heap.top();
		min_heap.pop();

		// push the current element
		if(i < arr.size())
			min_heap.emplace(arr[i++]);
	}

	return arr;
}

// overloading << for directly viewing the vector elements
template<typename T>
ostream&  operator<<(ostream& out, vector<T> arr) {
	for(const auto& a: arr)
		cout << a << " ";
	cout << endl;
}

int main() {
	vector<int> arr = {3, -1, 2, 6, 24, 15, 28};
							
	cout << kSortedData(arr, 2);
	return 0;
}


*/