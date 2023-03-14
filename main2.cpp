#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include "./include/request.hpp"
#include "./include/method.hpp"
#define PORT 8080
#define MAX_CLIENTS 10
#define MB 500000
std::string dataRequest;
using namespace std;

int main(int ac, char *av[]) {
    Data g_Data;
    int server_fd, new_socket, valread = 0;
    bool endRead = false;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int clients[MAX_CLIENTS] = {0};
    char buffer[MB] = {0};
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
    struct pollfd fds[MAX_CLIENTS + 1];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        cerr << "Binding failed" << endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        cerr << "Listen failed" << endl;
        return -1;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        fds[i].fd = -1;
    }

    fds[0].fd = server_fd;
    fds[0].events = POLLIN | POLLOUT;
    dataRequest = "";
    while (true) {
        int num_fds = poll(fds, MAX_CLIENTS + 1, -1);

        if (num_fds < 0) {
            cerr << "Poll error" << endl;
            return -1;
        }

        if (fds[0].revents & POLLIN) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                cerr << "Accept failed" << endl;
                return -1;
            }
    
            for (int i = 1; i < MAX_CLIENTS + 1; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = new_socket;
                    fds[i].events = POLLIN | POLLOUT;
                    clients[i - 1] = new_socket;
                    break;
                }
            }
        }

        for (int i = 1; i < MAX_CLIENTS + 1; i++) {
            if (fds[i].revents & POLLIN) {
                int valread2 = read(fds[i].fd, buffer, MB);
                valread+=valread2;
                dataRequest.append(buffer, valread2);
                if (valread == 0) {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    clients[i - 1] = 0;
                }
                if(valread > 19703)
                    endRead = true;
                // else {
                //     for (int j = 1; j < MAX_CLIENTS + 1; j++) {
                //         if (clients[j - 1] != fds[i].fd && clients[j - 1] != 0) {
                //             send(clients[j - 1], buffer, strlen(buffer), 0);
                //         }
                //     }
                // }
            }
            if (fds[i].revents & POLLOUT) {

                if(endRead)
                {
                    std::cout << "buffer :" << valread << std::endl;
                    std::cout << "buffer :" << dataRequest << std::endl;
                     std::vector<std::string> response_message;
                    request *req = new request(fds[i].fd, &g_Data, buffer, response_message);
                    // std::cout << "ID : " << atoi(response_message[0].c_str())  << " body :" << response_message[1] << std::endl;
                    // std::vector<std::string>::iterator Buffer = split(buffer, "\r\n\r\n").begin();
                    // response_message = req->execute(clientrequest[1], &g_Data);
                    req->sand(atoi(response_message[0].c_str()), response_message[1]);
                    dataRequest.clear();
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    clients[i - 1] = 0;
                }
            }
        }
    }

    return 0;
}