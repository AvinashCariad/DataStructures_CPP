#include <iostream>
using namespace std;


// recursive code to check if it is increasing:
bool checkIfIncreasing(int arr[], int idx, int len)
{
    // Base Case: If we've reached the last element, the array is successfully sorted
    if (idx == len - 1)
    {
        return true;
    }

    // Optimization: Check the current pair FIRST. 
    // If it's not strictly increasing, fail immediately and stop recursing!
    if (arr[idx] >= arr[idx + 1])
    {
        return false;
    }

    // Move to the next pair
    return checkIfIncreasing(arr, idx + 1, len);
}


// main code harness:
int main(int argc, char** argv)
{
    int arr1[] = {1, 6, 3, 4, 5};
    int len = sizeof(arr1) / sizeof(arr1[0]);

    if (checkIfIncreasing(arr1, 0, len))
    {
        cout << "Strictly Increasing\n";
    }
    else
    {
        cout << "Not Strictly Increasing\n"; // Output: Not Strictly Increasing
    }

    return 0;
}
