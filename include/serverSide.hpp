
#ifndef	SERV_SIDE_H
#define SERV_SIDE_H

#include "WebServer.hpp"
#include "request.hpp"
#include "method.hpp"
#include <poll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>


///////////////////// MACROS ////////////////////
#define	EXIT_FAILURE 1
#define	DUP_PORT	2

#define DEFAULT_IP "0.0.0.0"
#define	DEFAULT_PORT 8080

#define	MAXREAD	500000
#define	MAXSEND	500000

///////////////////// ***** ////////////////////

//	****	*********	****	//
//	****	 DEFINES	****	//
//	****	*********	****	//

enum ServerDef 
{
	//	** Socket Type
	SERVER_SOCK = 1,
	CLIENT_SOCK,

	//	** Request 
	KEEP_ALIVE = 7,
	CLOSE,
	SET_CNX, //? connexion not set

	//	** Request stat
	STAT = 199,
	WELL = 200,
	TIMEOUT,	//?: Request TimeOut

	//	**
	CRF = 4,
	TIMELIMIT = 5000	//?: 5 sec
};

//	****	*********	****	//
//	****	 CLASSES	****	//
//	****	*********	****	//

class	Server;

class	SockProp
{
	public :
		int				_fd;	//?:	socket FD.
		int				_Port;	//?:	Port bind to socket.
		std::string		_IP;	//?:	IP bind to socket.
		int				_type;	//?:	SERVER or CLIENT
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
		std::string		_host;				//?:	server name came in request Host header.
		std::vector<Server>	vecServ;		//?:	Vector of servers that may handle request came in this socket.
		ServerConf		_serverResponding;	//?:	Configuration of the server responsible of this client request.
		
		bool			_InitialRead;		//?:	true -> still initial read available.
		bool			_chunkedBody;		//?:	true -> body is chunked.
		bool			_readiness;			//?:	true -> socket is in write mode , send response.
		int				_connexion;			//?:	CLOSE -> connexion with client will be closed.
		int				_reqStat;			//?:	TIMEOUT -> request is on timeout.
		bool			_endChunk;			//?:	true -> if read of chunk body is done.

		std::string		_tmp;				//?:	where request is sotred until headers are separated than body.
		std::string		_reqHeader;			//?:	- After separation -> request Headers.
		std::string		_reqBody;			//?:	- After separation -> request body.
		long long		byteToRead;			//?:	-1 -> request with no body
		long long		byteRead;			//?:	bytes already read by recv().
		size_t			_nFind;				//?:	- chunked case -> Where find stopped until next chunked body read.
		long long		_skipedByte;		//?:	- chunked case -> Number of bytes skipped.
		std::string		_bodyChunk;			//?:	- chunked case -> whole body without chunked encoding delimiter.
		long long		_content_lenght;	//?:	- chunked case -> content_lenght of processed chunked body.
		std::string		_request;			//?:	- request formed to be processed by request-response process.
		long long		_lastRead;			//?:	- TimeOut case -> the last time read from socket.


		bool			_done;				//?:	true -> response send completely.
		std::string		_response;			//?:	response ready to be sent to client.
		long long		byteToSend;			//?:	size of response to be sent.
		long long		byteSent;			//?:	bytes already sent by send().


	public :
		ClientSock ();
		ClientSock (int fd, int port, std::string ip);
		~ClientSock ();

		//??: Request handling
		bool	timeOutRequest(void);
		void	separateHeadBody(std::string tmp);
		std::string	findHeaderValue(std::string header);
		void	transferEncoding(void);
		void	sockConnection(void);
		void	hostResp(void);
		void	readChunkBody(void);
		bool	checkSockReady(void);
		void	formRequest(void);

		//??: Response handling
		void	formResponse(void);
		void	reFormResponse(int sent);
		void	resetClientProp(void);
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
		ServerConf					getServconf		(void) const;

		void						setIpPort	(std::multimap<std::string, int> listen);

	private	:
		int							_id;			//?:	server ID.
		std::vector<std::string>	_combIpPort;	//?:	vector of string IP:PORT comb (0.0.0.0:80).
		std::vector<std::string>	_serverName;	//?:	vector of string server_name (example.com).
		ServerConf					_Server;		//?:	server configuration.
		std::multimap<std::string, int>	_IpPort;	//?:	multimap of IP:string -> key and PORT:int -> value.
};

class	WebServ
{
	public :
		std::vector<Server>			servers;						//?:	Vector of all servers.
		std::vector<struct pollfd>	vecPoll;						//?:	vector of poll struct.

		std::map<SockProp, std::vector<Server> >	serverSockets;	//?:	Servers map each server sockProp has vector of servers that may handle request came in it.
		std::map<int, ClientSock>					clientMap;		//?:	each client socket FD has its properties mentionned up.

		int					servNums;								//?:	number of servers.
		size_t				nSocketServer;							//?:	number of server sockets.
		
		WebServ ();
		~WebServ ();

		const SockProp&	findServSocket(int sockFd);
		void	closeClientConn(std::vector<struct pollfd>::iterator socket);
		int		readRequest(std::vector<struct pollfd>::iterator client);
		void	socketReadiness(std::vector<struct pollfd>::iterator client);
};

//	****	*********	****	//
//	****	FUNCTIONS	****	//
//	****	*********	****	//


	//	+++	 Utils	+++	//
std::vector<std::string>	getStringKeyVal(std::map<std::string, std::vector<std::string> > myMap, std::string key);
int							setOptionSocket(int fd);
long long					ft_gettime(void);

	//	+++	Checkint functions +++	//
int	checkDuplicatePort(std::multimap<std::string, int> Map);
bool	checkDefaultServer(std::map<SockProp, std::vector<Server> > &servMap,std::string ip, int port, Server serv);

	//	+++	Server Utils	+++	//
std::multimap<std::string, int>	extractionIpPort(std::vector<std::string> combIpPort);
WebServ						establishServers(Data &g_data);
void						createSockets(WebServ &serv);
void						logListen(WebServ serv);
void						acceptClients(WebServ &serv);

#endif