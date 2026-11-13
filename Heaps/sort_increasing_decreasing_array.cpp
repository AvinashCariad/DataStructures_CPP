/*
	There is an array which has elements which increase in order and then start decreasing. This can happen multiple
	time. Sort such an array.

	Input: vector of int
	Output: 1d sorted vector

	Solution:
		First we segregate the vectors into individual sorted arrays and then we
		use the algorithm for merging multiple sorted vectors. We use a 2d vector for storing the sorted
		vectors.
		Increasing vector can be added directly. For decreasing vector we first need to reverse the elements then add.

		For merging sorted vectors:
			For 'k' no. of vectors, we make a min-heap of 'k' size. Initially
			it is filled with first entry from each vector. Every time we pop the smallest
			from the heap and push the next element of the vector from which the min was popped.
			Since the files are having sorted content so we are only pushing the elements
			which are smaller and then finding the smallest amongst them each time.

		TC: O(nlogk), k: no. of vectors, n: total sequences
		SC: O(k)
*/

#include <iostream>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

struct VectorIterator {
    vector<int>::const_iterator curr;
    vector<int>::const_iterator last;

    bool operator>(const VectorIterator& b) const {
        return *curr > *(b.curr);
    }
};

vector<int> mergeMultipleSortedvectors(const vector<vector<int>>& arr) {
    vector<int> sorted_vector;

    // Using greater<VectorIterator> avoids std::function overhead
    priority_queue<VectorIterator, vector<VectorIterator>, greater<VectorIterator>> min_heap;

    // 1. Only push non-empty vectors so curr is always valid
    for (const vector<int>& a : arr) {
        if (a.cbegin() != a.cend()) {
            min_heap.emplace(VectorIterator{ a.cbegin(), a.cend() });
        }
    }

    // 2. Every item in min_heap is guaranteed to have curr != last
    while (!min_heap.empty()) {
        auto min_el = min_heap.top();
        min_heap.pop();                                                   // popping removes the first and last iterator both

        sorted_vector.emplace_back(*(min_el.curr));

        auto next_it = next(min_el.curr);
        // Only push back into the heap if more elements remain
		if (next_it != min_el.last) {
            min_heap.emplace(VectorIterator{ next_it, min_el.last});       // Because std::priority_queue does not let you modify an element in place. When you pop the smallest item, you remove that vector's "bookmark" from the heap, so you have to insert a new bookmark pointing to the next element in that same vecto
        }
    }

    return sorted_vector;
}



// sorts increasing decreasing vector elements
vector<int> sortIncreasingDecreasingElements(vector<int>& arr) {
	// for storing multiple sorted arrays from single array
	vector<vector<int>> multiple_sorted_arrays;

	vector<int> ::iterator start = arr.begin(), last = arr.begin();

	bool increasing = true;
	for (int i = 1; i <= arr.size(); i++, last++) {
		// add the series when a juction point is reached or the it is the last element
		if ((arr[i - 1] >= arr[i] && increasing == true) ||	// increasing 
			(arr[i - 1] < arr[i] && increasing == false) ||	// decreasing
			i == arr.size()) {

			if (increasing == true) {
				multiple_sorted_arrays.emplace_back(vector<int>(start, last));
			}
			else if (increasing == false) {
				//reverse(start, last);
				multiple_sorted_arrays.emplace_back(vector<int>(start, last));
			}
			
			start = last;
			// change the series type
			increasing = !increasing;
		}
	}

	return mergeMultipleSortedvectors(multiple_sorted_arrays);
}

// overloading << for directly viewing the vector elements
template<typename T>
ostream& operator<<(ostream& out, vector<T> arr) {
	for (const auto& a : arr)
		out << a << " ";
	out << endl;

	return out;
}


// main code harness:
int main(int argc, char** argv) {
	vector<int> arr = { -1, 0, 5, 89, 7, 2, 5, 7, 9, 10, -22, 67, 89, 91, 100, 2, 8, 9, 10, 15, 77 };

	auto sorted = sortIncreasingDecreasingElements(arr);
	cout << sorted;
	return 0;
}



/*

// Another alternative approach:
// To merge K sorted arrays in C++, use a min-heap (std::priority_queue with std::greater)
// to repeatedly extract the smallest available element across all arrays in \(O(N \log K)\) time. Here, N is the total number of elements across all arrays and K is the number of arrays.



#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Struct to track the value and its position in the 2D array
struct Element {
    int value;
    int arrayIndex;
    int elementIndex;

    // Overload '>' so std::priority_queue works as a min-heap
    bool operator>(const Element& other) const {
        return value > other.value;
    }
};

vector<int> mergeSortedArrays(const vector<vector<int>>& arrays) {
    priority_queue<Element, vector<Element>, greater<Element>> minHeap;
    vector<int> result;

    // 1. Push the first element of each non-empty array into the min-heap
    for (int i = 0; i < (int)arrays.size(); ++i) {
        if (!arrays[i].empty()) {
            minHeap.push({arrays[i][0], i, 0});
        }
    }

    // 2. Pop the smallest element and push the next element from the same array
    while (!minHeap.empty()) {
        Element current = minHeap.top();
        minHeap.pop();

        result.push_back(current.value);

        int nextIdx = current.elementIndex + 1;
        if (nextIdx < (int)arrays[current.arrayIndex].size()) {
            minHeap.push({arrays[current.arrayIndex][nextIdx], current.arrayIndex, nextIdx});
        }
    }

    return result;
}


// main code harness:
int main(int argc, char** argv) {
    vector<vector<int>> arrays = {
        {1, 5, 9, 21},
        {2, 6, 12},
        {0, 3, 7, 15, 30},
        {} // Handles empty arrays safely
    };

    vector<int> merged = mergeSortedArrays(arrays);

    cout << "Merged Array: ";
    for (int num : merged) {
        cout << num << " ";
    }
    cout << "\n";

    return 0;
}












*/