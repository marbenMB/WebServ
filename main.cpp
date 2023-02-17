
// ? Server :
// ! Create Socket
/*
* +> int socket (int family, int type, int protocol);

*/
// ! Bind Socket to an adress
/*
* +> int bind(int sockfd, struct sockaddr *my_addr,int addrlen);

* sockfd − It is a socket descriptor returned by the socket function.
* my_addr − It is a pointer to struct sockaddr that contains the local IP address and port.
* addrlen − Set it to sizeof(struct sockaddr).
*/
// ! Listen for connections
/*
* +> int listen(int sockfd,int backlog);

* sockfd − It is a socket descriptor returned by the socket function.
* backlog − It is the number of allowed connections.
*/
// ! Accept a connection
/*
* +> int accept (int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

* sockfd − It is a socket descriptor returned by the socket function.
* cliaddr − It is a pointer to struct sockaddr that contains client IP address and port.
* addrlen − Set it to sizeof(struct sockaddr).
*/
// ! Send and receive data
/*
* +> int send(int sockfd, const void *msg, int len, int flags);

* sockfd − It is a socket descriptor returned by the socket function.
* msg − It is a pointer to the data you want to send.
* len − It is the length of the data you want to send (in bytes).
* flags − It is set to 0.
*/
//  ? Client :
// ! Create Socket
// ! Connect the socket to the address of the server
/*
* +> int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

* sockfd − It is a socket descriptor returned by the socket function.
* serv_addr − It is a pointer to struct sockaddr that contains destination IP address and port.
* addrlen − Set it to sizeof(struct sockaddr).
*/

// ! Send and receive data
/*
* +> int recv(int sockfd, void *buf, int len, unsigned int flags);

*sockfd − It is a socket descriptor returned by the socket function.
* buf − It is the buffer to read the information into.
* len − It is the maximum length of the buffer.
* flags − It is set to 0.
*/

// * Structures

//      => sockaddr
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include "./include/request.hpp"
#include "./include/method.hpp"

int main(int ac, char *av[])
{
    // system("leaks webServ");
    Data g_Data;
    int sockfd, newsockfd, portno;
    // long maxBayte = 2147483648;
    long maxBayte = 2147483647;
    socklen_t clilen; // unsigned int
    char *buffer;
    std::string tmp;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (ac == 2)
    {
        g_Data.configuration.parcing_file(av[1], g_Data);
    }
    else
        g_Data.error = "error n number of argument";
    if (!g_Data.error.empty())
    {
        std::cout << RED << g_Data.error << END_CLR << std::endl;
        // system("leaks webserv");
        return (1);
    }
    tmp = (char *)malloc(sizeof(char) * maxBayte);
    buffer = (char *)malloc(sizeof(char) * maxBayte);

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
     */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (true)
    {
        /* Accept actual connection from the client */

        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        // close(sockfd);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            return 1;
        }

        /* If connection is established then start communicating */
        bzero(buffer, maxBayte);
        // bzero(tmp, maxBayte);
        std::string Buff = "";

        //  std::string buff[7000];
        // recv(newsockfd, (char *)tmp.c_str(), sizeof(buff), 0);
        sleep(15);
        // n = recv(newsockfd, buffer, maxBayte, 0);
        n = read(newsockfd, buffer, maxBayte);
        // while (1)
        // {
        //     n = read(newsockfd, (char *)tmp.c_str(), 1024);
        //     if (tmp.find("\r\n\r\n", 0) != std::string::npos)
        //         break;
        //     std::cout << "buff : " << tmp << std::endl;
        //     Buff.append(tmp);
        // }
        // std::cout << "read"<< n <<"- : " << buffer << std::endl << std::endl;
        // std::cout << buffer << std::endl;
        if (n < 0)
        {
            perror("ERROR reading from socket");
            return 1;
        }

        // std::cout << "byte : " << n << std::endl;
        // printf("Here is the message: \n%s\n", buffer);
        // std::vector<std::string> clientrequest = split(buffer, "\r\n\r\n");
        std::vector<std::string> response_message;
        request *req = new request(newsockfd, &g_Data, buffer, response_message);
        // std::cout << "ID : " << atoi(response_message[0].c_str())  << " body :" << response_message[1] << std::endl;
        // std::vector<std::string>::iterator Buffer = split(buffer, "\r\n\r\n").begin();
        // response_message = req->execute(clientrequest[1], &g_Data);
        req->sand(atoi(response_message[0].c_str()), response_message[1]);

        close(newsockfd);
        delete req;
    }

    return 0;
}
