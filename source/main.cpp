#include "../include/WebServer.hpp"

void	printWebServ (WebServ *myServ)
{
	for (std::vector<Server>::iterator it = myServ->servers.begin(); it != myServ->servers.end(); it++)
	{
		std::cout << "Server id : " << it->getID() << std::endl;
		printVector(it->getListenPorts(), "Server Ports : ");
		printVector(it->getServerHost(), "Server Hosts : ");
		printVector(it->getServerName(), "Server Names : ");
		std::cout << std::endl;
	}
}

int main (int ac, char **av)
{
    Data g_Data;
    if (ac == 2)
    {
        g_Data.configuration.parcing_file(av[1], g_Data);
		
		//	***	Creating WebServer :
		if (g_Data.error.empty())
		{
			WebServ	*myServ;
			
			myServ = establishServers(g_Data);
			// printWebServ(myServ);
			// printWebServ(myServ);
			createSockets(myServ);

			delete	myServ;
		}
    }
    else
        g_Data.error = "error n number of argument";
    if (!g_Data.error.empty())
    {
        std::cout << RED << g_Data.error << END_CLR << std::endl;
        // system("leaks webserv");
        return (1);
    }
	std::cout << GREEN << "+> Parsing Done!!" << END_CLR << std::endl << std::endl;
    // system("leaks webserv");
    return (0);
}
