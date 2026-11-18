#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <string>
//#include "../Chapter 05/split.h"


using namespace std;
typedef vector<string> Rule;
typedef vector<Rule> Rule_collection;
typedef map<string, Rule_collection> Grammar;


vector<string> split(const string& s)
{
    vector<string> ret;
    typedef string::size_type string_size;
    string_size i = 0;

    // invariant: we have processed characters [original value of i, i)
    while (i != s.size()) {
        // ignore leading blanks
        // invariant: characters in range [original i, current i) are all spaces
        while (i != s.size() && isspace(s[i]))
            ++i;

        // find end of next word
        string_size j = i;
        // invariant: none of the characters in range [original j, current j) is a space
        while (j != s.size() && !isspace(s[j]))
            ++j;

        // if we found some nonwhitespace characters
        if (i != j) {
            // copy from s starting at i and taking j - i chars
            ret.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return ret;
}



// code is taken from §7.4.2/131
// read a grammar from a given input stream
Grammar read_grammar(istream& in)
{
    Grammar ret;
    string line;

    // read the input
    while (getline(in, line)) {

        // split the input into words
        vector<string> entry = split(line);

        if (!entry.empty())
            // use the category to store the associated rule
            ret[entry[0]].push_back(
                Rule(entry.begin() + 1, entry.end()));
    }
    return ret;
}

// code is taken from §7.4.3/132
bool bracketed(const string& s)
{
    return s.size() > 1 && s[0] == '<' && s[s.size() - 1] == '>';
}

// code is taken from §7.4.4/135
// return a random integer in the range [0, n)
int nrand(int n)
{
    if (n <= 0 || n > RAND_MAX)
        throw domain_error("Argument to nrand is out of range");
    
    const int bucket_size = RAND_MAX / n;
    int r;

    do r = rand() / bucket_size; while (r >= n);       // r is [0, n)

    return r;
}

// code is taken from §7.4.3/132
vector<string> gen_sentence(const Grammar& g)
{
    vector<string> rules; // holds the rules
    vector<string> sentence; // holds the generated sentence

    rules.push_back("<sentence>");
    while (!rules.empty()) {
        // locate the rule that corresponds to word
        Grammar::const_iterator it = g.find(rules[0]);
        if (it == g.end())
            throw logic_error("empty rule");

        // fetch the set of possible rules
        const Rule_collection& c = it->second;

        // from which we select one at random
        const Rule& r = c[nrand(c.size())];

        // remove the used rule
        rules.erase(rules.begin());

        // add results to their vectors
        Rule::iterator rule_it = rules.begin();
        for (Rule::const_iterator i = r.begin(); i != r.end(); ++i) {
            if (bracketed(*i)) {
                rule_it = rules.insert(rule_it, *i);
                ++rule_it;
            }
            else
                sentence.push_back(*i);
        }
    }
    
    return sentence;
}

// code is taken from §7.4.3/134
// main code harness:
int main(int argc, char** argv)
{
    // generate the sentence
    vector<string> sentence = gen_sentence(read_grammar(cin));

    // write the first word, if any
    vector<string>::const_iterator it = sentence.begin();
    if (!sentence.empty()) {
        cout << *it;
        ++it;
    }
    
    // write the rest of the words, each preceded by a space
    while (it != sentence.end()) {
        cout << " " << *it;
        ++it;
    }

    cout << endl;
    return 0;
}




/*
// an example on vector of string:
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::sort

int main() {
    // 1. Initialize a vector of strings
    std::vector<std::string> fruits = {"Apple", "Banana", "Cherry"};

    // 2. Add new elements to the end
    fruits.push_back("Date");
    fruits.push_back("Elderberry");

    // 3. Access and modify an element by index
    fruits[1] = "Blueberry"; // Replaces "Banana"

    // 4. Sort the strings alphabetically
    std::sort(fruits.begin(), fruits.end());

    // 5. Loop through and print all elements
    std::cout << "Fruit list (" << fruits.size() << " items):\n";
    for (const std::string& fruit : fruits) {
        std::cout << "- " << fruit << "\n";
    }

    // 6. Remove the last element
    fruits.pop_back();

    return 0;
}



*/