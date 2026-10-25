#include <bits/stdc++.h>

using namespace std;

class Solution {
public:

    vector<int> IntersectTwoSortedArrays(
        const vector<int>& arr1,
        const vector<int>& arr2)
    {
        vector<int> result;

        int L1 = 0;
        int L2 = 0;

        while (L1 < arr1.size() &&
               L2 < arr2.size())
        {
            if (arr1[L1] == arr2[L2] &&
                (L1 == 0 || arr1[L1] != arr1[L1-1]))
            {
                result.push_back(arr1[L1]);

                L1++;
                L2++;
            }
            else if (arr1[L1] < arr2[L2])
            {
                L1++;
            }
            else
            {
                L2++;
            }
        }

        return result;
    }
};

int main()
{
    vector<int> arr1 =
    {
        2,3,3,5,5,6,7,7,8,12
    };

    vector<int> arr2 =
    {
        5,5,6,8,8,9,10,10
    };

    Solution sol;

    vector<int> ans =
        sol.IntersectTwoSortedArrays(arr1,arr2);

    cout<<"Intersection:\n";

    for(auto x:ans)
        cout<<x<<" ";

    cout<<endl;

    return 0;
}
