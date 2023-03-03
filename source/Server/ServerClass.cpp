#include "../../include/serverSide.hpp"
//	***	---		________	---	***	//

WebServ::WebServ() {}
WebServ::~WebServ() { servers.clear(); }

//	***	---		________	---	***	//

Server::Server (int id, ServerConf server, std::vector<std::string> portIp, std::vector<std::string> name)
{
	_id = id;
	_Server = server;
	_combIpPort = portIp;
	_serverName = name;
}

Server::~Server ()
{
	_combIpPort.clear();
	_serverName.clear();
	_serverName.clear();
}

int							Server::getID(void) const { return _id; }
std::vector<std::string>	Server::getCombIpPort(void) const { return _combIpPort; }
std::vector<std::string>	Server::getServerName(void) const { return _serverName; }
std::multimap<std::string, int>	Server::getIpPort(void) const { return _IpPort; }

void						Server::setIpPort (std::multimap<std::string, int> listen)
{
	_IpPort = listen;
}
//	***	---		________	---	***	//

SockProp::SockProp(int fd, int port, std::string ip, int type) : _fd(fd), _Port(port), _IP(ip), _type(type)
{
	_pSFD.fd = _fd;
	_pSFD.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
}
SockProp::~SockProp() {}

//	***	---		________	---	***	//

// ClientSock::ClientSock (int fd, int port, std::string ip) : SockProp(fd, port, ip, CLIENT) {}
// ClientSock::~ClientSock () {};

// std::string	ClientSock::getRequest (void) const { return _request; }
