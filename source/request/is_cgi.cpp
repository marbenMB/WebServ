#include "../../include/request.hpp"
#include "../../include/ft_cgi.hpp"

bool Is_cgi(std::string str)
{
    size_t pos = 0;
    if (((pos = str.rfind(".py")) != std::string::npos && (pos + 3) == str.length()) || ((pos = str.rfind(".go")) != std::string::npos && (pos + 3) == str.length()))
        return true;
    return false;
}

std::string generate_client_id() {
    std::stringstream client_id;
    std::srand(std::time(0));
    int id = rand() % 100000;
    client_id << "ID=";
    client_id << id;
    return (client_id.str());
}

void set_env(request req, std::vector<char*> &env_vec) {
    std::string line;
    std::ostringstream number_str;

    if (!req.getContent_Type().empty()) {
        line = "CONTENT_TYPE=";
        line += req.getContent_Type();
        env_vec.push_back(strdup(line.c_str()));
    }
    if (req.getContent_Length() > 0) {
        number_str << req.getContent_Length();
        line = "CONTENT_LENGTH=";
        line += number_str.str();
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req.getcookie().empty()) {
        line = "COOKIE=";
        line += req.getcookie();
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req.getquery_string().empty()) {
        line = "QUERY_STRING=";
        line += req.getquery_string();
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req.getreq_method().empty()) {
        line = "REQUEST_METHOD=";
        line += req.getreq_method();
        env_vec.push_back(strdup(line.c_str()));
    }

    if (!req.getrequest_URI().empty()) {
        line = "SCRIPT_FILENAME=";
        line += req.getrequest_URI();
        env_vec.push_back(strdup(line.c_str()));
    }
    if (!req.getrequest_URI().empty()) {
        if (req.getrequest_URI().find(".py") != std::string::npos)
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
    line = "HOME=/Users/mbenbajj";
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

int run_cgi(char **envp, std::string &_body) {
    int result = 0;
    pid_t pid;
    char **argv = new char*[4];
    std::string filename = "./source/cgi_files/cgi_script";
    filename += get_env(envp, "SCRIPT_FILENAME");
    int   fd_out = 1;
    int status;
    std::string body;
    std::string line;
    std::ifstream body_file;

    std::string exc;
    std::string cmd;
    std::string tmp;

    if (access(filename.c_str(), R_OK) == -1) {
        return (ERR_FILE);
    }

    else {
        pid = fork();
        if (pid < 0)
            return (ERR_SERV);
        else if (pid == 0) {
            if (filename.rfind(".py") == (filename.length() - 3))
            {
                argv[0] = strdup("/usr/bin/python3");
                argv[1] = strdup(filename.c_str());
                argv[2] = NULL;
            }
            else
            {
                argv[0] = strdup("/Users/mbenbajj/.brew/bin/go");
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
        }
        else {
            waitpid(pid, &status, 0);
            if (!WIFEXITED(status))
                result = WEXITSTATUS(status);
        }

    }

    // if (!result) {
        body_file.open("source/cgi_files/cgi_pages/file_cgi.html");
        if (body_file.is_open()) {
            while(std::getline(body_file, line)) {
                _body.append(line);
            }
            return (SUCCESS);
        }
        return (ERR_SERV);
    // }
    // else
    //     return (result);
}
