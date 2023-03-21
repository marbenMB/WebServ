#include "../../include/request.hpp"
#include "../../include/ft_cgi.hpp"

bool Is_cgi(std::string str)
{
    size_t pos = 0;
    if (((pos = str.rfind(".py")) != std::string::npos && (pos + 3) == str.length()) ||
    ((pos = str.rfind(".go")) != std::string::npos && (pos + 3) == str.length()) 
    ) return true;
    return false;
}

std::string generate_id() {
    std::stringstream client_id;
    std::srand(time(0));
    int id = rand() % 1000;
    client_id << id;
    return (client_id.str());
}

void set_env(request req, std::vector<char*> &env_vec, int is_valid) {
    std::string line;
    std::ostringstream number_str;

    if (!req._findHeader("Content-Type").empty()) {
        line = "CONTENT_TYPE=";
        line += req._findHeader("Content-Type");
        env_vec.push_back(strdup(line.c_str()));
    }
    if (!req._findHeader("Content-Length").empty()) {
        line = "CONTENT_LENGTH=";
        line += req._findHeader("Content-Length");
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req._findHeader("Cookie").empty()) {
        line = "COOKIE=";
        line += req._findHeader("Cookie");
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req._findHeader(PARAMS).empty()) {
        line = "QUERY_STRING=";
        line += req._findHeader(PARAMS);
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req._findHeader(REQUEST_METHOD).empty()) {
        line = "REQUEST_METHOD=";
        line += req._findHeader(REQUEST_METHOD);
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req. _findHeader(REQUEST_URI).empty()) {
        line = "SCRIPT_FILENAME=";
        line += req. _findHeader(REQUEST_URI);
        env_vec.push_back(strdup(line.c_str()));
    }
    if (!req. _findHeader(REQUEST_URI).empty()) {
        if (req. _findHeader(REQUEST_URI).find(".py") != std::string::npos)
            line = "SCRIPT_LANG=python";
        else
            line = "SCRIPT_LANG=go";
        env_vec.push_back(strdup(line.c_str()));
    }
    if (!req.getCGIbody().empty()) {
        line = "REQUEST_BODY=";
        line += req.getCGIbody();
        env_vec.push_back(strdup(line.c_str()));
    }
    if (!req.getFastcgiPass().empty()) {
        line = "FASTCGI_PASS=";
        line += req.getFastcgiPass();
        env_vec.push_back(strdup(line.c_str()));
    }
    else
    {
        line = "FASTCGI_PASS=";
        line += "./source/cgi_files/cgi_script";
        env_vec.push_back(strdup(line.c_str()));
    }
    if (is_valid == NEED_COOKIE) {
        line = "VALID_COOKIE=NO";
        env_vec.push_back(strdup(line.c_str()));
    }
    else {
        line = "VALID_COOKIE=YES";
        env_vec.push_back(strdup(line.c_str()));
    }
    line = "HOME=";
    line += HOME_PATH;
    env_vec.push_back(strdup(line.c_str()));
    env_vec.push_back(NULL);
}

std::string get_env(char **envp, std::string key) {
    int i = 0;
    char *str = NULL;
    while (envp && envp[i]) {
        str = strstr(envp[i], key.c_str());
        if (str) {
            std::string line(str);
            return (line.erase(0, (key.length() + 1)));
        }
        i++;
    }
    return ("NULL");
}

void run_child(std::string filename, char **envp) {
    int             fd_out = 1;
    char            **argv = new char*[4];

    if (filename.rfind(".py") == (filename.length() - 3))
    {
        argv[0] = strdup("/usr/bin/python3");
        argv[1] = strdup(filename.c_str());
        argv[2] = NULL;
    }
    else
    {
        argv[0] = strdup(GO_PATH);
        argv[1] = strdup("run");
        argv[2] = strdup(filename.c_str());
        argv[3] = NULL;
    }
    fd_out = open("source/cgi_files/cgi_pages/file_cgi.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd_out == -1) {    
        exit (ERR_SERV);
    }
    if (dup2(fd_out, 1) == -1)
        exit (ERR_SERV);
    if (execve(argv[0], argv, envp) == -1){
        exit (ERR_SERV);
    }
    delete[] argv;
    close(fd_out);
}

