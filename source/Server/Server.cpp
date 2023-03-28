#include "../../include/serverSide.hpp"
#include "../../include/unitTests.hpp"

std::multimap<std::string, int>	extractionIpPort(std::vector<std::string> combIpPort)
{
	//	...
	//	Loop IP PORT combination and insert it as string-int (ip-Port) pairs .
	//	if PORT is not specified then 80 is default
	//	if IP is not specified then 0.0.0.0 is default
	std::string	ip;
	int			port;
	std::multimap<std::string, int>	listen;
	size_t	nFind;

	for (std::vector<std::string>::iterator it = combIpPort.begin(); it != combIpPort.end(); it++)
	{
		nFind = it->find(':');
		if (nFind != std::string::npos)
		{
			ip = it->substr(0, nFind);
			port = std::atoi(it->substr((nFind + 1), it->length()).c_str());
		}
		else
		{
			nFind = it->find('.');
			if (nFind != std::string::npos)
			{
				ip = std::string(*it);
				port = DEFAULT_PORT;
			}
			else
			{
				port = std::atoi(it->substr((nFind + 1), it->length()).c_str());
				ip = std::string(DEFAULT_IP);
			}
		}
		listen.insert(std::make_pair(ip, port));
	}
	return listen;
}

WebServ	establishServers(Data &g_data)
{
	WebServ 					serv;
	std::vector<std::string>	servName;
	int							id = 1;

	//	...
	//	Creating Severs
	//	puch them to WebServ vector.
	for (Vect_Serv::iterator itServ = g_data.server_list.begin(); itServ != g_data.server_list.end(); itServ++)
	{
		Server newServ = Server(id, *itServ, getStringKeyVal(itServ->server_data, "listen"), getStringKeyVal(itServ->server_data, "server_name"));

		newServ.setIpPort(extractionIpPort(newServ.getCombIpPort()));
		if (checkDuplicatePort(newServ.getIpPort()))
		{
			throw	std::invalid_argument("Duplicated Port !!");
		}

		id++;
		serv.servers.push_back(newServ);
	}
	serv.servNums = id - 1;
	serv.nSocketServer = 0;
	return serv;
}

