#include <iostream>
#include <fstream> 
#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <string>  
using namespace std;

// finds the top scoring student
string findStudentWithHighestScore(ifstream* in) {
	unordered_map<string, priority_queue<int, vector<int>, greater<int>>> student_records;
	int score;
	string name;

	// make the student score record hash table
	while(*in >> name >> score) {
		student_records[name].emplace(score);
		// if there are more than 3 scores, remove 1
		if(student_records[name].size() > 3)
			student_records[name].pop();
	}

	// find the top scoring student
	string top_student;
	int top_score = 0;

	// PART 2: Modified to use explicit iterators and pointers
	for (auto it = student_records.begin(); it != student_records.end(); ++it) {
		// Use a pointer to point to the current key-value element
		auto* record_ptr = &(*it);           // note this  
		
		// Reset curr_score inside the loop for each student
		int curr_score = 0; 

		// Use the arrow operator (->) to access members via the pointer
		if (record_ptr->second.size() == 3) {
			// take the sum of top 3 test scores from heap
			while (!record_ptr->second.empty()) {
				curr_score += record_ptr->second.top();
				record_ptr->second.pop();
			}

			// check if it is the highest score
			if (curr_score > top_score) {
				top_score = curr_score;
				top_student = record_ptr->first; 
			}
		}
	}

	return top_student;
}
