#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


/*
Given an integer array arr[], find the subarray (containing at least one element) which 
has the maximum possible sum, and return that sum. A subarray is a continuous part of an array.
*/



// find the max subarray:
int maxSubarraySum(vector<int> &arr) {
    
    // Stores the result (maximum sum found so far)
    int res = arr[0];           
    
    // Maximum sum of subarray ending at current position
    int maxEnding = arr[0];     

    for (int i = 1; i < arr.size(); i++) {
        
        // Either extend the previous subarray or start 
        // new from current element
        maxEnding = max(arr[i], maxEnding + arr[i]);

        // Update result if the new subarray sum is larger
        res = max(res, maxEnding);
    }
    return res;
}



// main code harness:
int main(int argc, char** argv) {
    vector<int> arr = {2, 3, -8, 7, -1, 2, 3};
    cout << maxSubarraySum(arr); 
    return 0;
}




/*

// Brute force approach: knaive approach: O(n^2): By iterating over all subarrays - O(n^2) Time and O(1) Space

To calculate the maximum sum of subarray ending at current element, say maxEnding, we can use the maximum sum ending at the previous element.

So for any element, we have two choices:

Choice 1: Extend the maximum sum subarray ending at the previous element by adding the current element to it. If the maximum subarray sum ending at the previous index is positive, then it is always better to extend the subarray.
Choice 2: Start a new subarray starting from the current element. If the maximum subarray sum ending at the previous index is negative, it is always better to start a new subarray from the current element.
This means that maxEnding at index i = max(maxEnding at index (i - 1) + arr[i], arr[i]) and the maximum value of maxEnding at any index will be our answer.



#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxSubarraySum(vector<int> &arr) {
    int res = arr[0];
  
    // Outer loop for starting point of subarray
  	for(int i = 0; i < arr.size(); i++) {
    	int currSum = 0;
      
        // Inner loop for ending point of subarray
        for(int j = i; j < arr.size(); j++) {
        	currSum = currSum + arr[j];
          
            // Update res if currSum is greater than res
            res = max(res, currSum);
        }
    }
    return res;
}


// main code harness:
int main(int argc, char** argv) {
    vector<int> arr = {2, 3, -8, 7, -1, 2, 3};
    cout << maxSubarraySum(arr);
    return 0;
}



*/

