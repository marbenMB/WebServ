#include "../../include/WebServer.hpp"

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

	for (std::vector<ServerConf>::iterator it = g_data.server_list.begin(); it != g_data.server_list.end(); it++)
	{

		Server	newServ(id, portsExtraction(*it), getStringKeyVal(*it, "server_name"), getStringKeyVal(*it, "host"));
		serv->servers.push_back(newServ);
		id++;
	}
	serv->servNums = --id;
	return serv;
}

void	createSockets(WebServ *serv)
{
	std::vector<int>	input;
	std::vector<int>	sockFd;
	int					idx;


	for (std::vector<Server>::iterator itServ = serv->servers.begin(); itServ != serv->servers.end(); itServ++)
	{
		input = itServ->getListenPorts();
		idx = 0;
		for (std::vector<int>::iterator itPort = input.begin(); itPort != input.end(); itPort++)
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
			struct sockaddr_in	addr;
			std::string			host;

			host = itServ->getServerHost()[idx];
			bzero(&addr, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(*itPort);
			addr.sin_addr.s_addr = inet_addr(host.c_str());
			std::cout << "Port : " << *itPort << " - Host : " << host << std::endl;

			// getting the address infos
			// std::stringstream	ss;
			// std::string			strPort;
			// struct addrinfo		hints;
			// struct addrinfo		*res;
			// int 				addrStat;

			// ss << *itPort;
			// ss >> strPort;
			// hints.ai_family = AF_INET;
			// hints.ai_socktype = SOCK_STREAM;
			// addrStat = getaddrinfo(host.c_str(), strPort.c_str(), &hints, &res);
			// if (addrStat != 0)
			// {
			// 	std::cerr << "Current Host : " << host << " is not available !!" << std::endl;
			// 	exit (EXIT_FAILURE);
			// }
			// std::cout << host << " - " << addrStat << std::endl;

			// Binding socketfd with addrss = bind()
			if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
				throw	std::runtime_error("Bind Failed!!");

			// listening to server_sock = listen()
			if (listen(server_sock, 5) < 0)
			{
				std::cerr << "Listen() Failed!!" << std::endl;
				exit (EXIT_FAILURE);
			}
			
			// pushing sockfd to vec = sockFd.push_back()
			sockFd.push_back(server_sock);
			
			// pushing sockfd to queue of I/O Multiplexer

			// accepting request = accept()
			int	client_sock;
			struct sockaddr_in	client_addr;
			socklen_t	client_addrSize = sizeof(client_addr);

			client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addrSize);
			if (client_sock < 0)
			{
				std::cerr << "Accept() Failied!!" << std::endl;
				exit (EXIT_FAILURE);
			}
			
			std::cout << "Request Accepted in port : " << *itPort << std::endl;
			close(client_sock);
			idx++;
		}
		std::cout << std::endl;

		//	set sockfd vector of the server = itServ.setSocket(sockFd)
		itServ->setSocket(sockFd);

		//	clear sockFd = sockFd.clear
		sockFd.clear();
	}
}