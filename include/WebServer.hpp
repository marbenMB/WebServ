#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#define BLUE "\x1b[36m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define MAUVE "\x1b[33m"
#define END_CLR "\033[0m"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <stack>
#include <netdb.h>
#include <cstring>
#include <sys/stat.h>
#include "classes.hpp"
#include "ConfigFile.hpp"

///////////////////// Parent class ////////////////////

class Data {
    public:
        Data() {};
        std::vector<ServerConf> server_list;
        ConfigFile configuration;
        std::string error;
};

class	Server
{
	public	:
		Server (std::vector<int> ports, std::vector<std::string> name, std::vector<std::string> host);
		~Server ();

		std::vector<int>			getListenPorts	(void) const;
		std::vector<std::string>	getServerName	(void) const;
		std::vector<std::string>	getServerHost	(void) const;

	private	:
		std::vector<int>			_listenPorts;
		std::vector<std::string>	_serverName;
		std::vector<std::string>	_serverHost;
};

class	WebServ
{
	public :
		std::vector<Server>	servers;
		
		WebServ ();
		~WebServ ();
};

//	****	*********	****	//
//	****	TEMPLATES	****	//
//	****	*********	****	//

template <typename T>
void	printVector(std::vector<T> vec, std::string forWhat)
{
	typename std::vector<T>::iterator	it;

	it = vec.begin();
	std::cout << forWhat;
	while (it != vec.end())
	{
		std::cout << *it << " ";
		it++;
	}
	std::cout << std::endl;
}

//	****	*********	****	//
//	****	FUNCTIONS	****	//
//	****	*********	****	//


	//	+++	Server Utils	+++	//
std::vector<int>			portsExtraction(ServerConf server);
std::vector<std::string>	getStringKeyVal(ServerConf server, std::string key);
WebServ	*estabilishServers(Data &g_data);

#endif