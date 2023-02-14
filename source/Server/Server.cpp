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

WebServ	*estabilishServers(Data &g_data)
{
	WebServ *serv = new WebServ();

	for (std::vector<ServerConf>::iterator it = g_data.server_list.begin(); it != g_data.server_list.end(); it++)
	{

		Server	newServ(portsExtraction(*it), getStringKeyVal(*it, "server_name"), getStringKeyVal(*it, "host"));
		serv->servers.push_back(newServ);
	}
	return serv;
}