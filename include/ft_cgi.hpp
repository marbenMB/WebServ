#ifndef FT_CGI_HPP
#define FT_CGI_HPP

#include <fcntl.h>
#include <sys/wait.h>
#include "./WebServer.hpp"
#include "./request.hpp"

#define SUCCESS 200
#define ERR_FILE 404
#define ERR_SERV 500
#define NEED_COOKIE 15
#define COOKIE_TIME 10
#define FAST_CGI_PASS "./source/cgi_files/cgi_script"
#define SESSION_TEMPLATE "./public/assets/session_template.html"
#define SESSION_PAGE "./public/session_list.html"

class request;
// #define 

void set_env(request req, std::vector<char*> &env_vec);
int run_cgi(char **envp, std::string &_body);
std::string get_env(char **envp, std::string key);
std::string generate_client_id();
void list_session_page();

#endif