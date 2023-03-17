#include "../../include/serverSide.hpp"
#include "../../include/unitTests.hpp"
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

void	WebServ::closeClientConn(std::vector<struct pollfd>::iterator socket)
{
	close(socket->fd);
	clientMap.erase(socket->fd);
	vecPoll.erase(socket);
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
ServerConf					Server::getServconf(void) const { return _Server; }

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
	_InitialRead = true;
	_readiness = false;
	_chunkedBody = false;
	_done = false;
	_connexion = SET_CNX;
	_reqStat = STAT;
	byteRead = 0;
	byteToRead = 0;
	byteToSend = 0;
	byteSent = 0;
	_content_lenght = 0;
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
		if (nFind + 4 < tmp.length())
			_reqBody = tmp.substr(nFind + 4, tmp.length());
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
	if (_chunkedBody)
	{
		// add content length to request headers
		_request.append(_bodyChunk);
	}
	else
		_request.append(_reqBody);
}

std::string	ClientSock::findHeaderValue(std::string header)
{
	size_t		nFind_1;
	size_t		nFind_2;
	std::string	findWord;
	std::string	word;

	nFind_1 = _reqHeader.find(header);
	if (nFind_1 != std::string::npos)
	{
		findWord = _reqHeader.substr(nFind_1 + header.length(), _reqHeader.length());
		nFind_2 = findWord.find("\r\n");
		if (nFind_2 != std::string::npos)
			word = _reqHeader.substr(nFind_1 + header.length(), nFind_2);
	}
	return word;
}

void	ClientSock::sockConnection(void)
{
	std::string	conn;

	conn = this->findHeaderValue("Connection: ");
	if (!conn.compare("keep-alive"))
		_connexion = KEEP_ALIVE;
	else if	(!conn.compare("close"))
		_connexion = CLOSE;
	else
		_connexion = KEEP_ALIVE;
}

void	ClientSock::hostResp(void)
{
	//	-- getting the host name form the request headers
	_host = this->findHeaderValue("Host: ");
	
	//	--	finding server
	bool	match = false;
	std::vector<std::string>	names;
	std::vector<Server>::iterator servIt = vecServ.begin();

	while (servIt != vecServ.end())
	{
		names = servIt->getServerName();
		for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
		{
			if (!_host.compare(*it))
			{
				match = true;
				break;
			}
		}
		if (match)
			break;
		servIt++;
	}

	//	-- storing server config for the client
	if (match)
		_serverResponding = servIt->getServconf();
	else
		_serverResponding = vecServ.begin()->getServconf();
}

void	ClientSock::readChunkBody(void)
{
	std::stringstream	bodyStream;
	std::string			hexStr;
	long long			byte;

	std::cout << MAUVE << _reqBody << RED "++END BODY++" END_CLR << std::endl;

	bodyStream << _reqBody;

	std::getline(bodyStream, hexStr);
	hexStr.append("\n");
	
	byte = std::stoi(hexStr, nullptr, 16);

	std::cout << "- STR : " << hexStr
			<<	"- bytes : " << byte << std::endl;

	

	bodyStream << "";
	bodyStream.clear();

}

//	--- Response part

void	ClientSock::formResponse(void)
{
	//	**************************************************************************

	//!	Request stat if is it WELL || TIMEOUT
	_reqStat = WELL;
	//*	generate Request
	request *req = new request(_fd ,&_serverResponding, _request, _response);

	delete req;
	//	**************************************************************************

	byteToSend = _response.length();
	byteSent = 0;
	_done = false;
}

void	ClientSock::reFormResponse(int sent)
{
	std::string	tmp = std::string(_response);

	_response.clear();
	_response = tmp.substr(sent, tmp.length() - sent);
	if (_response.empty() && byteSent >= byteToSend)
		_done = true;
}

void	ClientSock::resetClientProp(void)
{
	_host.clear();

	_InitialRead = true;
	_readiness = false;
	_chunkedBody = false;
	_connexion = SET_CNX;
	_reqStat = STAT;

//	-- Request
	_tmp.clear();
	_reqHeader.clear();
	_reqBody.clear();
	byteRead = 0;
	byteToRead = 0;
	_request.clear();

//	-- Chunked body :
	_bodyChunk.clear();
	_content_lenght = 0;

//	--	Response :
	_response.clear();
	byteToSend = 0;
	byteSent = 0;
	_done = false;
}
