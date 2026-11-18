#ifndef GUARD_Student_info
#define Guard_Student_info

#include <vector>
#include <string>
#include <stdexcept>
#include <vector>
//#include "grade.h"
//#include "median.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <list>
using std::istream;                  // note the use of istream
using namespace std;




struct Student_info {
    std::string name;
    double final_grade;
};

bool compare(const Student_info&, const Student_info&);
std::istream& read(std::istream&, Student_info&);
std::istream& read_hw(std::istream&, std::vector<double>&);
#endif


#ifndef GUARD_grade_h
#define GUARD_grade_h

double grade(double, double, double);
double grade(double, double, const std::vector<double>&);

#endif


#ifndef GUARD_median_h
#define GUARD_median_h

double median(std::vector<double>);

#endif


using std::domain_error;
using std::vector;
using std::sort;

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

// extract all the students who have failed:
vector<Student_info> extract_fails(vector<Student_info>& students)
{
    vector<Student_info> fails;
    vector<Student_info>::iterator it = students.begin();
    while (it != students.end()) {
        // we can access only the final grade from simplified version
        if (it->final_grade < 60) {
            fails.push_back(*it);
            it = students.erase(it);
        } else
            ++it;
    }

    return fails;   // return the list of all the students who have failed
}


// use of list instead of vector: we cannot do indexing as we do it for vector:
list<Student_info> extract_fails_list(list<Student_info>& students)
{
    list<Student_info> fails;
    list<Student_info>::iterator it = students.begin();
    while (it != students.end()) {
        // we can access only the final grade from simplified version
        if (it->final_grade < 60) {
            fails.push_back(*it);
            it = students.erase(it);
        } else
            ++it;
    }

    return fails;
}


// ues of erase in the cpp code:
// code is taken from §5.1.1/77, modified to fit the simplified
// Student_info structure.
typedef vector<Student_info>::size_type vec_std_sz;
vector<Student_info> extract_fails(vector<Student_info>& students)
{
    vector<Student_info> fail;
    vec_std_sz i = 0;

    // invariant: elements [0, i) of students represent passing grades
    while (i != students.size()) {
        if (students[i].final_grade < 60) {
            fail.push_back(students[i]);
            students.erase(students.begin() + i);        // note how the erase is being used  
        } else
            ++i;
    }
    return fail;
}



vector<Student_info> resizing_extract_fails(vector<Student_info>& students)
{
    vector<Student_info> fail;
    vec_std_sz original_size = students.size();
    
    for (vec_std_sz i = 0; i != students.size(); ++i)
        if (students[i].final_grade < 60)
            fail.push_back(students[i]);
        else
            // vector.insert(position, value);
            students.insert(students.begin(), students[i++]);     // inserts at the beginning and increments i

    students.resize(original_size - fail.size());
    return fail;
}


// main code harness:
int main(int argc, char** argv)
{
    vector<Student_info> students;
    Student_info student;

    // read all student data
    while (read(cin, student))
        students.push_back(student);

    // save the original size
    vector<Student_info>::size_type count = students.size();

    double begin = clock();
    vector<Student_info> fails = extract_fails(students);
    double end = clock();

    cout << "Extracting fails for " << count << " students took "
        << (end - begin) / CLOCKS_PER_SEC << " seconds." << endl;


    // save the original size
    list<Student_info>::size_type count1 = students.size();


    //list<Student_info> fails_list = extract_fails_list(students);
    
    return 0;
}







