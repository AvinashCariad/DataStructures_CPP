#include <iostream>
#include <sstream> // Required for stringstream
#include <string>

using namespace std;

// main code harness:
int main(int argc, char** argv) {
    // 1. Define the input string and the separator
    string input = "Apple,Banana,Orange,Grape";
    char delimiter = ',';

    // 2. Load the string into a stringstream so it behaves like an input file/stream
    stringstream ss(input);
    string token;

    cout << "Starting to split the string..." << endl << endl;

    // 3. The loop reads the stream piece by piece until it runs out of text
    int count = 1;
    while (getline(ss, token, delimiter)) {
        cout << "Loop Step " << count << ":" << endl;
        cout << "  Extracted piece (token): " << token << endl;
        cout << "-------------------------" << endl;
        count++;
    }

    cout << "The stream is empty. Loop finished!" << endl;
    return 0;
}
