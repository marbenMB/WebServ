#include <iostream>


std::string trimFront(const std::string &s, std::string trim)
{
	size_t start = s.find_first_not_of(trim.c_str());
	return (start == std::string::npos) ? "" : s.substr(start);
}