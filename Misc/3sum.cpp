#include <algorithm>
#include <iostream>
#include <vector>
#include <map>


// define solution class:
class Solution {
public:

    // define the func and nb the return type:
    std::vector<std::vector<int>> threeSum(std::vector<int> &nums)
    {
        std::vector<std::vector<int>> result;
        int n = nums.size();
        
        if (n < 3){ return result; }

        sort(nums.begin(), nums.end());

        for (int i = 0; i < n - 2; i++)
        {
            if (nums[i] > 0) break;
            if (i > 0 && nums[i - 1] == nums[i]) continue;

            int j = i + 1;
            int k = n - 1;

            while (j < k)
            {
                int sum = nums[i] + nums[j] + nums[k];

                if (sum < 0) j++;
                else if (sum > 0) k--;
                else {
                    result.push_back({nums[i], nums[j], nums[k]});  // note how the vector sum is pushed

                    while (j < k && nums[j] == nums[j + 1]) j++;
                    j++;

                    while (j < k && nums[k - 1] == nums[k]) k--;
                    k--;
                }
            }
        }
        
        return result;
    }
};


// main code harness:
int main(int argc, char** argv)
{
    std::vector<int> nums = {-1, 0, 1, 2, -1, -4};

    std::cout << "Input: nums = [";
    for (const auto &num : nums)
    {
        std::cout << num << (num == nums.back() ? "" : ", ");
    }
    std::cout << "]\n";

    Solution sol;
    std::vector<std::vector<int>> result = sol.threeSum(nums);

    std::cout << "Output: [";
    for (const auto &vec : result)
    {
        std::cout << "[";
        for (size_t i = 0; i < vec.size(); i++)
        {
            std::cout << vec[i] << (i < vec.size() - 1 ? ", " : "");
        }
        std::cout << "]" << (vec == result.back() ? "" : ", ");
    }
    std::cout << "]\n";

    return 0;
}
