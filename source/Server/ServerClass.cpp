#include "../../include/WebServer.hpp"

Server::Server (int id, std::vector<int> ports, std::vector<std::string> name)
{
	_id = id;
	_listenPorts = ports;
	_serverName = name;
}

Server::~Server ()
{
	_listenPorts.clear();
	_serverName.clear();
	_socketFD.clear();
}

int							Server::getID(void) const { return _id; }
std::vector<int>			Server::getListenPorts(void) const { return _listenPorts; }
std::vector<std::string>	Server::getServerName(void) const { return _serverName; }

void	Server::setSocket(std::vector<int> sock) { _socketFD = deepCopyVector(sock); }

//	***	---		________	---	***	//

WebServ::WebServ() {}
WebServ::~WebServ() { servers.clear(); }