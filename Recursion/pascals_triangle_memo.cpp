#include <iostream>
#include <vector>

using namespace std;

class PascalMemo {
private:
    vector<vector<long long>> memo;

public:
    PascalMemo(int maxRows) {
        // Initialize DP table with -1 to represent uncomputed states
        memo.assign(maxRows, vector<long long>(maxRows, -1));        // note how the initialization is being done
    }

    // Returns the value at (row, col) using top-down recursion + memoization
    long long getElement(int row, int col) {
        // Base cases: first/last column or top of the triangle
        if (col == 0 || col == row) {
            return 1;
        }
        
        // Return cached result if already computed
        if (memo[row][col] != -1) {
            return memo[row][col];
        }

        // Recursive relation: C(r, c) = C(r-1, c-1) + C(r-1, c)
        memo[row][col] = getElement(row - 1, col - 1) + getElement(row - 1, col);
        return memo[row][col];
    }
};


// main code harness:
int main(int argc, char** argv) {
    PascalMemo pt(35);
    cout << "Row 5, Col 2: " << pt.getElement(5, 2) << endl; // Output: 10
    cout << "Row 6, Col 3: " << pt.getElement(6, 3) << endl; // Output: 20
    return 0;
}
