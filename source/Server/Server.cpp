#include "../../include/serverSide.hpp"

std::multimap<std::string, int>	portsExtraction(ServerConf server)
{
	//	...
	//	Loop all servers Ports and IPs.
	//	each IP with its Port | if Port has no IP then 0.0.0.0 is set by default.
	//	Return multimap.
}

std::vector<std::string>	getStringKeyVal(std::map<std::string, std::vector<std::string> > myMap, std::string key)
{
	std::string		value;
	std::stringstream	ss;

	if (myMap.find(key) != myMap.end())
		return myMap[key];
	return	std::vector<std::string>();
}

WebServ	*establishServers(Data &g_data)
{
	WebServ *serv = new WebServ();

	//	...
	//	Creating Severs
	//	puch them to WebServ vector.
	
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
}