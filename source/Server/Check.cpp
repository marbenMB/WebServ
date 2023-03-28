#include "../../include/serverSide.hpp"

int	checkDuplicatePort(std::multimap<std::string, int> Map)
{
	std::multimap<std::string, int>::iterator	it = Map.begin();
	std::multimap<std::string, int>::iterator	Scit = Map.begin();

	while (it != Map.end() && Scit != Map.end())
	{
		Scit++;
		while (Scit != Map.end())
		{
			if (it->first == Scit->first)
				if (it->second == Scit->second)
					return DUP_PORT;
			Scit++;
		}
		it++;
		Scit = it;
	}
	return 0;
}

bool	checkDefaultServer(std::map<SockProp, std::vector<Server> > &servMap,std::string ip, int port, Server serv)
{
	for (std::map<SockProp, std::vector<Server> >::iterator it = servMap.begin(); it != servMap.end(); it++)
	{
		if (it->first._IP == ip && it->first._Port == port)
		{
			it->second.push_back(serv);
			return false;
		}
	}
	return true;
}