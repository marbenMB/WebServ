#include "../../include/serverSide.hpp"

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

WebServ	*establishServers(Data &g_data)
{
	WebServ 					*serv = new WebServ();
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
			throw	std::invalid_argument("+> Duplicated Port!!");

		id++;
		serv->servers.push_back(newServ);
	}
	serv->servNums = id - 1;
	serv->nSocketServer = 0;
	return serv;
}

void	createSockets(WebServ *serv)
{
	std::multimap<std::string, int>	mmap;
	
	int		sockFd;
	int		optval = 1;

	struct addrinfo	hints, *res;
	std::stringstream		ss;
	std::string				strPort;

	struct sockaddr_in	addr;

	//	...
	//	Loop to each Server and Create socket
	for (std::vector<Server>::iterator servIt = serv->servers.begin(); servIt != serv->servers.end(); servIt++)
	{
		mmap = servIt->getIpPort();
		for (std::multimap<std::string, int>::iterator it = mmap.begin(); it != mmap.end(); it++)
		{
			//	-- Socket creation and setting reused addr option.
			sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sockFd < 0)
				throw	std::runtime_error("Socket() Failed!!");
			fcntl(sockFd, F_SETFL,	O_NONBLOCK);
			if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
				throw	std::runtime_error("Setsockopt() Failed!!");
			
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
				throw	std::runtime_error("Address Not Available!!");
			}

			//	-- Binding the socket with Ip and Port
			bzero(&addr, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(it->second);
			addr.sin_addr.s_addr = inet_addr(it->first.c_str());
			if (bind(sockFd, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
			{
				if (checkDefaultServer(serv->serverSockets, it->first, it->second, *servIt))
					throw	std::runtime_error("Bind() Failed!!");
			}

			//	--	Listening to socket
			if (listen(sockFd, 5))
				throw	std::runtime_error("Listen() Failed!!");
			
			//	-- Create sockPorp for each socket and insert it to the map
			SockProp	prop(sockFd, it->second, it->first, SERVER_SOCK);
			
			//	-- Push socket to poll vector
			serv->vecPoll.push_back(prop._pSFD);

			//	-- Insert sockProp and vector of Server in the serverSockets map
			serv->serverSockets[prop];
			serv->serverSockets[prop].push_back(*servIt);

			//	--	Incrementing number of sockets
			serv->nSocketServer++;
		}
	}



	// listening to server_sock = listen()


	// pushing sockfd to vec = sockFd.push_back()


	//	pushing sockfd to queue of I/O Multiplexer


	//	set sockfd vector of the server


	//	clear sockFd = sockFd.clear
	(void)serv;
}