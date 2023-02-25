#include "../../include/serverSide.hpp"

int	checkDuplicatePort(std::multimap<std::string, int> Map)
{
	std::multimap<std::string, int>::iterator	it = Map.begin();
	std::multimap<std::string, int>::iterator	Scit = Map.begin();

	Scit++;
	while (it != Map.end() && Scit != Map.end())
	{
		if (it->first == Scit->first)
			if (it->second == Scit->second)
				return DUP_PORT;
		it++;
		Scit++;
	}
	return 0;
}