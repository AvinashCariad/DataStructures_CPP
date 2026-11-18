#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <string>



using namespace std;

//#include "../Chapter 04/Student_info.h" // using the simplifed version




#ifndef GUARD_median_h
#define GUARD_median_h
double median(std::vector<double>);
#endif



// code is taken from §4.1.1/53
// compute the median of a vector<double>
// note that calling this function copies the entire argument vector
double median(vector<double> vec)
{
    typedef vector<double>::size_type vec_sz;

    vec_sz size = vec.size();
    if (size == 0)
        throw domain_error("median of an empty vector");

    sort(vec.begin(), vec.end());

    vec_sz mid = size/2;

    return size % 2 == 0 ? (vec[mid] + vec[mid-1]) / 2 : vec[mid];
}



#ifndef GUARD_grade_h
#define GUARD_grade_h

#include <vector>

double grade(double, double, double);
double grade(double, double, const std::vector<double>&);
#endif

#include <stdexcept>
#include <vector>
//#include "grade.h"
//#include "median.h"

using std::domain_error;    using std::vector;

// code taken from §4.1/52
// compute a student's overall grade from midterm and final exam grades and homework grade
double grade(double midterm, double final, double homework)
{
    return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}

// code taken from §4.1.2/54
// compute a student's overall grade from midterm and final exam grades
// and vector of homework grades.
// this function does not copy its argument, because median does so for us.
double grade(double midterm, double final, const vector<double>& hw)
{
    if (hw.size() == 0)
        throw domain_error("student has done no homework");
    return grade(midterm, final, median(hw));
}



#ifndef GUARD_Student_info
#define Guard_Student_info

struct Student_info {
    std::string name;
    double final_grade;
};

bool compare(const Student_info&, const Student_info&);
std::istream& read(std::istream&, Student_info&);
std::istream& read_hw(std::istream&, std::vector<double>&);
#endif


// code is taken from §4.4/69
bool compare(const Student_info& x, const Student_info& y)
{
    return x.name < y.name;
}

istream& read(istream& is, Student_info& s)
{
    double midterm, final;
    vector<double> homework;
    // read and store the student's name and final grade
    is >> s.name >> midterm >> final;

    // handle end-of-file
    if (is) {
        read_hw(is, homework);
        // save only the final grade
        s.final_grade = grade(midterm, final, homework);
    }
    
    return is;
}

// code is taken from §4.1.3/57
istream& read_hw(istream& in, vector<double>& hw)
{
    if (in) {
        // get rid of previous contetns
        hw.clear();

        // read homework grades
        double x;
        while (in >> x)
            hw.push_back(x);

        // clear the stream so that input will work for the next student
        in.clear();
    }
    return in;
}



// main code harness:
int main(int argc, char** argv)
{
    vector<Student_info> students;
    Student_info record;
    string::size_type maxlen = 0;

    // read and store all the records, and find the length of the longest name
    while (read(cin, record)) {
        maxlen = max(maxlen, record.name.size());
        students.push_back(record);
    }

    // alphabetize the records
    sort(students.begin(), students.end(), compare);

    // assign letter grades
    map<string, vector<Student_info> > grades;
    for (vector<Student_info>::const_iterator it = students.begin();
        it != students.end(); ++it) {
        if (it->final_grade > 90)
            grades["A"].push_back(*it);
        else if (it->final_grade > 80)
            grades["B"].push_back(*it);
        else if (it->final_grade > 70)
            grades["C"].push_back(*it);
        else if (it->final_grade > 60)
            grades["D"].push_back(*it);
        else
            grades["F"].push_back(*it);
    }

    for (map<string, vector<Student_info> >::const_iterator it = grades.begin();
        it != grades.end(); ++it) {
        // write the grade
        cout << "The student(s) who got " << (it->first == "A" || it->first == "F"
        ? "an " : "a ") << it->first << " are: ";
        
        // followed by one or more student names
        vector<Student_info>::const_iterator student_it = it->second.begin();
        cout << student_it->name; // write the first student name

        ++student_it;
        // write the rest of the student names, if any
        while (student_it != it->second.end()) {
            cout << ", " << student_it->name;
            ++student_it;
        }
        // write a new line to separate each student name from the next
        cout << endl;
    }
    
    return 0;
}