#include "../../include/request.hpp"

bool is__subDir(const std::string Dir, const std::string subDir)
{
    
    bool status = true;

    char _Dir[PATH_MAX];
    char _subDir[PATH_MAX];

    realpath(Dir.c_str(), _Dir);
    realpath(subDir.c_str(), _subDir);

    // std::cout << "#DIR :" << _Dir << std::endl;
    // std::cout << "#SUBDIR :" << _subDir << std::endl;
    std::string tmp(_subDir);

    size_t pos = tmp.find(_Dir, 0);
    if (pos != 0){
        status = false;
        // std::cerr << "`"<<tmp<<"`Not sub Dir [P:"<< pos <<"]\n";
    }
    return status;
}

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

std::string trimFront(const std::string &s, std::string trim)
{
	size_t start = s.find_first_not_of(trim.c_str());
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string trimBack(const std::string &s, std::string trim)
{
	size_t start = s.find_last_not_of(trim.c_str());
	return (start == std::string::npos) ? "" : s.substr(0,start+1);
}