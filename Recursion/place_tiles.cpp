#include <iostream>
using namespace std;

/*
When you place a \(1 \times m\) tile on an \(n \times m\) floor:
Placing a tile horizontally reduces the remaining height 
(\(n\)) by \(m\) (leaving an \((n-m) \times m\) space).
Placing a tile vertically reduces the remaining height (\(n\)) by \(1\) (leaving an \((n-1) \times m\) space).

*/


int placeTiles(int n, int m)
{
    // If the remaining floor height is less than the tile length,
    // the only option is to place all remaining tiles vertically (1 way).
    if (n < m)
    {
        return 1;
    }
    // If the remaining height exactly matches the tile length,
    // you can either place them all vertically OR all horizontally (2 ways).
    else if (n == m)
    {
        return 2;
    }

    // Fixed comment: Placing a tile vertically reduces the height by 1
    int verticalPlacements = placeTiles(n - 1, m);

    // Fixed comment: Placing a tile horizontally reduces the height by m
    int horizontalPlacements = placeTiles(n - m, m);

    return verticalPlacements + horizontalPlacements;
}

int main()
{
    int n = 4, m = 2;
    cout << "Total ways to place tiles: " << placeTiles(n, m) << endl; // Output: 5

    return 0;
}
