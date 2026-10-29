#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

// Function to calculate the square root of a real number
// Time Complexity: O(log(log(N/epsilon))) - Converges extremely fast
double findSquareRoot(double n, double epsilon = 1e-12) {
    // Base corner cases
    if (n < 0) {
        return NAN; // Return Not-A-Number for negative values
    }
    if (n == 0 || n == 1) {
        return n;
    }

    // Initial guess setup (n / 2.0 works well for numbers >= 1)
    double guess = n / 2.0;
    double next_guess = 0.5 * (guess + n / guess);

    // Iterate until the difference between consecutive guesses is smaller than precision threshold (epsilon)
    while (abs(guess - next_guess) > epsilon) {
        guess = next_guess;
        next_guess = 0.5 * (guess + n / guess);
    }

    return next_guess;
}


// main code harness:
int main(int argc, char** argv) {
    // Setup high precision output formatting for decimal display
    cout << fixed << setprecision(10);

    double test_numbers[] = {2.0, 0.25, 12345.6789, 0.00004};

    cout << "--- Square Root App Execution ---" << endl;
    for (double num : test_numbers) {
        double custom_res = findSquareRoot(num);
        double library_res = sqrt(num); // Standard library reference

        cout << "Number: " << num << endl;
        cout << "  Custom Algo : " << custom_res << endl;
        cout << "  std::sqrt() : " << library_res << endl;
        cout << "  Abs Error   : " << abs(custom_res - library_res) << "\n" << endl;
    }

    return 0;
}


/*

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

// Function to calculate the square root of a real number using Binary Search
// Time Complexity: O(log((high - low) / epsilon))
double findSquareRootBinarySearch(double n, double epsilon = 1e-12) {
    if (n < 0) {
        return NAN; // Not-A-Number for negative values
    }
    if (n == 0 || n == 1) {
        return n;
    }

    double low = 0.0;
    double high = n;

    // CRUCIAL CORNER CASE: If n < 1 (e.g., 0.25), its square root is LARGER than n (sqrt(0.25) = 0.5)
    // Therefore, the upper boundary must be extended to 1.0.
    if (n < 1.0) {
        high = 1.0;
    }

    double mid = 0.0;

    // Loop until our search window is smaller than our desired precision
    while ((high - low) > epsilon) {
        mid = low + (high - low) / 2.0;
        double square = mid * mid;

        if (square > n) {
            high = mid; // Root is in the lower half
        } else {
            low = mid;  // Root is in the upper half
        }
    }

    // Returning low or high here yields practically identical results within the epsilon threshold
    return low;
}


// main code harness:
int main(int argc, char** argv) {
    // Setup high precision output formatting for decimal display
    cout << fixed << setprecision(10);

    double test_numbers[] = {2.0, 0.25, 12345.6789, 0.00004};

    cout << "--- Square Root via Binary Search ---" << endl;
    for (double num : test_numbers) {
        double custom_res = findSquareRootBinarySearch(num);
        double library_res = sqrt(num); // Standard library reference

        cout << "Number: " << num << endl;
        cout << "  Binary Search: " << custom_res << endl;
        cout << "  std::sqrt()  : " << library_res << endl;
        cout << "  Abs Error    : " << abs(custom_res - library_res) << "\n" << endl;
    }

    return 0;
}

























*/