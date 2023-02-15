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
		Server (int id, std::vector<int> ports, std::vector<std::string> name, std::vector<std::string> host);
		~Server ();

		int							getID			(void) const;
		std::vector<int>			getListenPorts	(void) const;
		std::vector<std::string>	getServerName	(void) const;
		std::vector<std::string>	getServerHost	(void) const;
		std::vector<int>			getSocketFD		(void) const;

		void						setSocket (std::vector<int> sock);

	private	:
		int							_id;
		std::vector<int>			_listenPorts;
		std::vector<std::string>	_serverName;
		std::vector<std::string>	_serverHost;
		std::vector<int>			_socketFD;
};

class	WebServ
{
	public :
		std::vector<Server>	servers;
		int					servNums;
		
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

template <typename T>
std::vector<T>	deepCopyVector(std::vector<T> vec)
{
	std::vector<T>	newVec;
	typename std::vector<T>::iterator	it;

	it = vec.begin();
	while (it != vec.end())
	{
		newVec.push_back(*it);
		it++;
	}
	return newVec;
}

//	****	*********	****	//
//	****	FUNCTIONS	****	//
//	****	*********	****	//


	//	+++	Server Utils	+++	//
std::vector<int>			portsExtraction(ServerConf server);
std::vector<std::string>	getStringKeyVal(ServerConf server, std::string key);
WebServ	*establishServers(Data &g_data);

#endif