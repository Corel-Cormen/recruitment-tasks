#include <iostream>

using namespace std;

void compression(string &str);                  // function compressing string
void decompression(string &str);                // function decompressing string
bool checkCompression(const string &str);       // check if the string is valid to compression
bool checkDecompression(const string &str);     // check if the string is valid to decompression

int main()
{
    string s = "aaaabbcdd";

    try {
        compression(s);
        cout << s << endl;
        decompression(s);
        cout << s << endl;
    } catch(std::invalid_argument &e) {
        cerr << e.what() << endl;
    }

    return 0;
}

void compression(string &str)
{
    if(!checkCompression(str))
        throw std::invalid_argument("Compression invalid string as argument.");

    char c = *str.begin();
    string newStr;
    int counter = 1;

    for(std::string::iterator it = str.begin()+1; it != str.end(); it++)
    {
        if(c == *it)
        {
            counter++;
            continue;
        }

        newStr += (c + std::to_string(counter));
        c = *it;
        counter = 1;
    }
    newStr += (c + std::to_string(counter));
    str = newStr;
}

void decompression(string &str)
{
    if(!checkDecompression(str))
        throw std::invalid_argument("Compression invalid string as argument.");

    char c = str.at(0);
    string number = "0";
    string newStr;

    for(std::string::iterator it = str.begin(); it < str.end(); it++)
    {
        if((*it >= 'a' && *it <= 'z'))
        {
            for(int i = 0; i < std::atoi(number.c_str()); i++)
                newStr += c;

            number.clear();
            c = *it;
            continue;
        }
        number += *it;
    }
    for(int i = 0; i < std::atoi(number.c_str()); i++)
        newStr += c;

    str = newStr;
}

bool checkCompression(const string &str)
{
    if(str.empty())
        return false;

    for(auto s : str)
    {
        if(s < 'a' || s > 'z')
            return false;
    }
    return true;
}

bool checkDecompression(const string &str)
{
    if(str.empty())
        return false;
    if(str.at(0) < 'a' || str.at(0) > 'z')
        return false;
    if(str.at(str.length()-1) < '0' || str.at(str.length()-1) > '9')
        return false;

    for(auto it = str.begin(); it != str.end()-1; it++)
    {
        if((*it < 'a' || *it > 'z') && (*it < '0' || *it > '9'))
            return false;
        if((*it >= 'a' && *it <= 'z') && (*(it+1) >= 'a' && *(it+1) <= 'z'))
            return false;
    }

    return true;
}
