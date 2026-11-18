#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <stdexcept>


using namespace std;

typedef vector<string> Rule;
typedef vector<Rule> Rule_collection;
typedef map<string, Rule_collection> Grammar;

vector<string> split(const string& s) {
    vector<string> ret;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        while (i != s.size() && isspace(s[i]))
            ++i;

        string_size j = i;
        while (j != s.size() && !isspace(s[j]))
            ++j;

        if (i != j) {
            ret.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return ret;
}

Grammar read_grammar(istream& in) {
    Grammar ret;
    string line;

    while (getline(in, line)) {                    // note line is a std::string 
        vector<string> entry = split(line);        // this converts string into vector<string>, this is why Rule is a vector<std::string>
        if (!entry.empty())
            ret[entry[0]].push_back(Rule(entry.begin() + 1, entry.end()));
    }
    return ret;
}

bool bracketed(const string& s) {
    return s.size() > 1 && s[0] == '<' && s[s.size() - 1] == '>';
}

int nrand(int n) {
    if (n <= 0 || n > RAND_MAX)
        throw domain_error("Argument to nrand is out of range");
    
    const int bucket_size = RAND_MAX / n;
    int r;

    do r = rand() / bucket_size;
    while (r >= n);

    return r;
}

// Non-recursive (iterative) version using a vector as a stack
vector<string> gen_sentence(const Grammar& g) {
    vector<string> sentence;
    vector<string> stack;

    // Start with the root rule on the stack
    stack.push_back("<sentence>");

    while (!stack.empty()) {
        // Grab the item at the top (back) of the stack and remove it
        string word = stack.back();
        stack.pop_back();

        if (!bracketed(word)) {
            // Plain word: add it directly to our final sentence
            sentence.push_back(word);
        } else {
            // Rule category: look it up in the grammar
            Grammar::const_iterator it = g.find(word);
            if (it == g.end())
                throw logic_error("empty rule");

            // Pick a random rule expansion
            const Rule_collection& c = it->second;       // NB: one key can have multiple values 
            const Rule& r = c[nrand(c.size())];

            // Push the rule's tokens onto the stack in REVERSE order
            // so the first token ends up on top (at the back)
            for (Rule::const_reverse_iterator i = r.rbegin(); i != r.rend(); ++i) {
                stack.push_back(*i);
            }
        }
    }

    return sentence;
}


// main code harness:
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    string grammar_data = 
        "<noun> cat\n"
        "<noun> dog\n"
        "<noun> table\n"
        "<noun-phrase> <noun>\n"
        "<noun-phrase> <adjective> <noun-phrase>\n"
        "<adjective> large\n"
        "<adjective> brown\n"
        "<adjective> absurd\n"
        "<verb> jumps\n"
        "<verb> sits\n"
        "<location> on the stairs\n"
        "<location> under the sky\n"
        "<location> wherever it wants\n"
        "<sentence> the <noun-phrase> <verb> <location>\n";

    istringstream input(grammar_data);
    Grammar g = read_grammar(input);

    for (int count = 0; count < 3; ++count) {
        // every time the loop is run - it generates a new sentence
        vector<string> sentence = gen_sentence(g);

        vector<string>::const_iterator it = sentence.begin();
        if (!sentence.empty()) {
            cout << *it;
            ++it;
        }
        while (it != sentence.end()) {
            cout << " " << *it;
            ++it;
        }
        cout << "." << endl;
    }

    return 0;
}



/*

// same code without the use of Rule_Collection and Grammar DS

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

// Only keeping the Rule typedef
typedef vector<string> Rule;

Rule split(const string& s) {
    Rule ret;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        while (i != s.size() && isspace(s[i]))
            ++i;

        string_size j = i;
        while (j != s.size() && !isspace(s[j]))
            ++j;

        if (i != j) {
            ret.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return ret;
}

// Replaces Grammar with map<string, vector<Rule>>
map<string, vector<Rule>> read_grammar(istream& in) {
    map<string, vector<Rule>> ret;
    string line;

    while (getline(in, line)) {
        Rule entry = split(line);
        if (!entry.empty())
            ret[entry[0]].push_back(Rule(entry.begin() + 1, entry.end()));
    }
    return ret;
}

bool bracketed(const string& s) {
    return s.size() > 1 && s[0] == '<' && s[s.size() - 1] == '>';
}

// generate rand:
int nrand(int n) {
    if (n <= 0 || n > RAND_MAX)
        throw domain_error("Argument to nrand is out of range");
    
    const int bucket_size = RAND_MAX / n;
    int r;

    do r = rand() / bucket_size;
    while (r >= n);

    return r;
}

// Non-recursive sentence generator using only Rule and standard types
Rule gen_sentence(const map<string, vector<Rule>>& g) {
    Rule sentence;
    Rule stack;

    stack.push_back("<sentence>");

    while (!stack.empty()) {
        string word = stack.back();
        stack.pop_back();

        if (!bracketed(word)) {
            sentence.push_back(word);
        } else {
            // Replaces Grammar::const_iterator
            map<string, vector<Rule>>::const_iterator it = g.find(word);
            if (it == g.end())
                throw logic_error("empty rule");

            // Replaces const Rule_collection&
            const vector<Rule>& c = it->second;
            const Rule& r = c[nrand(c.size())];

            for (Rule::const_reverse_iterator i = r.rbegin(); i != r.rend(); ++i) {
                stack.push_back(*i);
            }
        }
    }

    return sentence;
}


// main code harness:
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    string grammar_data = 
        "<noun> cat\n"
        "<noun> dog\n"
        "<noun> table\n"
        "<noun-phrase> <noun>\n"
        "<noun-phrase> <adjective> <noun-phrase>\n"
        "<adjective> large\n"
        "<adjective> brown\n"
        "<adjective> absurd\n"
        "<verb> jumps\n"
        "<verb> sits\n"
        "<location> on the stairs\n"
        "<location> under the sky\n"
        "<location> wherever it wants\n"
        "<sentence> the <noun-phrase> <verb> <location>\n";

    istringstream input(grammar_data);
    map<string, vector<Rule>> g = read_grammar(input);

    for (int count = 0; count < 3; ++count) {
        Rule sentence = gen_sentence(g);

        Rule::const_iterator it = sentence.begin();
        if (!sentence.empty()) {
            cout << *it;
            ++it;
        }
        while (it != sentence.end()) {
            cout << " " << *it;
            ++it;
        }
        cout << "." << endl;
    }

    return 0;
}


*/