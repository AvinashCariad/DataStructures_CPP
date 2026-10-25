#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

void handleError() {
    std::cout << "Error occurred\n";
    exit(1);
}

// 3-Way Partitioning (Dutch National Flag Sort)
// TC: O(N), SC: O(1)
void dutchSort(std::vector<int>& a, int pivot_value) {
    if (a.empty()) return;

    int cur_pos = 0;
    int left_pos = 0;
    int right_pos = static_cast<int>(a.size()) - 1;

    // main logic block: note just one while loop:
    while (cur_pos <= right_pos) {
        if (a[cur_pos] < pivot_value) {
            std::swap(a[left_pos], a[cur_pos]);
            ++left_pos;
            ++cur_pos;
        } else if (a[cur_pos] > pivot_value) {
            std::swap(a[cur_pos], a[right_pos]);
            --right_pos; // Don't advance cur_pos yet, must evaluate the newly swapped element
        } else {
            ++cur_pos;
        }
    }
}

// print array:
void printArray(const std::vector<int>& a) {
    for (int val : a) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}


// test:
void test(std::vector<int>& a, int pivot_value) {
    std::cout << "Pivot value = " << pivot_value << "\n";
    std::cout << "Before : ";
    printArray(a);

    /* Perform the sort */
    dutchSort(a, pivot_value);

    /* Verify the result of dutch sort */
    size_t i = 0;
    
    // 1. Elements less than pivot must occur first
    while (i < a.size() && a[i] < pivot_value)
        ++i;

    // 2. Elements matching the pivot come next
    while (i < a.size() && a[i] == pivot_value)
        ++i;

    // 3. Elements greater than the pivot finish the array
    while (i < a.size() && a[i] > pivot_value)
        ++i;

    if (i != a.size())
        handleError();

    std::cout << "After  : ";
    printArray(a); 
    std::cout << "_______________________________________________\n";
} 



// main code harness:
int main(int argc, char** argv) {
    const int MAX_NUM_ELEMENTS = 10;
    const int MAX_VALUE = 10;
    const int MAX_NUM_ITERATIONS = 10;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> length_dist(1, MAX_NUM_ELEMENTS);
    std::uniform_int_distribution<int> value_dist(0, MAX_VALUE - 1);

    for (int iter = 0; iter < MAX_NUM_ITERATIONS; ++iter) {
        int length = length_dist(gen);
        std::vector<int> a(length);

        for (int i = 0; i < length; ++i) {
            a[i] = value_dist(gen);
        }
        
        // Pick a random pivot value from within the newly generated values
        std::uniform_int_distribution<int> pivot_idx_dist(0, length - 1);
        int pivot_value = a[pivot_idx_dist(gen)];
        
        test(a, pivot_value);
    }

    std::cout << "Test passed\n";
    return 0;
}
