/*
    Given an array of int, where each entry represent value of a coin. There are two players
    and they are playing a game, where in each turn they select a coin either from front or
    back of the array, the player who ends up the largest sum of coin values once 
    all the coins have been picked wins. Find the largest value coins that can be
    collected if a player starts first, assume that the second player is trying his best at each turn also. 
    A player cannot skip his turn.

    Solution:
        The thing to note here is that each player will try to maximize his total score.
        So, for player two will maximize his score or try to minimize player 1 score.

        TC: O(n^2), n: no. of coins 
        SC: O(n^2)

        This is the classic Optimal Strategy for a Game problem, solved using recursion + memoization.
        The important idea is:
        Player 1 wants to maximize their collected value, while Player 2 also plays optimally and therefore tries to leave Player 1 with the worst possible remaining situation.
*/

#include <iostream>
#include <vector>
using namespace std;


// Helper routine:
int maximizeCoinValueHelper(vector<int>& coins, int a, int b, vector<vector<int>>& max_value) {
    // if there are no coins left
    if(a > b)
        return 0;

    // every time player one can pick a coin either from front or back
    if(max_value[a][b] == 0) {
        // when coin at 'a' is selected, player 2 will try to select a coin
        // which is either a+1 or b-1 such that player 1 gets min value 
        // player 1 selscts the front coin, and player 2 is left with other remaining options:
        int front_coin_selected = coins[a] + min(maximizeCoinValueHelper(coins, a + 1, b - 1, max_value),   // player 2 selects back coin
                                                maximizeCoinValueHelper(coins, a + 2, b, max_value));   // player 2 selects front coin

        // player 1 selects the back coin, and player 2 is left with other remaining options:                                         
        int back_coin_selected = coins[b] + min(maximizeCoinValueHelper(coins, a + 1, b - 1, max_value), // player 2 selects front coin
                                                 maximizeCoinValueHelper(coins, a, b - 2, max_value)); // player 2 selects back coin
        // player 1 will select the max of above
        max_value[a][b] = max(front_coin_selected, back_coin_selected);                
    }
    return max_value[a][b];
}

// finds the maximum value thatcan be gained if the palyer starts first
int maximizeCoinValue(vector<int>& coins) {
    // (a, b) stores the max value for the current range of coins
    vector<vector<int>> max_value(coins.size(), vector<int>(coins.size(), 0));
    return maximizeCoinValueHelper(coins, 0, coins.size() - 1, max_value);
}   


// main code harness:
int main(int argc, char** argv) {
    vector<int> coins = {10,25,5,1,10,5};

    cout << maximizeCoinValue(coins);
    cout << endl;
    return 0;
}
