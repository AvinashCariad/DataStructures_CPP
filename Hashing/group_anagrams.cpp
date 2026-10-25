/*
	Given a set of words, group the words which are anagrams of each other.
	
	Input: array of string
	Output: 2d array of strings
	
	Solution:
		Same words will have the same characters in frequency. So we can just sort the words and then hash the value
		in a hash table. Then we can just do a look up for each word if it is already there then just put it in the same
		group, otherwise make another group.

		TC: O(n * mlogm), n: no. of words, m: max length of a word 
		SC: O(n), n: no. of words
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

// Groups words that have at least one anagram.
unordered_map<string, vector<string>>
groupAnagrams(const vector<string>& arr)
{
    unordered_map<string, vector<string>> anagrams;

    // Build groups.
    for (const string& word : arr)
    {
        string sorted_word = word;

        sort(sorted_word.begin(),
             sorted_word.end());

        anagrams[sorted_word].push_back(word);
    }

    // Remove groups containing only one word.
    for (auto it = anagrams.begin();
         it != anagrams.end();)
    {
        if (it->second.size() < 2)
        {
            it = anagrams.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return anagrams;
}

void printAnagramGroups(
    const unordered_map<string, vector<string>>& groups)
{
    for (const auto& group : groups)
    {
        for (const string& word : group.second)
        {
            cout << word << " ";
        }

        cout << '\n';
    }
}


// main code harness:
int main(int argc, char** argv)
{
    vector<string> arr = {
        "dog",
        "god",
        "kite",
        "debitcard",
        "badcredit"
    };

    auto groups = groupAnagrams(arr);

    printAnagramGroups(groups);

    return 0;
}









/*
// groups anagrams in groups
unordered_map<string, vector<string>> groupAnagrams(const vector<string>& arr) {
	// make a hash table for storing the anagrams
	unordered_map<string, vector<string>> anagrams;

	// do a traversal 
	for(const string& word: arr){
		string sorted_word = word;
		// sort the word so that it can be matched lexographically
		sort(sorted_word.begin(), sorted_word.end());
		// add the word to the hash table
		anagrams[sorted_word].emplace_back(word);     // create a map from sorted 
	} 

	// now all the keys with atleast 2 words mean that it has anagrams
	for(unordered_map<string, vector<string>> :: iterator it = anagrams.begin();
			it != anagrams.end(); it++) {
		// if the size is less than 2, remove it 
		if(it->second.size() < 2){
			anagrams.erase(it->first);
		} else {
			++it;
		}
	}

	return anagrams; 
}


// print anagrams:
void printAnagramGroups(unordered_map<string, vector<string>>& groups) {
	for(const auto anagrams: groups) {
		for(const auto words: anagrams.second) {
			cout << words << " ";
		}
		cout << endl;
	}
}


// main code harness:
int main(int argc, char** argv) {
	
	vector<string> arr = {"dog", "god", "kite", "debitcard", "badcredit"};

	unordered_map<string, vector<string>> groups = groupAnagrams(arr);

	printAnagramGroups(groups);
	
	std::cout << endl;
	
	return 0;
}

*/
