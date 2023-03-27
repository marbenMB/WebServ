#include "../../include/serverSide.hpp"

long long	ft_gettime(void)
{
	struct timeval	tp;
	long long		time;

	gettimeofday(&tp, NULL);
	time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	return (time);
}

/**
 * @brief Set the Option Socket object
 * 
 * @param int:fd 
 * @return int 
 */
int	setOptionSocket(int fd)
{
	int	optval = 1;

	//?-	Reusable addresss
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
		return 1;
	//?-	Protection form sigPipe signal
	if (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&optval, sizeof(optval)))
		return 1;
	//?-	Non Blocking fd
	fcntl(fd, F_SETFL,	O_NONBLOCK);
	return 0;
}

std::vector<std::string>	getStringKeyVal(std::map<std::string, std::vector<std::string> > myMap, std::string key)
{
	std::string		value;
	std::stringstream	ss;

	if (myMap.find(key) != myMap.end())
		return myMap[key];
	return	std::vector<std::string>();
}

void	logListen(WebServ serv)
{
	std::map<SockProp, std::vector<Server> >::iterator	it = serv.serverSockets.begin();

	std::cout << GREEN << "\n=========================================" << END_CLR << std::endl;
	std::cout << "+> WebServ Listen to : " << std::endl;
	while (it != serv.serverSockets.end())
	{
		std::cout << "		- " << it->first._IP << " : " << it->first._Port << std::endl;
		it++;
	}
	std::cout << GREEN << "=========================================" << END_CLR << std::endl;
}