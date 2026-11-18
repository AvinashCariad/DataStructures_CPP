#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using std::cout;
using std::vector;
using std::istream;
using std::domain_error;
using std::sort;
using std::domain_error;


using namespace std;




#ifndef GUARD_Student_info
#define GUARD_Student_info


struct Student_info {
    std::string name;
    double midterm, final;
    std::vector<double> homework;
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

// code is taken from §4.2.2/62
istream& read(istream& is, Student_info& s)
{
    // read and store the student's name and final grade
    is >> s.name >> s.midterm >> s.final;

    read_hw(is, s.homework); // read and store all the student's homework grades
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
//#include "Student_info.h"

double grade(double, double, double);
double grade(double, double, const std::vector<double>&);
double grade(const Student_info&);
#endif


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

// code taken from §4.2.2/63
double grade(const Student_info& s)
{
    return grade(s.midterm, s.final, s.homework);
}


#ifndef GUARD_analysis_h
#define GUARD_analysis_h

//#include "Student_info.h"

double analysis(const std::vector<Student_info>&,
    double (const Student_info&));
double optimistic_median(const Student_info&);
double optimistic_median_analysis(const std::vector<Student_info>&);
double grade_aux(const Student_info&);
double median_analysis(const std::vector<Student_info>&);
double average_grade(const Student_info&);
double average_analysis(const std::vector<Student_info>&);
void write_analysis(std::ostream&, const std::string&,
                    double analysis(const std::vector<Student_info>&),
                    const std::vector<Student_info>&,
                    const std::vector<Student_info>&);
void write_analysis(std::ostream&, const std::string&,
                    double (const Student_info&),
                    const std::vector<Student_info>&,
                    const std::vector<Student_info>&);
bool did_all_hw(const Student_info&);
#endif


double analysis(const vector<Student_info>& students,
    double metric(const Student_info&))
{
    vector<double> grades;
    transform(students.begin(), students.end(),
              back_inserter(grades), metric);
    return median(grades);
}

// code is taken from §6.2.4/116
// median of the nonzero elements of s.homework, or 0 if no such elements exist
// calculates a student's final grade by taking the median of only their submitted (non-zero)
// homework assignments, ignoring any missed assignments scored as 0.
double optimistic_median(const Student_info& s)
{
    vector<double> nonzero;
    // remove_copy(s.homework.begin(), s.homework.end(), back_inserter(nonzero), 0);
    // iterates through the student's homework vector and copies every element except
    // those equal to 0 into nonzero. (Note: remove_copy does not modify the original s.homework vector.)
    remove_copy(s.homework.begin(), s.homework.end(),
        back_inserter(nonzero), 0);

    if (nonzero.empty())
        return grade(s.midterm, s.final, 0);
    else
        return grade(s.midterm, s.final, median(nonzero));
}

double optimistic_median_analysis(const vector<Student_info>& students)
{
    vector<double> grades;
    transform(students.begin(), students.end(),
              back_inserter(grades), optimistic_median);
    return median(grades);
}

// code is taken from §6.2.2/113
double grade_aux(const Student_info& s)
{
    try {
        return grade(s);
    } catch (domain_error) {
        return grade(s.midterm, s.final, 0);
    }
}

// code is taken from §6.2.2/113
double median_analysis(const vector<Student_info>& students)
{
    vector<double> grades;

    transform(students.begin(), students.end(),
              back_inserter(grades), grade_aux);
    return median(grades);
}

// code is taken from §6.2.3/115
double average(const vector<double>& v)
{
    return accumulate(v.begin(), v.end(), 0.0) / v.size();
}

// code is taken from §6.2.3/115
double average_grade(const Student_info& s)
{
    return grade(s.midterm, s.final, average(s.homework));
}

// code is taken from §6.2.3/115
double average_analysis(const vector<Student_info>& students)
{
    vector<double> grades;

    transform(students.begin(), students.end(),
              back_inserter(grades), average_grade);
    return median(grades);
}

// code is taken from §6.2.1/110
bool did_all_hw(const Student_info& s)
{
    return ((find(s.homework.begin(), s.homework.end(), 0))
        == s.homework.end());
}

// code is taken from §6.2.2/113
void write_analysis(ostream& out, const string& name,
                    double analysis(const vector<Student_info>&),
                    const vector<Student_info>& did,
                    const vector<Student_info>& didnt)
{
    out << name << ": median(did) = " << analysis(did) <<
                   ", median(didnt) = " << analysis(didnt) << endl;
}

void write_analysis(ostream& out, const string& name,
                    double metric(const Student_info&),
                    const vector<Student_info>& did,
                    const vector<Student_info>& didnt)
{
    out << name << ": median(did) = " << analysis(did, metric) <<
                   ", median(didnt) = " << analysis(didnt, metric) << endl;
}


// main code harness:
int main(int argc, char** argv)
{
    // students who did and didn't do all their homework
    vector<Student_info> did, didnt;

    // read the student records and partition them
    Student_info student;
    // read all student data
    while (read(cin, student)) {
        if (did_all_hw(student)) 
            did.push_back(student);
        else
            didnt.push_back(student);
    }
    
    // verify that the analyses will show us something
    if (did.empty()) {
        cout << "No student did all the homework!" << endl;
        return 1;
    }
    if (didnt.empty()) {
        cout << "Every student did all the homework!" << endl;
        return 1;
    }

    // do the analyses
    write_analysis(cout, "median", median_analysis, did, didnt);
    write_analysis(cout, "average", average_analysis, did, didnt);
    write_analysis(cout, "median of homework turned in",
                   optimistic_median_analysis, did, didnt);

    return 0;
}