#include "../../include/WebServer.hpp"

Server::Server (std::vector<int> ports, std::vector<std::string> name, std::vector<std::string> host)
{
	_listenPorts = ports;
	_serverName = name;
	_serverHost = host;
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

Server::~Server ()
{
	_listenPorts.clear();
	_serverName.clear();
	_serverHost.clear();
}

std::vector<int>			Server::getListenPorts(void) const { return _listenPorts; }
std::vector<std::string>	Server::getServerHost(void) const { return _serverHost; }
std::vector<std::string>	Server::getServerName(void) const { return _serverName; }

//	***	---		________	---	***	//

WebServ::WebServ() {}
WebServ::~WebServ() { servers.clear(); }