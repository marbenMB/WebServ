#include "../../include/unitTests.hpp"

void	printWebServ (WebServ *myServ)
{
	for (std::vector<Server>::iterator it = myServ->servers.begin(); it != myServ->servers.end(); it++)
	{
		std::cout << "------------ Server ID : " << it->getID() << " ------------" << std::endl;
		printVector(it->getCombIpPort(), "Server Ports : ");
		printVector(it->getServerName(), "Server Names : ");
		printMultiMap(it->getIpPort());
		std::cout << "---------------------------------------" << std::endl;
		std::cout << std::endl;
	}
}

void	printSockProp (std::map<SockProp, std::vector<Server> > m)
{
	std::map<SockProp, std::vector<Server> >::iterator	mapIt = m.begin();

	while (mapIt != m.end())
	{
		std::cout << "* Socket FD : " << mapIt->first._fd << std::endl
		<< "* IP : " << mapIt->first._IP << " : PORT : " << mapIt->first._Port << std::endl;
		for (std::vector<Server>::iterator sIt = mapIt->second.begin(); sIt != mapIt->second.end(); sIt++)
		{
			printVector(sIt->getCombIpPort(), "Server Ports : ");
			printVector(sIt->getServerName(), "Server Names : ");
		}
		std::cout << "********************" << std::endl;
		mapIt++;
	}
}