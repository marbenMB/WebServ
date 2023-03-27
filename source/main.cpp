#include "../include/serverSide.hpp"
#include "../include/unitTests.hpp"
#include "../include/Assets.hpp"
#include "../include/ft_cgi.hpp"

std::map<std::string, std::string> session_map;

int _main (int ac, char **av)
{
    Data 	g_Data;
    if (ac == 2)
        g_Data.configuration.parcing_file(av[1], g_Data);
    else if (ac == 1)
        g_Data.configuration.parcing_file("./conf/def_configfile.conf", g_Data);
    else
        g_Data.error = "error in number of argument";
    if (!g_Data.error.empty())
    {
        std::cout << RED << g_Data.error << END_CLR << std::endl;
        // system("leaks webserv");
        return (1);
    }
	std::cout << GREEN << "+> Parsing Done !!" << END_CLR << std::endl << std::endl;
    list_session_page();

    Assets  _assets;
    //	***	Creating WebServer :
    WebServ	myServ;
    
    try {
        myServ = establishServers(g_Data);

        // std::cout << "--> Num of Servers : " << myServ.servNums << std::endl << "\n";
        // printWebServ(myServ);
        
        createSockets(myServ);
        // printSockProp(myServ.serverSockets);
        std::cout << GREEN << "+> Socket Created !!" << END_CLR << std::endl;

		//	Log IP:PORTS listen
        logListen(myServ);

        acceptClients(myServ);
    } catch (std::exception &e) {
        std::cout << RED << "+> " << e.what() << END_CLR << std::endl;
    }

    // system("leaks webserv");
    return (0);
}

void s3ediya() {
    system("leaks webServ");
}

int main(int ac, char **av)
{
	_main(ac, av);
    // atexit(s3ediya);
}