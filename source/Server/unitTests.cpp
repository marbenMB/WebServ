#include "../../include/unitTests.hpp"

void	printWebServ (WebServ &myServ)
{
	for (std::vector<Server>::iterator it = myServ.servers.begin(); it != myServ.servers.end(); it++)
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
		std::cout << "********************\n" << std::endl;
		mapIt++;
	}
}

void	printMapClient (std::map<int, ClientSock> m)
{
	for (std::map<int, ClientSock>::iterator it = m.begin(); it != m.end(); it++)
	{
		std::cout << it->first << " -> " 
		<< "FD : " << it->second._fd << std::endl
		<< "Port : " << it->second._Port << std::endl
		<< "IP : " << it->second._IP << std::endl
		<< "byte Read : " << it->second.byteRead << std::endl;
		std::cout << "========================" << std::endl;
	}
}

void	printMultiMap(std::multimap<std::string, int> mmap)
{
	for (std::multimap<std::string, int>::iterator it = mmap.begin(); it != mmap.end(); it++)
	{
		std::cout << "IP : " << it->first << " : PORT : " << it->second << std::endl;
	}
}

void	debug()
{
	std::cout << RED << "********* DEBUG **************** " << END_CLR << std::endl;
}

void	log(std::string forWhat, std::string msg)
{
	std::cout << GREEN << "|----| " << forWhat << " :  " 
	<< msg << "|----| " << END_CLR << std::endl;
}