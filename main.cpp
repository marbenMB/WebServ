// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <poll.h>
// #include <iostream>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <netinet/in.h>
// #include <fstream>
// #include <unistd.h>
// #include "./include/request.hpp"
// #include "./include/method.hpp"

// #define MAX_CLIENTS 10
// #define PORT 5001

// int getContentLengthInt(std::string buffer)
// {
//     int index = -1;
//     std::vector<std::string> splitbuff = split(buffer, "\n\r\n\r");
//     std::cout << "Header Size :" << splitbuff[0].size() << std::endl;
//     splitbuff = split(splitbuff[0], "");
//     std::vector<std::string>::iterator vectIT = splitbuff.begin();
//     while (vectIT != splitbuff.end())
//     {
//         if ((*vectIT).find("Content-Length") != std::string::npos)
//         {
//             std::cout << "|" << (*vectIT) <<"|"<< std::endl;
//             std::string tmp = (*vectIT);
//             tmp.erase(0, strlen("Content-Length: "));
//             index = atoi(tmp.c_str());
//         }
//         vectIT++;
//     }
//     // std::cout << RED << "*["<< splitbuff.size() <<"] :" << splitbuff[0] << END_CLR << std::endl;
//     std::cout << "Content-Length :" << index << std::endl;

//     return index;
// }
// int main()
// {
//     int server_fd, new_socket, max_clients = MAX_CLIENTS;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     struct pollfd clients[MAX_CLIENTS];
//     char buffer[500000];
//     std::string BUUF;
//     Data g_Data;
//     int index = 0;

//     // Create a socket
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//     {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // // Set socket options to reuse address
//     // int opt = 1;
//     // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
//     // {
//     //     perror("setsockopt failed");
//     //     exit(EXIT_FAILURE);
//     // }

//     // Bind the socket to a port
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
//     {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     // Listen for incoming connections
//     if (listen(server_fd, 3) < 0)
//     {
//         perror("listen failed");
//         exit(EXIT_FAILURE);
//     }

//     // Add the server socket to the clients array
//     clients[0].fd = server_fd;
//     clients[0].events = POLLIN;

//     printf("Server listening on port %d...\n", PORT);

//     while (1)
//     {
//         // Wait for events on the clients array
//         if (poll(clients, max_clients, -1) < 0)
//         {
//             perror("poll failed");
//             exit(EXIT_FAILURE);
//         }

//         // Handle events on the clients array
//         for (int i = 0; i < max_clients; i++)
//         {
//             int fd = clients[i].fd;
//             short revents = clients[i].revents;

//             // Handle events on the server socket
//             if (fd == server_fd && revents & POLLIN)
//             {
//                 if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
//                 {
//                     perror("accept failed");
//                     exit(EXIT_FAILURE);
//                 }

//                 // Add the new client socket to the clients array
//                 for (int j = 1; j < max_clients; j++)
//                 {
//                     if (clients[j].fd == 0)
//                     {
//                         clients[j].fd = new_socket;
//                         clients[j].events = POLLIN;
//                         break;
//                     }
//                 }
//             }
//             // Handle events on client sockets
//             else if (revents & POLLIN)
//             {
//                 int n = read(fd, buffer, sizeof(buffer));
//                 index += n;
//                 if (n <= 0)
//                 {
//                     // Connection closed or error
//                     printf("Client disconnected\n");
//                     close(fd);
//                     clients[i].fd = 0;
//                     clients[i].events = 0;
//                 }
//                 BUUF.append(buffer);
//                 // std::cout << "Header[" << index << "] :" << buffer << std::endl;
//                 if (index >= getContentLengthInt(std::string(buffer)) + 869)
//                 {
//                     std::vector<std::string> response_message;
//                     request *req = new request(clients[i].fd, &g_Data, BUUF, response_message);
//                     // std::cout << "ID : " << atoi(response_message[0].c_str())  << " body :" << response_message[1] << std::endl;
//                     // std::vector<std::string>::iterator Buffer = split(buffer, "\r\n\r\n").begin();
//                     // response_message = req->execute(clientrequest[1], &g_Data);
//                     req->sand(atoi(response_message[0].c_str()), response_message[1]);
//                     // Echo data back to the client
//                     // write(fd, buffer, n);
//                     BUUF.clear();
//                 }
//             }
//         }
//     }
//     return 0;
// }



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
 #define PORT 9090

int _main(int ac, char *av[])
{
    // system("leaks webServ");
    Data g_Data;
    int sockfd, newsockfd;
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

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

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
        // sleep(5);
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
        // exit(1);
    }

    return 0;
}
void CheckLeack( void ){
    system("leaks webServ");
}
int main(int ac, char *av[]){
    // atexit(CheckLeack);
    _main(ac, av);
    
}