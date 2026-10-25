/*	
	Given 'n', find the nth fibonacci number.

	Input: int
	Output: int

	Solution:
		For each term we check if the previous term has been found or not if not, then
		find it and cache it. We use the cache values later on on so that each computation is done
		only once.

		TC: O(n)
		SC: O(n)
*/

#include <iostream>
#include <unordered_map>
using namespace std;

// finds the nth fibonacci number
int nthFibonacciNumber(int n) {
	static unordered_map<int, int> cache;    // note how the cache is being defined
	
	// base cases
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;

	// if the cache has the value, return it - this is the step 
    // for memoization, find if it has been already computed and 
    // if so, just use the cache
	if (cache.find(n) != cache.end()) {
		return cache[n];
	}

	cache[n] = nthFibonacciNumber(n - 1) + nthFibonacciNumber(n-2);
	return cache[n];
}


// main code harness:
int main(int argc, char** argv) {

	int n = 5;
	for(int n = 0; n < 6; n++)
		cout << nthFibonacciNumber(n) << endl;
	return 0;
}
