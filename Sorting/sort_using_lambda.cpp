#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Required for std::sort and std::for_each


/*
// Source - https://stackoverflow.com/a/5123582
// Ascending:

std::ranges::sort(mMyClassVector, [](const MyClass &a, const MyClass &b)
{ 
    return a.mProperty < b.mProperty; 
});




// Source - https://stackoverflow.com/a/5123582
// Descending:

std::ranges::sort(mMyClassVector, [](const MyClass &a, const MyClass &b)
{ 
    return a.mProperty > b.mProperty; 
});



*/


/*
We want to sort a list of employees based on three rules:Department
alphabetically (primary sort).Performance Rating from highest to lowest
if they are in the same department (secondary sort).Salary from lowest
to highest if both department and rating are identical (tertiary tie-breaker).
We will also use a local variable to toggle the department sorting order dynamically.

*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

class Employee {
public:
    int id;
    std::string name;
    std::string department;
    int performanceRating; // Scale of 1 to 5 (5 being best)
    double salary;

    Employee(int id, std::string name, std::string dept, int rating, double sal)
        : id(id), name(name), department(dept), performanceRating(rating), salary(sal) {}
};


// main code harness:
int main() {
    std::vector<Employee> staff = {
        Employee(101, "Alice",  "Engineering", 5, 120000.0),
        Employee(102, "Bob",    "Engineering", 4, 95000.0),
        Employee(103, "Charlie", "Marketing",   5, 85000.0),
        Employee(104, "David",   "Engineering", 5, 110000.0), // Same dept & rating as Alice, lower salary
        Employee(105, "Eva",     "Marketing",   3, 90000.0),
        Employee(106, "Frank",   "Sales",       4, 75000.0)
    };

    // A runtime configuration variable that we will pass into the lambda
    bool sortDeptAscending = true;

    // Advanced sorting using a multi-key lambda with variable capture
    std::sort(staff.begin(), staff.end(), [sortDeptAscending](const Employee& a, const Employee& b) {
        // Rule 1: Compare Departments
        // If their departments are different:
        // It immediately resolves the order. For example,
        // "Engineering" comes before "Marketing", so any
        // Engineering employee is immediately placed ahead
        // of a Marketing employee.If their departments are 
        // identical: The code skips this if block entirely and moves to Step 2 to break the tie.
        if (a.department != b.department) {
            if (sortDeptAscending) {
                return a.department < b.department; // A-Z
            } else {
                return a.department > b.department; // Z-A
            }
        }

        // Rule 2: Secondary tie-breaker - Performance Rating (Descending order: 5 down to 1)
        // If two employees work in the exact same department, the algorithm looks at their performanceRating.
        if (a.performanceRating != b.performanceRating) {
            return a.performanceRating > b.performanceRating;
        }

        // Rule 3: Tertiary tie-breaker - Salary (Ascending order)
        // If both the department AND the performance rating match perfectly, 
        // the algorithm relies on the final return statement.
        return a.salary < b.salary;
    });

    // Print the beautifully sorted results
    std::cout << std::left << std::setw(12) << "Dept" 
              << std::setw(10) << "Name" 
              << std::setw(8) << "Rating" 
              << "Salary\n";
    std::cout << std::string(40, '-') << "\n";

    for (const auto& emp : staff) {
        std::cout << std::left << std::setw(12) << emp.department
                  << std::setw(10) << emp.name
                  << std::setw(8) << emp.performanceRating
                  << "$" << std::fixed << std::setprecision(2) << emp.salary << "\n";
    }

    return 0;
}




/*
// define student struct:
struct Student {
    std::string name;
    int grade;
};


// main code harness:
int main(int argc, char** argv) {
    std::vector<Student> students = {
        {"Alice", 85},
        {"Bob", 92},
        {"Charlie", 78}
    };

    // 1. Sort students by grade in descending order
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b)->int {
        return a.grade > b.grade;
    });

    // 2. Print out the results using a lambda function
    std::cout << "--- Student Rankings ---\n";
    // Iterate:
    std::for_each(students.begin(), students.end(), [](const Student& s) {
        std::cout << s.name << ": " << s.grade << "\n";
    });

    return 0;
}


*/
