#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> split(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> substrings;
    std::string::size_type start = 0;
    std::string::size_type end = str.find(delimiter);
    int index = 0;

    while (end != std::string::npos)
    {
        std::string substring = str.substr(start, end - start);
        // if (substring.length() == 0 && index == 0){
        //     continue;
        // }
        // trim spaces from the begining and end of the substring
        substring.erase(0, substring.find_first_not_of(" "));
        substring.erase(substring.find_last_not_of(" ") + 1);
        if (substring.length())
            substrings.push_back(substring);
        start = end + delimiter.length();
        end = str.find(delimiter, start);
        index++;
    }
    std::string substring = str.substr(start, end);
    // trim spaces from the begining and end of the substring
    substring.erase(0, substring.find_first_not_of(" "));
    substring.erase(substring.find_last_not_of(" ") + 1);
    substrings.push_back(substring);
    return substrings;
}