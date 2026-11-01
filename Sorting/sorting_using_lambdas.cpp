#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>


using namespace std;


struct Employee {
    int id;
    std::string name;
    double salary;
};


// main code harness:
int main(int argc, char** argv) {
    std::vector<Employee> employees = {
        {101, "Alice", 75000.0},
        {102, "Bob", 90000.0},
        {103, "Charlie", 60000.0}
    };

    // Sort by salary in descending order
    std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b) {
        return a.salary > b.salary; 
    });

    for (const auto& emp : employees) {
        std::cout << emp.name << ": $" << emp.salary << "\n";
    }


    // Multi-Criteria Sorting (Tie-Breaking)
    // You can chain conditions inside a lambda. For example, 
    // sorting students by their grades in descending order, and
    // if their grades are identical, breaking the tie by their names
    // alphabetically.

    struct Student {
    std::string name;
    int grade;
    };
    std::vector<Student> students = {
        {"Zack", 85},
        {"Alice", 95},
        {"Charlie", 85},
        {"Bob", 95}
    };

    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        if (a.grade != b.grade) {
            return a.grade > b.grade; // Primary: Higher grade first
        }
        return a.name < b.name;       // Secondary: Alphabetical order
    });

    for (const auto& s : students) {
        std::cout << s.name << " (" << s.grade << ")\n";
    }

    // weak_ptr: 
    // A std::weak_ptr holds a non-owning reference to a resource managed by
    // a std::shared_ptr.
    // • Properties: It does not increment the reference count. It is primarily
    // used to break cyclic dependencies (where Object A points to Object B, and Object B points to Object A, preventing both from ever being deleted).
    // • Accessing Data: Because it doesn't own the object, it cannot access the
    // data directly. It must be temporarily converted back into a std::shared_ptr using the .lock() method to ensure the object hasn't already been deleted.

    struct Point {
        int x, y;
    };

    std::vector<Point> points = {{1, 1}, {5, 5}, {2, 2}, {0, 3}};
    
    // Target anchor point
    Point reference = {0, 0}; 

    // Capture 'reference' by value to calculate distance
    sort(points.begin(), points.end(), [reference](const Point& a, const Point& b) {
        double distA = std::hypot(a.x - reference.x, a.y - reference.y);
        double distB = std::hypot(b.x - reference.x, b.y - reference.y);
        return distA < distB; // Closest to reference point comes first
    });

    for (const auto& p : points) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }
}


