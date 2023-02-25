#include "../../include/serverSide.hpp"

void	printMultiMap(std::multimap<std::string, int> mmap)
{
	for (std::multimap<std::string, int>::iterator it = mmap.begin(); it != mmap.end(); it++)
	{
		std::cout << "IP : " << it->first << " : PORT : " << it->second << std::endl;
	}
}