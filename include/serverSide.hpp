
#ifndef	SERV_SIDE_H
#define SERV_SIDE_H

#include "WebServer.hpp"
#include <poll.h>

///////////////////// MACROS ////////////////////
#define	EXIT_FAILURE 1
#define	DUP_PORT	2

#define DEFAULT_IP "0.0.0.0"
#define	DEFAULT_PORT 80

#define	MAXREAD	1024
///////////////////// ***** ////////////////////

//	****	*********	****	//
//	****	 DEFINES	****	//
//	****	*********	****	//

enum defines 
{
	//	-- Socket Type
	SERVER_SOCK = 1,
	CLIENT_SOCK,

	//	-- Request 
	KEEP_ALIVE = 7,
	CLOSE,

	//	-- Socket readiness to respond
	READY = 9,
	N_READY
};

//	****	*********	****	//
//	****	 CLASSES	****	//
//	****	*********	****	//

class	Server;

class	SockProp
{
	public :
		int				_fd;
		int				_Port;
		std::string		_IP;
		int				_type;
		struct pollfd	_pSFD;

		bool	operator< (const SockProp &other) const
		{
			if (_fd < other._fd)
				return true;
			return false;
		}
	
		SockProp (int fd, int port, std::string ip, int type);
		~SockProp ();
};

class	ClientSock : public SockProp
{
	public :
		// std::string		_host;
		std::vector<Server>	vecServ;
		ServerConf		_serverResponding;
		
		bool			_InitialRead;
		bool			_chunkedBody;
		int				_connexion;

		std::string		_tmp;
		std::string		_request;
		std::string		_reqHeader;
		std::string		_reqBody;
		long long		byteToRead;
		long long		byteRead;
		long long		_content_lenght;
		// size_t			byteLeft;
		bool			_readiness;

		// std::string		_response;
		// size_t			byteToSend;
		// size_t			byteSent;
		// size_t			byteLeft;

	public :
		// std::string		getRequest (void) const;

		ClientSock ();
		ClientSock (int fd, int port, std::string ip);
		~ClientSock ();

		void	separateHeadBody(std::string tmp);
		void	transferEncoding(void);
		void	formRequest(void);
		void	sockConnection(void);
		void	hostResp(void);
		void	readBody(void);
};

class	Server
{
	public	:
		Server (int id, ServerConf server, std::vector<std::string> portIp, std::vector<std::string> name);
		~Server ();

		int							getID			(void) const;
		std::vector<std::string>	getCombIpPort	(void) const;
		std::vector<std::string>	getServerName	(void) const;
		std::multimap<std::string, int>	getIpPort	(void) const;

		void						setIpPort	(std::multimap<std::string, int> listen);

	private	:
		int							_id;
		std::vector<std::string>	_combIpPort;
		std::vector<std::string>	_serverName;
		ServerConf					_Server;
		std::multimap<std::string, int>	_IpPort;

		// std::vector<int>			_socketFD;
};

class	WebServ
{
	public :
		std::vector<Server>			servers;
		std::vector<struct pollfd>	vecPoll;

		std::map<SockProp, std::vector<Server> >	serverSockets;
		std::map<int, ClientSock>	clientMap;

		int					servNums;
		size_t				nSocketServer;
		// std::vector<struct pollFd>	fds;
		
		WebServ ();
		~WebServ ();
		const SockProp&	findServSocket(int sockFd);
};

//	****	*********	****	//
//	****	TEMPLATES	****	//
//	****	*********	****	//

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


	//	+++	 Utils	+++	//
std::vector<std::string>	getStringKeyVal(std::map<std::string, std::vector<std::string> > myMap, std::string key);

	//	+++	Checkint functions +++	//
int	checkDuplicatePort(std::multimap<std::string, int> Map);
bool	checkDefaultServer(std::map<SockProp, std::vector<Server> > &servMap,std::string ip, int port, Server serv);

	//	+++	Server Utils	+++	//
std::multimap<std::string, int>	extractionIpPort(std::vector<std::string> combIpPort);
WebServ						establishServers(Data &g_data);
void						createSockets(WebServ &serv);
void						acceptClients(WebServ &serv);

#endif