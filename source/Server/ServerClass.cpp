#include "../../include/serverSide.hpp"
//	***	---		________	---	***	//

WebServ::WebServ() {}
WebServ::~WebServ() 
{ 
	servers.clear();
	vecPoll.clear();
	serverSockets.clear();
}

const SockProp&	WebServ::findServSocket(int sockFd)
{
	std::map<SockProp, std::vector<Server> >::iterator it = serverSockets.begin();

	while ( it != serverSockets.end())
	{
		if (it->first._fd == sockFd)
			break;
		it++;
	}
	return ((*it).first);
}

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
	_IpPort.clear();
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
	_pSFD.revents = 0;
}
SockProp::~SockProp() { _IP.clear(); }

//	***	---		________	---	***	//

ClientSock::ClientSock () :  SockProp(0, 0, "", CLIENT_SOCK) {};

ClientSock::ClientSock (int fd, int port, std::string ip) : SockProp(fd, port, ip, CLIENT_SOCK) 
{
	byteRead = 0;
	byteToRead = 0;
	_InitialRead = true;
	_readiness = false;
}

ClientSock::~ClientSock () {};

// std::string	ClientSock::getRequest (void) const { return _request; }

void	ClientSock::separateHeadBody(std::string tmp)
{
	size_t		nFind;
	
	nFind = tmp.find("\r\n\r\n");
	if (nFind != std::string::npos)
	{
		_reqHeader = tmp.substr(0, nFind + 4);
		if (nFind + 5 < tmp.length())
			_reqBody = tmp.substr(nFind + 5, tmp.length());
		_tmp.clear();
	}
	else
	{
		_InitialRead = true;
		_readiness = false;
	}
}

void	ClientSock::transferEncoding(void)
{
	size_t		nFind;
	std::string	tmp;
	std::stringstream ss;
	
	nFind = _reqHeader.find("Transfer-Encoding: chunked");
	if (nFind != std::string::npos)
		_chunkedBody = true;
	else
	{
		_chunkedBody = false;
		nFind = _reqHeader.find("Content-Length: ");
		if (nFind != std::string::npos)
		{
			tmp = _reqHeader.substr(nFind + 16, _reqHeader.length() - (nFind + 16));
			ss << tmp;
			ss >> byteToRead;
			_content_lenght = byteToRead;
			byteToRead += _reqHeader.length();
		}
		else if (nFind == std::string::npos && _InitialRead == false)
			byteToRead = -1;
	}
}

void	ClientSock::formRequest(void)
{
	_request.append(_reqHeader);
	_request.append(_reqBody);
}

void	ClientSock::sockConnection(void)
{
	size_t		nFind_1;
	size_t		nFind_2;
	std::string	findConn;
	std::string	conn;

	nFind_1 = _reqHeader.find("Connection: ");
	if (nFind_1 != std::string::npos)
	{
		findConn = _reqHeader.substr(nFind_1 + 12, _reqHeader.length());
		nFind_2 = findConn.find("\r\n");
		if (nFind_2 != std::string::npos)
			conn = _reqHeader.substr(nFind_1 + 12, nFind_2);
	}
	if (!conn.compare("keep-alive"))
		_connexion = KEEP_ALIVE;
	else if	(!conn.compare("close"))
		_connexion = CLOSE;
}

void	ClientSock::readBody(void)
{
	std::cout << std::endl;
}
