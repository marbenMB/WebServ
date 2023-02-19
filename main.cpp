#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
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

#define MAX_CLIENTS 10
#define PORT 5001

int getContentLengthInt(std::string buffer)
{
    int index = -1;
    std::vector<std::string> splitbuff = split(buffer, "\n\r\n\r");
    std::cout << "Header Size :" << splitbuff[0].size() << std::endl;
    splitbuff = split(splitbuff[0], "");
    std::vector<std::string>::iterator vectIT = splitbuff.begin();
    while (vectIT != splitbuff.end())
    {
        if ((*vectIT).find("Content-Length") != std::string::npos)
        {
            std::cout << "|" << (*vectIT) <<"|"<< std::endl;
            std::string tmp = (*vectIT);
            tmp.erase(0, strlen("Content-Length: "));
            index = atoi(tmp.c_str());
        }
        vectIT++;
    }
    // std::cout << RED << "*["<< splitbuff.size() <<"] :" << splitbuff[0] << END_CLR << std::endl;
    std::cout << "Content-Length :" << index << std::endl;

    return index;
}
int main()
{
    int server_fd, new_socket, max_clients = MAX_CLIENTS;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct pollfd clients[MAX_CLIENTS];
    char buffer[500000];
    std::string BUUF;
    Data g_Data;
    int index = 0;

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // // Set socket options to reuse address
    // int opt = 1;
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    // {
    //     perror("setsockopt failed");
    //     exit(EXIT_FAILURE);
    // }

    // Bind the socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Add the server socket to the clients array
    clients[0].fd = server_fd;
    clients[0].events = POLLIN;

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // Wait for events on the clients array
        if (poll(clients, max_clients, -1) < 0)
        {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }

        // Handle events on the clients array
        for (int i = 0; i < max_clients; i++)
        {
            int fd = clients[i].fd;
            short revents = clients[i].revents;

            // Handle events on the server socket
            if (fd == server_fd && revents & POLLIN)
            {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
                {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }

                // Add the new client socket to the clients array
                for (int j = 1; j < max_clients; j++)
                {
                    if (clients[j].fd == 0)
                    {
                        clients[j].fd = new_socket;
                        clients[j].events = POLLIN;
                        break;
                    }
                }
            }
            // Handle events on client sockets
            else if (revents & POLLIN)
            {
                int n = read(fd, buffer, sizeof(buffer));
                index += n;
                if (n <= 0)
                {
                    // Connection closed or error
                    printf("Client disconnected\n");
                    close(fd);
                    clients[i].fd = 0;
                    clients[i].events = 0;
                }
                BUUF.append(buffer);
                // std::cout << "Header[" << index << "] :" << buffer << std::endl;
                if (index >= getContentLengthInt(std::string(buffer)) + 869)
                {
                    std::vector<std::string> response_message;
                    request *req = new request(clients[i].fd, &g_Data, BUUF, response_message);
                    // std::cout << "ID : " << atoi(response_message[0].c_str())  << " body :" << response_message[1] << std::endl;
                    // std::vector<std::string>::iterator Buffer = split(buffer, "\r\n\r\n").begin();
                    // response_message = req->execute(clientrequest[1], &g_Data);
                    req->sand(atoi(response_message[0].c_str()), response_message[1]);
                    // Echo data back to the client
                    // write(fd, buffer, n);
                    BUUF.clear();
                }
            }
        }
    }
    return 0;
}