#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution
{
public:

    bool containsDuplicate(vector<int>& nums)
    {
        unordered_map<int,int> freq;

        for(int num : nums)
        {
            if(freq.find(num) == freq.end())
            {
                freq[num] = 1;
            }
            else
            {
                return true;
            }
        }

        return false;
    }
};


// main code harness:
int main(int argc, char** argv)
{
    vector<int> nums =
    {
        1,2,3,4,5,6,2
    };

    Solution sol;

    if(sol.containsDuplicate(nums))
        cout<<"Contains Duplicate"<<endl;
    else
        cout<<"No Duplicate"<<endl;

    return 0;
}
