/*
 * Recursive Linear Search Function
 *
 * @author
 *   Abhilash Muraleedharan<amuraleedharan13@gmail.com>
 */
#include <iostream>

/*
 * Recursive Linear Search Function
 * Time Complexity: O(n)
 * Space Complexity: O(n)
 */
bool search(int arr[], int length, int e) {
   if (length == 0) { return false; }
   if (arr[length-1] == e) { return true; }
   else { return search(arr, length-1, e); }
}


// main code harness:
int main(int argc, char** argv) {
   int length, element;
   std::cout << "This program checks whether an element X is present in the given unsorted array of elements" << std::endl;
   std::cout << "Enter the length of the array: ";
   std::cin >> length;
   int *arr = new int[length];
   std::cout << "Enter the array elements" << std::endl;
   for (int i=0; i<length; i++) {
      std::cin >> arr[i];
   }
   std::cout << "Input array: " << std::endl;
   std::cout << "[";
   for (int i=0; i<length; i++) {
      if (i == length-1) { 
         std::cout << arr[i] << "]" << std::endl; 
      } else {
         std::cout << arr[i] << ",";
      }
   }
   std::cout << "Enter the element to search: ";
   std::cin >> element;

   if (search(arr, length, element)) { 
      std::cout << "Element " << element << " is found!" << std::endl;
   } else {
      std::cout << "Element " << element << " is not found!" << std::endl;
   }   
   delete []arr;
   return 0;
}