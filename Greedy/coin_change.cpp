/*
	Given coins of denominations 1, 5, 10, 25, 50, 100. Find the minimum no. of coins for making change 
	for a given amount.

	Input: Array of denominations, amount
	Output: int
	
	Solution:
		Using greedy approach select the highest possible denomination every time.
		TC: O (n)
*/

#include <iostream>
#include<vector>
using namespace std;

// finds the combination of coins for getting change in min number of coins
int minChangeCmb(vector<int>& coins, int& amount){
	int coin_num = 0;
	
	for(auto coin = coins.rbegin(); coin != coins.rend(); coin++){
		if(amount == 0){
			break;
		}
		
		coin_num += amount / *coin;
		amount %= *coin;
	}
	
	return coin_num;
}


// main code ahrness:
int main(int argc, char** argv) {
	vector<int> coins = {1, 5, 10, 25, 50, 100};
	int amount = 40;
	
	cout << minChangeCmb(coins, amount);
	return 0;
}



/*

// C++ program to traverse vector in reverse
// using vector::rbegin() and vector::rend()
#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> v = {9, 11, 15, 56};

    // Printing all elements in reverse order
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        cout << *it << " ";
    return 0;
}

// 56 15 11 9 

*/