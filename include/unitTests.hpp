
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

#endif