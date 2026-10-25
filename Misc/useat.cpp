#include <iostream>
#include <map>

using namespace std;


// main code harness:
int main(int argc, char** argv)
{
    map<string, int> age;

    age["Alice"] = 25;
    age["Bob"] = 30;

    cout << age.at("Alice") << endl;
    cout << age.at("Bob") << endl;

    return 0;
}
