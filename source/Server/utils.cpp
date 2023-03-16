#include "../../include/serverSide.hpp"

long long	ft_gettime(void)
{
	struct timeval	tp;
	long long		time;

	gettimeofday(&tp, NULL);
	time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	return (time);
}

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