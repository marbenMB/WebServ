#include "../include/serverSide.hpp"
#include "../include/unitTests.hpp"

int _main (int ac, char **av)
{
    Data 	g_Data;
    if (ac == 2)
        g_Data.configuration.parcing_file(av[1], g_Data);
<<<<<<< HEAD
		
		//	***	Creating WebServer :
		if (g_Data.error.empty())
		{
			WebServ	myServ;
			
			try {
				myServ = establishServers(g_Data);

				// std::cout << "--> Num of Servers : " << myServ.servNums << std::endl << "\n";
				// printWebServ(myServ);
				
				createSockets(myServ);
				// printSockProp(myServ.serverSockets);
				std::cout << GREEN << "+> Socket Created !!" << END_CLR << std::endl;

				acceptClients(myServ);
			} catch (std::exception &e) {
				std::cout << RED << "+> " << e.what() << END_CLR << std::endl;
			}
		}
    }
=======
    else if (ac == 1)
        g_Data.configuration.parcing_file("./conf/def_configfile.conf", g_Data);
>>>>>>> d9ffec7723292c83c629384aea2901c5976cfa2b
    else
        g_Data.error = "error in number of argument";
    if (!g_Data.error.empty())
    {
        std::cout << RED << g_Data.error << END_CLR << std::endl;
        // system("leaks webserv");
        return (1);
    }
	std::cout << GREEN << "+> Parsing Done !!" << END_CLR << std::endl << std::endl;

    //	***	Creating WebServer :
    WebServ	myServ;
    
    try {
        myServ = establishServers(g_Data);

        // std::cout << "--> Num of Servers : " << myServ.servNums << std::endl << "\n";
        // printWebServ(myServ);
        
        createSockets(myServ);
        // printSockProp(myServ.serverSockets);
        std::cout << GREEN << "+> Socket Created !!" << END_CLR << std::endl;

        acceptClients(myServ);
    } catch (std::exception &e) {
        std::cout << RED << "+> " << e.what() << END_CLR << std::endl;
    }

    // system("leaks webserv");
    return (0);
}

int main(int ac, char **av)
{
	_main(ac, av);
    // system("leaks webserv");
}