#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    unordered_map<string, vector<string>> graph;        // build graph
    unordered_map<string, int> distance;                // distance

public:
    // define edge:
    bool has_edge(const string& str1, const string& str2) {
        if (str1.length() != str2.length()) {
            return false;
        }

        int diff = 0;

        for (int i = 0; i < (int)str1.length(); ++i) {
            if (str1[i] != str2[i]) {
                diff++;
            }

            if (diff > 1) {
                return false;
            }
        }

        return diff == 1;
    }

    // build graph:
    void build_graph(vector<string>& wordList) {
        for (int from = 0; from < (int)wordList.size(); ++from) {
            for (int to = from + 1; to < (int)wordList.size(); ++to) {
                if (has_edge(wordList[from], wordList[to])) {
                    // Build directed graph:
                    graph[wordList[from]].push_back(wordList[to]);
                    graph[wordList[to]].push_back(wordList[from]);
                }
            }
        }
    }


    // code routine for the ladder:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        bool has_found = false;

        for (const auto& word : wordList) {
            if (word == endWord) {
                has_found = true;
                break;
            }
        }

        if (!has_found) {
            return 0;
        }

        wordList.push_back(beginWord);      // push the begin word to the wordlist

        build_graph(wordList);              // build graph now

        queue<string> q;
        q.push(beginWord);

        distance[beginWord] = 1;

        while (!q.empty()) {
            string cur_word = q.front();
            q.pop();

            if (cur_word == endWord) {
                return distance[cur_word];
            }

            for (const auto& neighbour : graph[cur_word]) {
                if (distance.count(neighbour)) {
                    continue;
                }

                distance[neighbour] = distance[cur_word] + 1;
                q.push(neighbour);
            }
        }

        return 0;
    }
};



// main code harness:
int main(int argc, char** argv) {
    Solution sol;

    string beginWord = "hit";
    string endWord = "cog";

    vector<string> wordList = {
        "hot", "dot", "dog", "lot", "log", "cog"
    };

    int result = sol.ladderLength(beginWord, endWord, wordList);

    cout << "Shortest ladder length: " << result << endl;

    return 0;
}
