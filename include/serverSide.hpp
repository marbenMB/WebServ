
#ifndef	SERV_SIDE_H
#define SERV_SIDE_H

#include "WebServer.hpp"

///////////////////// MACROS ////////////////////
#define	EXIT_FAILURE 1
#define	DUP_PORT	2

#define DEFAULT_IP "0.0.0.0"
#define	DEFAULT_PORT 80

typedef std::vector<ServerConf> Vect_Serv;
typedef std::map<std::string, std::vector<std::string> >::iterator map_srv_data_it;

typedef std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator vector_location_it;
typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator map_path_location_it;
typedef std::map<std::string, std::vector<std::string> >::iterator map_location_it;
///////////////////// ***** ////////////////////

class	Server
{
	public	:
		Server (int id, ServerConf server, std::vector<std::string> portIp, std::vector<std::string> name);
		~Server ();

		int							getID			(void) const;
		std::vector<std::string>	getCombIpPort	(void) const;
		std::vector<std::string>	getServerName	(void) const;
		std::multimap<std::string, int>	getIpPort	(void) const;

		void						setIpPort	(std::multimap<std::string, int> listen);

	private	:
		int							_id;
		std::vector<std::string>	_combIpPort;
		std::vector<std::string>	_serverName;
		ServerConf					_Server;
		std::multimap<std::string, int>	_IpPort;

		// std::vector<int>			_socketFD;
};

class	WebServ
{
	public :
		std::vector<Server>	servers;
		int					servNums;
		// std::vector<struct pollFd>	fds;
		
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


	//	+++	 Utils	+++	//
void	printMultiMap(std::multimap<std::string, int> mmap);
std::vector<std::string>	getStringKeyVal(std::map<std::string, std::vector<std::string> > myMap, std::string key);

	//	+++	Checkint functions +++	//
int	checkDuplicatePort(std::multimap<std::string, int> Map);

	//	+++	Server Utils	+++	//
void	printWebServ (WebServ *myServ);
std::multimap<std::string, int>	extractionIpPort(std::vector<std::string> combIpPort);
WebServ						*establishServers(Data &g_data);
void						createSockets(WebServ *serv);

#endif