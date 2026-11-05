#include <iostream>
#include <vector>
#include <algorithm>

// Custom error handling using a clean C++ approach
void handle_error()
{
    std::cout << "Test failed \n";
    std::exit(1);
}

// Representing Box using a clean modern structure
struct Box
{
    int height;
    int length;
    int breadth;
};

/*
a: vector of boxes of different dimensions. Should contain at least one box
Result: maximum height of the stack of boxes that can be constructed.
Assumption is that multiple instances of each box are available
*/
int max_stack_height(const std::vector<Box>& a) {
    if (a.empty()) return 0;

    // Use std::vector to safely manage memory instead of calloc/free
    std::vector<Box> boxes;
    boxes.reserve(3 * a.size());

    /* For each box, all 3 orientations are possible. Length will always be
       greater than breadth */
    for (const auto& box : a) {
        boxes.push_back({
            box.height, 
            std::max(box.length, box.breadth), 
            std::min(box.length, box.breadth)
        });
        
        boxes.push_back({
            box.length, 
            std::max(box.breadth, box.height), 
            std::min(box.breadth, box.height)
        });

        boxes.push_back({
            box.breadth, 
            std::max(box.length, box.height), 
            std::min(box.length, box.height)
        });
    }

    /* Sort the boxes using std::sort with a modern lambda expression.
       Higher area boxes should be stored first after sorting */
    std::sort(boxes.begin(), boxes.end(), [](const Box& box1, const Box& box2) {
        return (box2.length * box2.breadth) < (box1.length * box1.breadth);
    });

    // Populate tracking vector for standard dynamic programming array initialization
    std::vector<int> best_height(boxes.size());
    for (size_t i = 0; i < boxes.size(); ++i) {
        best_height[i] = boxes[i].height;
    }

    // Iterate through all boxes and find the best height for each box
    for (size_t i = 1; i < boxes.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            /* We can place box i on box j, only if base of box i 
               is smaller than the base of box j */
            if (boxes[i].length < boxes[j].length && boxes[i].breadth < boxes[j].breadth) {
                if (best_height[i] < best_height[j] + boxes[i].height) {
                    best_height[i] = best_height[j] + boxes[i].height;
                }
            }
        }
    }

    /* Find the stack with the maximum height using standard library utilities - 
    //note the derefrencing */
    return *std::max_element(best_height.begin(), best_height.end());     // Return the maximum height of the stack
}


void test_01()
{
    std::vector<Box> a = { {100, 120, 320}, {10, 20, 30}, {40, 60, 70}, {40, 50, 60} };
    
    int result = max_stack_height(a);
    std::cout << "Max stack height = " << result << " \n";

    if (result != 600)
        handle_error();
}


void test_02()
{
    std::vector<Box> a = { {10, 20, 30} };
    
    int result = max_stack_height(a);
    std::cout << "Max stack height = " << result << " \n";
}


// main code harness:
int main(int argc, char *argv[])
{
    test_01();
    test_02();

    std::cout << "Test passed \n";
    return 0;
}
