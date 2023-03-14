#include <iostream>


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