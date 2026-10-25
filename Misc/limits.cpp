#include <iostream>
#include <vector>
#include <limits>

// main code harness:
int main(int argc, char** argv)
{
    std::vector<double> nums = {5.6, 8.1, 2.3, 10.4, -1.2, 7.8};

    double minimum = std::numeric_limits<double>::max();

    for (double x : nums)
    {
        if (x < minimum)
            minimum = x;
    }

    std::cout << "Minimum = " << minimum << std::endl;

    return 0;
}
