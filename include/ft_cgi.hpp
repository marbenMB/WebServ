#ifndef FT_CGI_HPP
#define FT_CGI_HPP

#include <fcntl.h>
#include "./WebServer.hpp"
#include "./request.hpp"

#define SUCCESS 200
#define ERR_FILE 404
#define ERR_SERV 500
// #define 

void set_env(request req, std::vector<char*> &env_vec);
int run_cgi(char **envp, std::string &_body);
std::string get_env(char **envp, std::string key);
std::string generate_client_id();

#endif