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

		// std::cout << "IP : " << ip << "	" << "PORT : " << port << std::endl;

		listen.insert(std::make_pair(ip, port));
	}
	// std::cout << "-------------------" << std::endl;

	return listen;

	// (void)combIpPort;
	// return std::multimap<std::string, int>();
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

		printMultiMap(newServ.getIpPort());
		std::cout << "----------------------------" << std::endl
		<< "Server ID : " << newServ.getID() << std::endl;
		printVector(newServ.getCombIpPort(), "Server Ports : ");
		printVector(newServ.getServerName(), "Server Name : ");
		std::cout << "----------------------------" << std::endl;

		id++;
		serv->servers.push_back(newServ);
	}

	return serv;
}

void	createSockets(WebServ *serv)
{
	//	...
	//	Loop to each Server and Create socket


	// set socket option to be reusable and its port = setsockopt()


	// Define struct sockaddr_in addrss = port + host


	// getting the address infos


	// Binding socketfd with addrss = bind()


	// listening to server_sock = listen()


	// pushing sockfd to vec = sockFd.push_back()


	//	pushing sockfd to queue of I/O Multiplexer


	//	set sockfd vector of the server


	//	clear sockFd = sockFd.clear
	(void)serv;
}