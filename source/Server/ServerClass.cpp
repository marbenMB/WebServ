#include "../../include/WebServer.hpp"

Server::Server (int id, std::vector<int> ports, std::vector<std::string> name, std::vector<std::string> host)
{
	_id = id;
	_listenPorts = ports;
	_serverName = name;
	_serverHost = host;
}

Server::~Server ()
{
	_listenPorts.clear();
	_serverName.clear();
	_serverHost.clear();
	_socketFD.clear();
}

int							Server::getID(void) const { return _id; }
std::vector<int>			Server::getListenPorts(void) const { return _listenPorts; }
std::vector<std::string>	Server::getServerHost(void) const { return _serverHost; }
std::vector<std::string>	Server::getServerName(void) const { return _serverName; }

//	***	---		________	---	***	//

WebServ::WebServ() {}
WebServ::~WebServ() { servers.clear(); }