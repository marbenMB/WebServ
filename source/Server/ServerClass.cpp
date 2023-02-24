#include "../../include/serverSide.hpp"

Server::Server (int id, std::vector<std::string> portIp, std::vector<std::string> name)
{
	_id = id;
	_combIpPort = portIp;
	_serverName = name;
}

Server::~Server ()
{
	_combIpPort.clear();
	_serverName.clear();
	_socketFD.clear();
}

int							Server::getID(void) const { return _id; }
std::vector<std::string>	Server::getCombIpPort(void) const { return _combIpPort; }
std::vector<std::string>	Server::getServerName(void) const { return _serverName; }

void	Server::setSocket(std::vector<int> sock) { _socketFD = deepCopyVector(sock); }

//	***	---		________	---	***	//

WebServ::WebServ() {}
WebServ::~WebServ() { servers.clear(); }