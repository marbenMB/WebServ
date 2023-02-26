#include "../include/header.hpp"

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

int _main (int ac, char **av)
{
    Data g_Data;
    if (ac == 2)
    {
        g_Data.configuration.parcing_file(av[1], g_Data);
		
		//	***	Creating WebServer :
		if (g_Data.error.empty())
		{
			WebServ	*myServ;
			
			try {
				myServ = establishServers(g_Data);

				printWebServ(myServ);
				// createSockets(myServ);
				delete	myServ;
			} catch (std::exception &e) {
				std::cout << e.what() << std::endl;
			}

		}
    }
    else
        g_Data.error = "error in number of argument";
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

int main(int ac, char **av)
{
	_main(ac, av);
    system("leaks webserv");
}