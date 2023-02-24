
#ifndef	SERV_SIDE_H
#define SERV_SIDE_H

#include "WebServer.hpp"

///////////////////// MACROS ////////////////////
#define	EXIT_FAILURE 1

typedef std::map<std::string, std::vector<std::string> >::iterator map_srv_data_it;
typedef std::vector<ServerConf> Vect_Serv;

typedef std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator vector_location_it;
typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator map_path_location_it;
typedef std::map<std::string, std::vector<std::string> >::iterator map_location_it;
///////////////////// ***** ////////////////////

class	Server
{
	public	:
		Server (int id, std::vector<std::string> portIp, std::vector<std::string> name);
		~Server ();

		int							getID			(void) const;
		std::vector<std::string>	getCombIpPort	(void) const;
		std::vector<std::string>	getServerName	(void) const;
		std::vector<int>			getSocketFD		(void) const;

		void						setSocket (std::vector<int> sock);

	private	:
		int							_id;
		std::vector<std::string>	_combIpPort;
		std::vector<std::string>	_serverName;
		std::vector<int>			_socketFD;
};

class	WebServ
{
	public :
		std::vector<Server>	servers;
		int					servNums;
		std::vector<struct pollFd>	fds;
		
		WebServ ();
		~WebServ ();
};

//	****	*********	****	//
//	****	TEMPLATES	****	//
//	****	*********	****	//

template <typename T>
void	printVector(std::vector<T> vec, std::string forWhat)
{
	typename std::vector<T>::iterator	it;

	it = vec.begin();
	std::cout << forWhat;
	while (it != vec.end())
	{
		std::cout << *it << " ";
		it++;
	}
	std::cout << std::endl;
}

template <typename T>
std::vector<T>	deepCopyVector(std::vector<T> vec)
{
	std::vector<T>	newVec;
	typename std::vector<T>::iterator	it;

	it = vec.begin();
	while (it != vec.end())
	{
		newVec.push_back(*it);
		it++;
	}
	return newVec;
}

//	****	*********	****	//
//	****	FUNCTIONS	****	//
//	****	*********	****	//


	//	+++	Server Utils	+++	//
void	printWebServ (WebServ *myServ);
std::vector<int>			portsExtraction(ServerConf server);
std::vector<std::string>	getStringKeyVal(ServerConf server, std::string key);
WebServ						*establishServers(Data &g_data);
void						createSockets(WebServ *serv);

#endif