void	createSockets(WebServ &serv)
{
	std::multimap<std::string, int>	mmap;
	
	int		sockFd;

	struct sockaddr_in	addr;

	bool	def;

	//	...
	//	Loop to each Server and Create socket
	for (std::vector<Server>::iterator servIt = serv.servers.begin(); servIt != serv.servers.end(); servIt++)
	{
		mmap = servIt->getIpPort();
		for (std::multimap<std::string, int>::iterator it = mmap.begin(); it != mmap.end(); it++)
		{
			//?:	No default server is defined for the current IP:PORT (it->first:it->second) combination
			def = true;
			
			//?:	Socket creation and setting reused addr option.
			sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sockFd < 0)
				throw	std::runtime_error("Socket() Failed !!");
			if (setOptionSocket(sockFd))
				throw	std::runtime_error("Setsockopt() Failed !!");

			//?:	Binding the socket with Ip and Port
			bzero(&addr, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(it->second);
			addr.sin_addr.s_addr = inet_addr(it->first.c_str());
			if (bind(sockFd, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
			{
				//?:	Checking if the there is a default Server listenning to same IP:PORT
				if ((def = checkDefaultServer(serv.serverSockets, it->first, it->second, *servIt)))
					throw	std::runtime_error("Bind() Failed !!");
				close(sockFd);
			}

			if (def)
			{
				//?:	Listening to socket
				if (listen(sockFd, SOMAXCONN))
					throw	std::runtime_error("Listen() Failed !!");
				
				//?:	Create sockPorp for each socket and insert it to the map
				SockProp	prop(sockFd, it->second, it->first, SERVER_SOCK);
				
				//?:	Push socket to poll vector
				serv.vecPoll.push_back(prop._pSFD);

				//?:	Insert sockProp and vector of Server in the serverSockets map
				serv.serverSockets[prop];
				serv.serverSockets[prop].push_back(*servIt);

				//?:	Incrementing number of sockets
				serv.nSocketServer++;
			}
		}
	}
}

void	acceptClients(WebServ &serv)
{
	//?:	Poll var
	int		nFd;

	//?:	Accept var
	int					clientFD;
	struct sockaddr_in	clientAddr;
	int					clientLen = sizeof(clientAddr);

	//?:	Response Var
	int		toSend;
	int		sent;

	//?:	sockets
	std::vector<struct pollfd>::iterator socket;

	while (true)
	{
		nFd = poll(&serv.vecPoll[0], serv.vecPoll.size(), -1);
		if (nFd < 0)
		{
			// continue;
			throw	std::runtime_error("Poll() Failed !!");
		}

		//?:	Looping waiting for an event in Server socket
		for (size_t idx = 0; idx < serv.vecPoll.size(); idx++)
		{
			if (idx < serv.nSocketServer)
			{
				if (serv.vecPoll[idx].revents & POLLIN)
				{
					//?:	Accepting new Client
					clientFD = accept(serv.vecPoll[idx].fd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLen);
					if (clientFD < 0)
						throw	std::runtime_error("Accept() Failed !!");
					if (setOptionSocket(clientFD))
						throw	std::runtime_error("Setsockopt() Failed !!");

					//?:	Create new Client prop object
					const SockProp&	serverSock = serv.findServSocket(serv.vecPoll[idx].fd);
					ClientSock	sockClient(clientFD, serverSock._Port, serverSock._IP);

					//?:	storing servers Vector for the new Client
					std::vector<Server>&	vecServ = serv.serverSockets[serverSock];
					sockClient.vecServ = vecServ;

					//?:	push client to poll vector
					serv.vecPoll.push_back(sockClient._pSFD);

					//?:	insert client to clientMap
					serv.clientMap.insert(std::make_pair(clientFD, sockClient));
				}
			}
			else
			{
				socket = serv.vecPoll.begin() + idx;
				
				//?:	check TimeOut Request.
				if (serv.clientMap[serv.vecPoll[idx].fd].timeOutRequest())
				{
					serv.vecPoll[idx].revents = POLLOUT;
					serv.clientMap[serv.vecPoll[idx].fd]._connexion = CLOSE;
					serv.clientMap[serv.vecPoll[idx].fd]._reqStat = TIMEOUT;
				}

				//?:	check the POLLIN event in the client socket.
				if (serv.vecPoll[idx].revents & POLLIN)
				{
					//?:	Reading Client Request if prob in recv() erase client & continue.
					if (serv.readRequest(socket))
						continue ;
				}

				//?:	checking socket Readiness to POLLOUT.
				serv.socketReadiness(socket);

				//?:	check the POLLOUT event in the client socket.
				if (serv.vecPoll[idx].revents & POLLOUT && serv.clientMap[serv.vecPoll[idx].fd]._readiness)
				{
					if (MAXSEND <= serv.clientMap[serv.vecPoll[idx].fd]._response.length())
						toSend = MAXSEND;
					else
						toSend = serv.clientMap[serv.vecPoll[idx].fd]._response.length();
					sent = send(serv.vecPoll[idx].fd, serv.clientMap[serv.vecPoll[idx].fd]._response.c_str(), toSend, 0);
					serv.clientMap[serv.vecPoll[idx].fd].byteSent += sent;
					serv.clientMap[serv.vecPoll[idx].fd].reFormResponse(sent);

					//?:	The end of Response send.
					if (serv.clientMap[serv.vecPoll[idx].fd]._done)
					{
						// ********* reset client data first **************
						if (serv.clientMap[serv.vecPoll[idx].fd]._connexion == KEEP_ALIVE)
							serv.clientMap[serv.vecPoll[idx].fd].resetClientProp();
						else if (serv.clientMap[serv.vecPoll[idx].fd]._connexion == CLOSE)
						{
							serv.closeClientConn(socket);
							continue;
						}
					}
				}

				//?:	check the POLLIN event in the client socket.
				if (serv.vecPoll[idx].revents & POLLERR || serv.vecPoll[idx].revents & POLLHUP)
					serv.closeClientConn(socket);
			}
		}
	}
}