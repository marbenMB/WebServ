#include "../../include/serverSide.hpp"

void	printMultiMap(std::multimap<std::string, int> mmap)
{
	for (std::multimap<std::string, int>::iterator it = mmap.begin(); it != mmap.end(); it++)
	{
		std::cout << "IP : " << it->first << " : PORT : " << it->second << std::endl;
	}
}

std::vector<std::string>	getStringKeyVal(std::map<std::string, std::vector<std::string> > myMap, std::string key)
{
	std::string		value;
	std::stringstream	ss;

	if (myMap.find(key) != myMap.end())
		return myMap[key];
	return	std::vector<std::string>();
}