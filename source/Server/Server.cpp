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
			try
			{
				port = std::stoi(it->substr((nFind + 1), it->length()));
			}	catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
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
				try
				{
					port = std::stoi(it->substr((nFind + 1), it->length()));
				}	catch (std::exception &e)
				{
					std::cout << e.what() << std::endl;
				}
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
	int		optval = 1;

	struct addrinfo	hints, *res;
	std::stringstream		ss;
	std::string				strPort;

	struct sockaddr_in	addr;

	bool	def;

	//	...
	//	Loop to each Server and Create socket
	for (std::vector<Server>::iterator servIt = serv.servers.begin(); servIt != serv.servers.end(); servIt++)
	{
		mmap = servIt->getIpPort();
		for (std::multimap<std::string, int>::iterator it = mmap.begin(); it != mmap.end(); it++)
		{
			//	-- No default server is defined for the current IP:PORT (it->first:it->second) combination
			def = true;
			
			//	-- Socket creation and setting reused addr option.
			sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sockFd < 0)
				throw	std::runtime_error("Socket() Failed !!");
			if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
				throw	std::runtime_error("Setsockopt() Failed !!");
			fcntl(sockFd, F_SETFL,	O_NONBLOCK);
			
			//	-- Getting the addr info
			bzero(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_protocol = SOCK_STREAM;
			ss << it->second;
			ss >> strPort;
			ss << "";
			ss.clear();
			if (getaddrinfo(it->first.c_str(), strPort.c_str(), &hints, &res))
			{
				std::cerr << it->first << " : " << strPort << std::endl;
				throw	std::runtime_error("Address Not Available !!");
			}
			freeaddrinfo(res);

			//	-- Binding the socket with Ip and Port
			bzero(&addr, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(it->second);
			addr.sin_addr.s_addr = inet_addr(it->first.c_str());
			if (bind(sockFd, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
			{
				if ((def = checkDefaultServer(serv.serverSockets, it->first, it->second, *servIt)))
					throw	std::runtime_error("Bind() Failed !!");
				close(sockFd);
			}

			if (def)
			{
				//	--	Listening to socket
				if (listen(sockFd, 5))
					throw	std::runtime_error("Listen() Failed !!");
				
				//	-- Create sockPorp for each socket and insert it to the map
				SockProp	prop(sockFd, it->second, it->first, SERVER_SOCK);
				
				//	-- Push socket to poll vector
				serv.vecPoll.push_back(prop._pSFD);

				//	-- Insert sockProp and vector of Server in the serverSockets map
				serv.serverSockets[prop];
				serv.serverSockets[prop].push_back(*servIt);

				//	--	Incrementing number of sockets
				serv.nSocketServer++;
			}
		}
	}
}

void	acceptClients(WebServ &serv)
{
	//	--	Poll var
	int		nFd;

	//	--	Accept var
	// int					optval = 1;
	int					clientFD;
	struct sockaddr_in	clientAddr;
	int					clientLen = sizeof(clientAddr);

	//	--	Read var
	char	buffer[MAXREAD];
	int		byte;

	//	-- Response Var
	int		toSend;
	int		sent;
	// std::string	tmp;

	while (true)
	{
		nFd = poll(&serv.vecPoll[0], serv.vecPoll.size(), -1);
		if (nFd < 0)
		{
			// continue;
			throw	std::runtime_error("Poll() Failed !!");
		}
		for (size_t idx = 0; idx < serv.vecPoll.size(); idx++)
		{
			if (idx < serv.nSocketServer)
			{
				if (serv.vecPoll[idx].revents & POLLIN)
				{
					//	--	Accepting new Client
					clientFD = accept(serv.vecPoll[idx].fd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLen);
					if (clientFD < 0)
						throw	std::runtime_error("Accept() Failed !!");
					// if (setsockopt(clientFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
					// 	throw	std::runtime_error("Setsockopt() Failed !!");
					fcntl(clientFD, F_SETFL, O_NONBLOCK);

					//	--	Create new Client prop object
					const SockProp&	serverSock = serv.findServSocket(serv.vecPoll[idx].fd);
					ClientSock	sockClient(clientFD, serverSock._Port, serverSock._IP);

					//	-- storing servers Vector for the new Client
					std::vector<Server>&	vecServ = serv.serverSockets[serverSock];
					sockClient.vecServ = vecServ;

					//	--	push client to poll vector
					serv.vecPoll.push_back(sockClient._pSFD);

					//	--	insert client to clientMap
					serv.clientMap.insert(std::make_pair(clientFD, sockClient));
				}
			}
			else
			{
				bzero(&buffer, MAXREAD);

				//	--	check the POLLIN event in the client socket
				if (serv.vecPoll[idx].revents & POLLIN)
				{
					byte = recv(serv.vecPoll[idx].fd, &buffer, MAXREAD, 0);
					// std::cout << "======== RECV BYTE : " << byte << "======\n" << std::endl;
					//	--	check if recv returns <= 0 (error case -> erase client socket)
					if (byte <= 0)
					{
						// std::cout << "**** RECV PROB : ****\n" << std::endl;
						close(serv.vecPoll[idx].fd);
						std::vector<struct pollfd>::iterator it = serv.vecPoll.begin() + (idx);
						serv.clientMap.erase(it->fd);
						serv.vecPoll.erase(it);
						// idx--;
						continue;
					}
					
					//	--	check if the first read in the socket
					if (serv.clientMap[serv.vecPoll[idx].fd]._InitialRead)
					{
						serv.clientMap[serv.vecPoll[idx].fd]._tmp.append(buffer, byte);
						//	--	set Initial read to false
						serv.clientMap[serv.vecPoll[idx].fd]._InitialRead = false;

						//	-- separate request headers than body
						serv.clientMap[serv.vecPoll[idx].fd].separateHeadBody(serv.clientMap[serv.vecPoll[idx].fd]._tmp);

						if (!serv.clientMap[serv.vecPoll[idx].fd]._InitialRead)
						{
							//	-- check transfer encoding of the request body and save content-length if not chunked
							serv.clientMap[serv.vecPoll[idx].fd].transferEncoding();
							std::cout << serv.clientMap[serv.vecPoll[idx].fd]._reqBody;
							exit(1);
							// std::cout << "====== BYTE TO READ ===== : " << serv.clientMap[serv.vecPoll[idx].fd].byteToRead 
							// << " ====== Content-Length ===== : " << serv.clientMap[serv.vecPoll[idx].fd]._content_lenght << std::endl;

							//	-- checking the connexion sent with request
							serv.clientMap[serv.vecPoll[idx].fd].sockConnection();
							
							//	-- Determin which server is responsible for the request
							serv.clientMap[serv.vecPoll[idx].fd].hostResp();
						}
					}
					else
					{
						serv.clientMap[serv.vecPoll[idx].fd]._reqBody.append(buffer, byte);
					}
					// tmp.clear();
					serv.clientMap[serv.vecPoll[idx].fd].byteRead += byte;
					if (serv.clientMap[serv.vecPoll[idx].fd].byteToRead && serv.clientMap[serv.vecPoll[idx].fd].byteRead >= serv.clientMap[serv.vecPoll[idx].fd].byteToRead)
					{
						serv.clientMap[serv.vecPoll[idx].fd]._readiness = true;

						//	-- Forming request by assambling headers and body together to be traited in the req-resp part
						serv.clientMap[serv.vecPoll[idx].fd].formRequest();
						// std::cout << "+++ REQUEST LENGTH : " << serv.clientMap[serv.vecPoll[idx].fd]._request.length() << " ++++ \n\n";
						// std::cout << serv.clientMap[serv.vecPoll[idx].fd]._request;

						//	-- Create Response 
						serv.clientMap[serv.vecPoll[idx].fd].formResponse();
					}
					if (serv.clientMap[serv.vecPoll[idx].fd]._chunkedBody)
						std::cout << std::string(buffer, byte) << std::endl;
				}
				else if (serv.vecPoll[idx].revents & POLLOUT && serv.clientMap[serv.vecPoll[idx].fd]._readiness)
				{
					if (MAXSEND <= serv.clientMap[serv.vecPoll[idx].fd]._response.length())
						toSend = MAXSEND;
					else
						toSend = serv.clientMap[serv.vecPoll[idx].fd]._response.length();
					sent = send(serv.vecPoll[idx].fd, serv.clientMap[serv.vecPoll[idx].fd]._response.c_str(), toSend, 0);
					serv.clientMap[serv.vecPoll[idx].fd].byteSent += sent;
					serv.clientMap[serv.vecPoll[idx].fd].reFormResponse(sent);

					// std::cout << RED << "+> Byte sent : " << sent << END_CLR << std::endl;
					// std::cout << RED << "+> Byte sent from Response : " << serv.clientMap[serv.vecPoll[idx].fd].byteSent << END_CLR << std::endl;

					if (serv.clientMap[serv.vecPoll[idx].fd]._done)
					{
						// ********* reset client data first **************
						if (serv.clientMap[serv.vecPoll[idx].fd]._connexion == KEEP_ALIVE)
							serv.clientMap[serv.vecPoll[idx].fd].resetClientProp();
						else if (serv.clientMap[serv.vecPoll[idx].fd]._connexion == CLOSE)
						{
							close(serv.vecPoll[idx].fd);
							std::vector<struct pollfd>::iterator it = serv.vecPoll.begin() + (idx);
							serv.clientMap.erase(it->fd);
							serv.vecPoll.erase(it);
						}
					}
				}
			}
		}
	}
}