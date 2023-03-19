
#ifndef	U_T_H
#define	U_T_H

#include "serverSide.hpp"

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

void	printMultiMap(std::multimap<std::string, int> mmap);
void	printWebServ (WebServ &myServ);
void	printSockProp (std::map<SockProp, std::vector<Server> > m);
void	printMapClient (std::map<int, ClientSock> m);
void	debug();
void	log(std::string forWhat, std::string msg);

#endif