#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

// Helper predicates to avoid std::isspace overload ambiguity in find_if
bool is_space(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

bool not_space(char c) {
    return !std::isspace(static_cast<unsigned char>(c));
}

// Generic split function parameterized by an Output Iterator type
template <class OutputIt>
void split(const std::string& text, OutputIt dest) {
    typedef std::string::const_iterator iter;
    iter i = text.begin();

    while (i != text.end()) {
        // Advance past leading whitespace
        i = std::find_if(i, text.end(), not_space);

        // Find the end of the current word
        iter j = std::find_if(i, text.end(), is_space);

        // If a word was found, write it to the output iterator
        if (i != text.end()) {
            *dest++ = std::string(i, j);
        }

        i = j;
    }
}


// main code harness:
int main(int argc, char** argv) {
    std::string sample = "Generic functions decouple algorithms from containers";

    // 1. Write words into a std::vector using back_inserter
    std::vector<std::string> vec_words;
    split(sample, std::back_inserter(vec_words));

    // 2. Write words into a std::list using back_inserter
    std::list<std::string> list_words;
    split(sample, std::back_inserter(list_words));

    // 3. Stream words directly to standard output using ostream_iterator
    std::cout << "Words streamed directly:\n";
    split(sample, std::ostream_iterator<std::string>(std::cout, "\n"));

    return 0;
}

