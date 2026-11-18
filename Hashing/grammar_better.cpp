#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>   // For istringstream (to simulate input)
#include <cctype>    // For isspace
#include <cstdlib>   // For rand, srand, RAND_MAX
#include <ctime>     // For time
#include <stdexcept> // For domain_error, logic_error

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

    // Build the grammar:
    while (getline(in, line)) {
        vector<string> entry = split(line);
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

// Note: Using a stack/LIFO order ensures words appear in the right sequence
void gen_aux(const Grammar& g, const string& word, vector<string>& ret) {
    if (!bracketed(word)) {
        ret.push_back(word);
    } else {
        Grammar::const_iterator it = g.find(word);
        if (it == g.end())
            throw logic_error("empty rule");

        const Rule_collection& c = it->second;
        const Rule& r = c[nrand(c.size())];
        
        // recurse:
        for (Rule::const_iterator i = r.begin(); i != r.end(); ++i)
            gen_aux(g, *i, ret);
    }
}

vector<string> gen_sentence(const Grammar& g) {
    vector<string> ret;
    gen_aux(g, "<sentence>", ret);
    return ret;
}


// main code harness:
int main(int argc, char** argv) {
    // Seed the random number generator so you get different sentences each run
    srand(static_cast<unsigned int>(time(0)));

    // Define sample grammar rules (simulating a text file or user input)
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

    // Read the grammar into the map
    istringstream input(grammar_data);
    Grammar g = read_grammar(input);

    // Generate and print 3 random sentences
    for (int count = 0; count < 3; ++count) {
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
