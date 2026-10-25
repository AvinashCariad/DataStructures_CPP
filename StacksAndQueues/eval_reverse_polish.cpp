/*
	Given a reverse polish expression, evaluate it. In simple terms operators are placed after the operands for each operation
	in reverse polish.
	Input: string
	Output: int/string

	Solution:
		This can be done using a stack. We start scanning from left to right. If we encounter a number/alphabet we push it to
		the stack. If we get an operator (/, *, +, -) we pop the top two operands from stack, apply the operator and again
		push the result.

		TC: O(n), n: no. of elements
		 
*/


#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <string>
using namespace std;

// returns the reverse polish result
int evaulateReversePolish(const string& expr) {
	stringstream ss(expr);
	string token;
	const char delimiter = ',';

	stack<int> operands;    // note how the stack is defined
	while(getline(ss, token, delimiter)) {        
		if(token == "*" || token == "/" || token == "-" || token == "+") {
			// The top element is the RIGHT operand
			int right = operands.top();
			operands.pop();
			// The next element is the LEFT operand
			int left = operands.top();
			operands.pop();

			switch(token.front()) {
				case '*':
					operands.emplace(left * right);
					break;
				case '/':
					operands.emplace(left / right);
					break;
				case '+':
					operands.emplace(left + right);
					break;
				case '-':
					operands.emplace(left - right);
					break;
			}
		}
		else {
			operands.emplace(stoi(token));
		}
	}
	return operands.top();
}


// main code harness:
int main(int argc, char** argv) {
	// Evaluates as 3 + 4 = 7
	string expr1 = "3,4,+";
	cout << expr1 << " = " << evaulateReversePolish(expr1) << endl;

	// Evaluates as 3 - 4 = -1
	string expr2 = "3,4,-";
	cout << expr2 << " = " << evaulateReversePolish(expr2) << endl;
	
	return 0;
}
