
/*
	Given a text file which contains scores and student id in new line. So each record is there on a new line.
	Find the top scoring student. Consider using only the top 3 scores for computing the avg., students with
	atleast 3 test scores are only considered.

	Input: file
	Output: string 

	Solution:
		Use a hash table for storing the student id and their scores.
		We use a priority queue of 3 elements as value for keys, whose creation is O(3)~O(1).
		Now for after going through all the scores, we traverse through the table once and
		compute the avg, while keeping track of max.
		Since heapifying takes O(n), so in worst case it will take O(n), n: no. of test scores

		TC: O(n)
		SC: O(m), m: no. of distinct students, worst case: O(n), no. of scores=no. of students
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>

using namespace std;

string findStudentWithHighestScore(ifstream& in)
{
    // Min-heap containing the top 3 scores for each student.
    unordered_map<
        string,
        priority_queue<int, vector<int>, greater<int>>
    > student_records;

    string name;
    int score;

    // Read:
    //
    // Alice 90
    // Bob   80
    // Alice 95
    // ...

    while (in >> name >> score)
    {
        auto& scores = student_records[name];

        scores.push(score);

        // Keep only the top 3 scores.
        if (scores.size() > 3)
        {
            scores.pop();
        }
    }

    string top_student;
    int top_score = -1;

	// Iterate: 
    for (auto& record : student_records)
    {
        // Student must have at least 3 scores.
        if (record.second.size() == 3)
        {
            int curr_score = 0;

            while (!record.second.empty())
            {
                curr_score += record.second.top();
                record.second.pop();
            }

            if (curr_score > top_score)
            {
                top_score = curr_score;
                top_student = record.first;
            }
        }
    }

    return top_student;
}


// main code harness:
int main(int argc, char** argv)
{
    ifstream input("scores.txt");

    if (!input.is_open())
    {
        cerr << "Could not open scores.txt\n";
        return 1;
    }

    string student =
        findStudentWithHighestScore(input);

    if (student.empty())
    {
        cout << "No student has at least 3 scores.\n";
    }
    else
    {
        cout << "Top student: "
             << student
             << '\n';
    }

    return 0;
}












/*
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// finds the top scoring student
string findStudentWithHighestScore(ifstream* in) {
	unordered_map<string, priority_queue<int, vector<int>, greater<int>>> student_records;
	int score;
	string name;

	// make the student score record hash table
	while(in >> name >> score) {
		student_records[name].emplace(score);
		// if there are more than 3 scores, remove 1
		if(student_records[name].size() > 3)
			student_records[name].pop();
	}

	// find the top scoring student
	string top_student;
	int top_score = 0;
	int curr_score = 0;

	for(auto& record: student_records) {
		// only consider student with 3 scores
		if(record->second.size() == 3) {
			// take the sum of top 3 test scores from heap
			while(!record->second.empty()) {
				curr_score += record->second.top();
				record->second.pop();
			}

			// check if it is the highest score
			if(curr_score > top_score) {
				top_score = curr_score;
				top_student = record->first;
			}
		}
	}

	return top_student;
}

*/

