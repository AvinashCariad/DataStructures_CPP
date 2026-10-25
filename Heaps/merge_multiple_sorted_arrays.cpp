/* 
	There are multiple files containing say numbers in new line. The content of each file is sorted.
	The task is to merge the files into a single file such that the contents are still sorted.
	Another way of putting is given multiple vectors with elements sorted, merge them into a single
	vector.

	Input: 2d vector of int
	Output: 1d sorted vector
	
	Solution:
		Approach 1: Merge sorted vectors. If there are 'k' vectors, then O(logk * n), 
		n: max no. of sequences. But the space requirement will be O(n)

		Approach 2: Using heap:
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
#include <string>
#include <queue>
#include <sstream>
#include <functional>
using namespace std;


// Structure to keep track of elements and their positions
struct Element {
    int value;         // The actual data value
    int arrayIndex;    // Index of the array from which this element came
    int elementIndex;  // Current element index inside that specific array
};

// Custom comparator to turn std::priority_queue into a Min-Heap
struct CompareElement {
    bool operator()(const Element& a, const Element& b) {
        return a.value > b.value; // Lower values get higher priority
    }
};

// Define Class solution:
class Solution {
public:
    std::vector<int> mergeKSortedArrays(const std::vector<std::vector<int>>& arrays) {
        std::vector<int> result;
        
        // Priority Queue acting as a Min-Heap
        std::priority_queue<Element, std::vector<Element>, CompareElement> minHeap;
        
        // Step 1: Push the first element of each non-empty array into the heap
        for (int i = 0; i < arrays.size(); ++i) {
            if (!arrays[i].empty()) {
                minHeap.push({arrays[i][0], i, 0});
            }
        }
        
        // Step 2: Extract the minimum element and push the next element and all elements from the same array
        while (!minHeap.empty()) {
            Element current = minHeap.top();
            minHeap.pop();
            
            // Store the smallest element in the final result
            result.push_back(current.value);
            
            // If the source array has more elements, push the next one into the heap
            int nextElementIndex = current.elementIndex + 1;
            if (nextElementIndex < arrays[current.arrayIndex].size()) {
                minHeap.push({
                    arrays[current.arrayIndex][nextElementIndex],        // value 
                    current.arrayIndex,                                  // current array index
                    nextElementIndex                                     // next element index (current.elementIndex + 1)
                });
            }
        }
        
        return result;
    }
};


// main code harness:
int main(int argc, char** argv) {
    std::vector<std::vector<int>> arrays = {
		{-1, 0, 5, 89, 7},
		{2, 5, 7, 9, 10},
		{-22, 67, 89, 91, 100},
		{2, 8, 9, 10, 15, 77}
		};
    
    Solution sol;
    std::vector<int> merged = sol.mergeKSortedArrays(arrays);
    
    std::cout << "Merged array: ";
    for (int num : merged) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}




/*
struct VectorIterator {
	bool operator>(const VectorIterator& b) const {
		return 	*curr > *(b.curr);
	}
	
	vector<int>::const_iterator curr;	// for current vector element
	vector<int>::const_iterator last;   // for saving vector end
};

// merges multiple sorted vector into a single vector
vector<int> mergeMultipleSortedvectors(vector<vector<int>>& arr) {
	vector<int> sorted_vector;

	// min-heap of size equal to num of vectors, here we use lambda function for comparator
	// we can use greater<> also, since '<' has been overloaded already in structure
	priority_queue<VectorIterator, vector<VectorIterator>, function<bool(VectorIterator, VectorIterator)>>
		min_heap([](const VectorIterator& a, const VectorIterator& b)->bool{
			return *(a.curr) > *(b.curr);
		});
	
	// for each vector, we put its first and last iterators, this helps
	// to keep track of which vector element was popped 
	for(const vector<int>& a: arr) {
		if(!a.empty()) {
			min_heap.emplace(VectorIterator{a.cbegin(), a.cend()});
		};	
	}
	
	// now we keep popping the smallest element from heap untill all the 
	// vector elements are finished
	while(!min_heap.empty()) {
		auto min_el = min_heap.top();
		
		// pop the heap min
		min_heap.pop();

		// now push the next element from the vector from
		// which the element was popped
		if(min_el.curr != min_el.last) {
			sorted_vector.emplace_back(*(min_el.curr));
			min_heap.emplace( VectorIterator{ next(min_el.curr), min_el.last });
		}
	}
	
	return sorted_vector;
}

// overloading << for directly viewing the vector elements
template<typename T>
ostream&  operator<<(ostream& out, vector<T> arr) {
	for(const auto& a: arr)
		cout << a << " ";
	cout << endl;
}


// main code harness:
int main(int argc, char** argv) {
	vector<vector<int>> arr = {
								{-1, 0, 5, 89, 7},
								{2, 5, 7, 9, 10},
								{-22, 67, 89, 91, 100},
								{2, 8, 9, 10, 15, 77}
							};
							

	cout << mergeMultipleSortedvectors(arr);
	return 0;
}


*/
