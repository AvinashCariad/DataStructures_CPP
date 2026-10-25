/*
 * Objective:
 * Write a C++ program to determine whether a
 * given string is palindrome or not.
 *
 * Input: malayalam
 * Output: yes
 *
 * Input: algorithm
 * Output: no
 */
#include <iostream>
#include <string>
#include <algorithm>  // For transform

/*
 * A palindrome is a string that reads the same
 * way in both the direction.
 * For e.g. racecar
 */

/*
 * Iterative Approach:
 *
 * Time Complexity: Theta(N)
 * Space Complexity: Theta(1)
 */
bool isPalindrome(std::string str) {
   int s = 0; // Represents starting index
   int e = str.length() - 1; // Represents ending index
   while (s < e) {
      if (str[s] != str[e]) { return false; }
      s++; e--;
   } 
   return true;
}

/*
 * Recursive Approach
 *
 * Time Complexity: Theta(N)
 * Space Complexity: Theta(N)
 */  
bool isPalindromeRec(std::string str, int start, int end) {
   if (start == end) { return true; } // 1 char long string case
   if (str[start] != str[end]) { return false; }
   if (start < end) {
      return isPalindromeRec(str, start+1, end-1);    // shrink now
   }
   return true;
}


// main code harness:
int main(int argc, char** argv) {
   std::cout << "Program that determines whether a given string is palindrome or not!" << std::endl;
   std::string str;
   std::cout << "Enter the string: ";
   std::cin >> str;
   // Converting to lower case to make it case insensitive
   transform(str.begin(), str.end(), str.begin(), ::tolower);
#if 0
   if (isPalindrome(str)) { std::cout << "It's a palindrome!" << std::endl; }
   else { std::cout << "It's not a palindrome" << std::endl; }
#endif
#if 1 
   if (isPalindromeRec(str, 0, str.length()-1)) { std::cout << "It's a palindrome!" << std::endl; }
   else { std::cout << "It's not a palindrome" << std::endl; }
#endif
   return 0;
}