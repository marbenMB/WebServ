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


	for (std::vector<Server>::iterator itServ = serv->servers.begin(); itServ != serv->servers.end(); itServ++)
	{
		input = itServ->getListenPorts();
		for (std::vector<int>::iterator itPort = input.begin(); itPort != input.end(); itPort++)
		{
			// create socketfd = socket();
			int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0)
			{
				std::cerr << "Socket() Failed!!" << std::endl;
				exit (EXIT_FAILURE);
			}

			// set socket option to be reusable and its port = setsockopt()
			int	optval = 1;
			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
			{
				std::cerr << "Setsockopt() Failed!!" << std::endl;
				exit(EXIT_FAILURE);
			}

			// Define struct sockaddr_in addrss = port + host
			struct sockaddr_in	addr;

			bzero(&addr, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(*itPort);
			addr.sin_addr.s_addr = INADDR_ANY;

			// Binding socketfd with addrss = bind()
			if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
			{
				std::cerr << "Bind() Failed!!" << std::endl;
				exit (EXIT_FAILURE);
			}

			// listening to sockfd = listen()
			if (listen(sockfd, 5) < 0)
			{
				std::cerr << "Listen() Failed!!" << std::endl;
				exit (EXIT_FAILURE);
			}

			// pushing sockfd to vec = sockFd.push_back()
			// pushing sockfd to queue of I/O Multiplexer
		}
		std::cout << std::endl;
		//	set sockfd vector of the server = itServ.setSocket(vec)
		//	clear vec = vec.clear
	}
}