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