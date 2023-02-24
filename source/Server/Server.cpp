#include "../../include/serverSide.hpp"

std::vector<int>	portsExtraction(ServerConf server)
{
	std::map<std::string, std::vector<std::string> > servData = server.server_data;
	std::string			key("listen");
	std::vector<int>	ports;
	std::stringstream	ss;
	int					nPort;

	if (servData.find(key) != servData.end())
	{
		for (std::vector<std::string>::iterator it = servData[key].begin(); it != servData[key].end(); it++)
		{
			ss << *it;
			ss >> nPort;
			// if (ss.fail())
			// 	throw ...
			ports.push_back(nPort);
			ss.str("");
			ss.clear();
		}
	}
	return ports;
}

std::vector<std::string>	getStringKeyVal(ServerConf server, std::string key)
{
	std::string		value;
	std::stringstream	ss;

	if (server.server_data.find(key) != server.server_data.end())
		return server.server_data[key];
	return	std::vector<std::string>();
}

WebServ	*establishServers(Data &g_data)
{
	WebServ *serv = new WebServ();
	int	id = 1;

	serv->servNums = 0;
	for (Vect_Serv::iterator it = g_data.server_list.begin(); it != g_data.server_list.end(); it++)
	{
		std::cout << "Server Id : " << id << " -> ";
		Server	newServ(id, getStringKeyVal(*it, "listen"), getStringKeyVal(*it, "server_name"));
		std::cout << std::endl;
		serv->servers.push_back(newServ);
		id++;
	}
	serv->servNums = --id;
	return serv;
}

void	createSockets(WebServ *serv)
{
	std::vector<std::string>	input;
	std::vector<int>	sockFd;
	std::vector<std::string>	host;
	int idx = 0;

	host.push_back("127.0.0.1");
	host.push_back("10.12.9.8");
	host.push_back("0.0.0.0");

	for (std::vector<Server>::iterator itServ = serv->servers.begin(); itServ != serv->servers.end(); itServ++)
	{
		input = itServ->getCombIpPort();
		for (std::vector<std::string>::iterator itPort = input.begin(); itPort != input.end(); itPort++)
		{
			// create socketfd = socket();
			int	server_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (server_sock < 0)
			{
				std::cerr << "Socket() Failed!!" << std::endl;
				exit (EXIT_FAILURE);
			}

			// set socket option to be reusable and its port = setsockopt()
			int	optval = 1;
			if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
			{
				std::cerr << "Setsockopt() Failed!!" << std::endl;
				exit(EXIT_FAILURE);
			}

			// Define struct sockaddr_in addrss = port + host
			

			// getting the address infos
			
			// Binding socketfd with addrss = bind()
			

			// listening to server_sock = listen()
			
			
			// pushing sockfd to vec = sockFd.push_back()
			
			
			// pushing sockfd to queue of I/O Multiplexer

		}
		std::cout << std::endl;

		//	set sockfd vector of the server = itServ.setSocket(sockFd)
		itServ->setSocket(sockFd);

		//	clear sockFd = sockFd.clear
		sockFd.clear();
		idx++;
	}
}