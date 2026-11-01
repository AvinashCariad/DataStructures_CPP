// string_12_02.h --for sol-12-02.cpp

#ifndef STRING_H_
#define STRING_H_
#include <iostream>
using std::ostream;
using std::istream;

class String
{
    private:
        char *str;          // private member class
        int len;
        static int num_strings;
        static const int CINLIM = 80;
    public:
        String(const char *s);
        String();
        // Note:
        String(const String &);
        ~String();
        
        int length() const {return len;}
        int has(const char);
        void stringlow();
        void stringup();
        
        // Note:
        String & operator=(const String &);
        String & operator=(const char *);
        
        char & operator[](int i);
        const char & operator[](int i) const;
        // friends:
        friend bool operator<(const String &st1, const String &st2);
        friend bool operator>(const String &st1, const String &st2);
        friend bool operator==(const String &st1, const String &st2);
        friend ostream & operator<<(ostream &os, const String &st);
        friend istream & operator>>(istream &is, String &st);
        friend String operator+(const String &st1, const String &st2);
        friend String operator+(const char *, const String &);
        friend String operator+(const String &, const char *);
        static int HowMany();
};
#endif // STRING_H


// string_12_02.cpp --for sol-12-02.cpp
#include <cstring>
using std::cin;
using std::cout;

int String::num_strings = 0;
int String::HowMany()
{
    return num_strings;
}

String::String(const char *s)
{
    len = std::strlen(s);
    str = new char[len + 1];
    std::strcpy(str, s);
    num_strings++;
}
String::String()
{
    len = 4;
    str = new char[1];
    str[0] ='\0';
    num_strings++;
}
String::String(const String & st)
{
    num_strings++;
    len = st.len;
    str = new char[len + 1];
    std::strcpy(str, st.str);
}
String::~String()
{
    --num_strings;
    delete [] str;
}
int String::has(const char s)
{
    int cnt = 0;
    for(int i = 0; i < len; i++)
        if(str[i] == 'A')
            cnt++;
    return cnt;
}
void String::stringlow()
{
    for(int i = 0; i < len; i++)
        str[i] = tolower(str[i]);
}
void String::stringup()
{
    for(int i = 0; i < len; i++)
        str[i] = toupper(str[i]);
}
String operator+(const String &st1, const String &st2)
{
    char tmp[st1.len + st2.len + 1];
    std::strcat(std::strcpy(tmp, st1.str), st2.str);     // note this
    return String(tmp);
}

String operator+(const char *s, const String &st)
{
    return String(s) + st;
}

String operator+(const String &st, const char *s)
{
    return st + String(s);
}

// 
String & String::operator=(const String & st)
{
    if(this == &st)
        return *this;
    delete [] str;
    len = st.len;
    str = new char[len + 1];
    std::strcpy(str, st.str);
    return *this;
}
String & String::operator=(const char *s)
{
    delete [] str;
    len = std::strlen(s);
    str = new char[len + 1];
    std::strcpy(str, s);
    return *this;
}
char& String::operator[](int i)
{
    return str[i];
}

const char & String::operator[](int i) const
{
    return str[i];
}

bool operator<(const String &st1, const String &st2)
{
    return (std::strcmp(st1.str, st2.str) < 0);
}

bool operator>(const String &st1, const String &st2)
{
    return st2.str < st1.str;
}
bool operator==(const String &st1, const String &st2)
{
    return (std::strcmp(st1.str, st2.str) == 0);
}
ostream & operator<<(ostream &os, const String &st)
{
    os << st.str;
    return os;
}

istream & operator>>(istream &is, String &st)
{
    char temp[String::CINLIM];
    is.get(temp, String::CINLIM);
    if(is)
        st = temp;
    while(is && is.get() != '\n')
        continue;
    return is;
}


// Bare-bone implementation of all the header funcs above:
//  --------------------- S.Prata C++ Primer Plus 6th ed. ----------------------
//  ----------------------- Programming Exercises 12.2 -------------------------

//#include "string_12_02.h"
#include <iostream>
using namespace std;


// main code harness:
int main(void)
{
    String s1(" and I am a C++ student.");
    String s2 = "Please enter your name: ";
    String s3;
    cout << s2;
    cin >> s3;
    s2 = "My name is  " + s3;
    cout << s2 << ".\n";
    s2 = s2 + s1;
    s2.stringup();
    cout << "The string\n" << s2 << "\ncontains " << s2.has('A')
        << " 'A' characters in it.\n";
    s1 = "red";
    String rgb[3] = {String(s1), String("green"), String("blue")};
    cout << "Enter the name of a primary coler for mixing light: ";
    String ans;
    bool success = false;
    while(cin >> ans)
    {
        ans.stringlow();
        for(int i = 0; i < 3; i++)
        {
            if(ans == rgb[i])
            {
                cout << "That's right!\n";
                success = true;
                break;
            }
        }
        if(success)
            break;
        else
            cout << "Try again!\n";
    }
    cout << "Bye\n";
    return 0;
}





