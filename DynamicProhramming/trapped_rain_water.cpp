/**
 * @file
 * @brief Implementation of the [Trapped Rainwater Problem](https://www.geeksforgeeks.org/trapping-rain-water/)
 */

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

namespace dynamic_programming {
/**
 * @brief Calculates trapped rainwater using the Two-Pointer technique
 * @param heights Array representing the heights of walls
 * @return The amount of trapped rainwater
 */
uint32_t trappedRainwater(const std::vector<uint32_t>& heights) {
    std::size_t n = heights.size();
    if (n <= 2) return 0; 

    std::size_t left = 0, right = n - 1;
    uint32_t left_max = 0, right_max = 0, trappedWater = 0;
    
    // two pointer method to calculate trapped rainwater:
    while (left < right) {
        if (heights[left] < heights[right]) {
            heights[left] >= left_max ? left_max = heights[left] : trappedWater += left_max - heights[left];
            ++left;
        } else {
            heights[right] >= right_max ? right_max = heights[right] : trappedWater += right_max - heights[right];
            --right;
        }
    }
    return trappedWater;
}
}  // namespace dynamic_programming

static void test() {
    // Standard test assertions are retained to validate the two-pointer logic.
    assert(dynamic_programming::trappedRainwater({0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}) == 6);
    assert(dynamic_programming::trappedRainwater({3, 0, 2, 0, 4}) == 7);
    assert(dynamic_programming::trappedRainwater({5, 1, 5}) == 4);
    std::cout << "All trapping rainwater tests successfully passed!\n";
}

// main code harness:
int main(int argc, char** argv) {
    test(); 
    return 0;
}
