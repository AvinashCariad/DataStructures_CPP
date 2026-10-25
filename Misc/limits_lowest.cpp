#include <iostream>
#include <vector>
#include <limits>


// main code harness:
int main(int argc, char** argv)
{
    std::vector<double> nums = {5.6, 8.1, 2.3, 10.4, -1.2};

    double maximum = std::numeric_limits<double>::lowest();      // note the use of lowest

    for (double x : nums)
    {
        if (x > maximum)
            maximum = x;
    }

    std::cout << maximum << std::endl;
}
