#include "../../include/serverSide.hpp"
#include "../../include/unitTests.hpp"

/**
 * @brief Read Request form client having POLLIN revent
 * 
 * @param std::vector<struct pollfd>::iterator:client 
 * @return int 
 */

int	WebServ::readRequest(std::vector<struct pollfd>::iterator client)
{
	char	buffer[MAXREAD] = {0};
	int		byte;

	bzero(&buffer, MAXREAD);

	byte = recv(client->fd, &buffer, MAXREAD, 0);

	//?:	saving last time reading from socket.
	clientMap[client->fd]._lastRead = ft_gettime();
					
	//?:	check if recv returns <= 0 (error case -> erase client socket)
	if (byte <= 0)
	{
		closeClientConn(client);
		return 1;
	}
	
	//?:	check if the first read in the socket
	if (clientMap[client->fd]._InitialRead)
	{
		clientMap[client->fd]._tmp.append(buffer, byte);
		//?:	set Initial read to false
		clientMap[client->fd]._InitialRead = false;

		//?:	separate request headers than body
		clientMap[client->fd].separateHeadBody(clientMap[client->fd]._tmp);

		if (!clientMap[client->fd]._InitialRead)
		{
			//?:	check transfer encoding of the request body and save content-length if not chunked
			clientMap[client->fd].transferEncoding();

			//?:	checking the connexion sent with request
			clientMap[client->fd].sockConnection();
			
			//?:	Determin which server is responsible for the request
			clientMap[client->fd].hostResp();
		}
	}
	else
	{
		//?:	Appending to body after separating header and body and Initial read is set to false
		clientMap[client->fd]._reqBody.append(buffer, byte);
	}
	if (clientMap[client->fd]._chunkedBody)
		clientMap[client->fd].readChunkBody();
	
	//?:	Appending byte read by recv() to know how many bytes are read
	clientMap[client->fd].byteRead += byte;

	return 0;
}

void	WebServ::socketReadiness(std::vector<struct pollfd>::iterator client)
{
	if (clientMap[client->fd].checkSockReady())
	{
		clientMap[client->fd]._readiness = true;

		//?:	Forming request by assambling headers and body together to be traited in the req-resp part
		clientMap[client->fd].formRequest();

		//?:	Create Response 
		clientMap[client->fd].formResponse();
	}
}