int run_cgi(char **envp, std::string &_body) {
    int             result = 0;
    int             status;
    pid_t           pid;
    std::string     filename;
    std::string     line;
    std::ifstream   body_file;
    
    filename += get_env(envp, "FASTCGI_PASS") + get_env(envp, "SCRIPT_FILENAME");
    if (access(filename.c_str(), R_OK) == -1)
        return (ERR_FILE);
    else {
        pid = fork();
        if (pid < 0)
            return (ERR_SERV);
        else if (pid == 0)
            run_child(filename, envp);
        else {
            waitpid(pid, &status, 0);
            if (!WIFEXITED(status))
                result = WEXITSTATUS(status);
        }
    }
    if (!result) {};
    body_file.open("source/cgi_files/cgi_pages/file_cgi.html");
    if (body_file.is_open()) {
        while(std::getline(body_file, line)) {
            _body.append(line);
        }
        body_file.close();
        return (SUCCESS);
    }
    body_file.close();
    return (ERR_SERV);
}

std::string getCurrentTimeplus() {
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[80];

  time(&rawtime);
  rawtime +=  COOKIE_TIME;
  timeinfo = localtime(&rawtime);

  strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
  std::string str(buffer);

  return str;
}

bool is_valid_cookie(std::string Cookie) {
    size_t      index_cookie;

    if (!Cookie.empty()) {
        index_cookie = Cookie.find("CookieId");
        if (index_cookie != std::string::npos)
            return true;
    }
    return false;
}

void free_env(char **env) {
    int i = 0;
    if (env) {
        while (env[i]) {
            free (env[i]);
            i++;
        }
    }
}

void request::CGI::cookie_session(request req, std::string &Cookie_value, int &is_valid) {
    std::string yes_no;
    size_t is_cookie = std::string::npos;
    size_t is_session = std::string::npos;
    std::string login;

    if (!is_valid_cookie(req._findHeader("Cookie")))
        is_valid = NEED_COOKIE;
    is_cookie = req.CGIbody.find("login=");
    is_session = req.CGIbody.find("&session=");
    if (is_cookie != std::string::npos)
        login = generate_id();
    if (is_session != std::string::npos)
        yes_no = req.CGIbody.substr(is_session + 9, req.CGIbody.length());
    Cookie_value.clear();
    if (is_cookie != std::string::npos && is_valid == NEED_COOKIE) {
        Cookie_value = "CookieId=";
        Cookie_value += login + "; Expires=";
        Cookie_value += getCurrentTimeplus();
        Cookie_value += "; path=/";
        is_valid = 0;
        is_cookie = std::string::npos;
    }
    if (yes_no.compare("yes") == 0){
        Cookie_value = "Session";
        Cookie_value += generate_id() + "=" + generate_id();
    }
}


method * request::CGI::runCGI(request req){
    char**              envp;
    std::vector<char*>  env_vec;
    std::string         _body;
    method              *resp;
    std::string         Cookie_value;
    int                 is_valid = 0;
    int                 status;
    
    cookie_session(req, Cookie_value, is_valid);
    set_env(req, env_vec, is_valid);
    envp = &env_vec[0];
    resp = new Error(req);
    status = run_cgi(envp, _body);
    free_env(envp);
    if (status == SUCCESS) {
        resp->setStatuscode(200);
        resp->setreason_phrase("OK");
        resp->setResponseBody(_body);
        resp->addHeader("Content-Type", "text/html");
        resp->addHeader("Content-Length", ft_to_string(_body.length()));
        resp->addHeader("Set-Cookie", Cookie_value);
    }
    else {
        delete resp;
        // system("leaks webServ");
        throw _Exception(status);
    }
    // system("leaks webServ");
    return resp;
}