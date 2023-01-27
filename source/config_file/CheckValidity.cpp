#include "../../include/WebServer.hpp"

char* ft_join(char *str1, const char *str2) {
    strcat(str1, str2);
    return str1;
}

void server_data(Data &g_Data) {
    typedef std::map<std::string, std::vector<std::string> >::iterator srv_data_it;
    struct addrinfo                                                    hints, *res;
    int                                                                status;
    struct stat                                                        sb;
    std::string                                                        root_path;
    int                                                               check_error_page = 0;

    std::vector<ServerConf> server_list = g_Data.server_list;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    for (std::vector<ServerConf>::iterator it = server_list.begin(); it != server_list.end(); ++it) {
        for (srv_data_it server_data_it = it->server_data.begin(); server_data_it != it->server_data.end(); ++server_data_it) {
            for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end(); ++value_it) {
                if (server_data_it->first == "root") {
                    if (stat((*value_it).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                        root_path = *value_it;
                        if (root_path[root_path.length() - 1] != '/')
                            root_path += "/";
                    }
                    else {
                        g_Data.error = "error : The root value is not valide";
                        return ;
                    }
                }
            }
        }
    }
    for (std::vector<ServerConf>::iterator it = server_list.begin(); it != server_list.end(); ++it) {
        for (srv_data_it server_data_it = it->server_data.begin(); server_data_it != it->server_data.end(); ++server_data_it) {
            for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end(); ++value_it) {
                if (server_data_it->first == "server_name") {
                    status = getaddrinfo((*value_it).c_str(), NULL, &hints, &res);
                    if (status != 0) {
                        g_Data.error = "error : server name not valide";
                        break;
                    }
                }
                else if (server_data_it->first == "host") {
                    status = getaddrinfo((*value_it).c_str(), NULL, &hints, &res);
                    if (status != 0) {
                        g_Data.error = "error : host value not valide";
                        break;
                    }
                }
                else if (server_data_it->first == "listen") {
                    status = getaddrinfo(NULL, (*value_it).c_str(), &hints, &res);
                    if (status != 0) {
                        g_Data.error = "error : Port value not valide";
                        break;
                    }
                }
                else if (server_data_it->first == "index") {
                    if (!(stat((root_path + (*value_it)).c_str(), &sb) == 0) || !S_ISREG(sb.st_mode)) {
                        g_Data.error = "error : The index value is not valide";
                    }
                }
                else if (server_data_it->first == "error_page" && !check_error_page)
                {
                    check_error_page = true;
                    for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end(); ++value_it) {
                        if ((check_error_page = (*value_it).find("."))  && check_error_page != -1) {
                            if (!(stat((root_path + (*value_it)).c_str(), &sb) == 0) || !S_ISREG(sb.st_mode)) {
                                g_Data.error = "error : The error page not found";
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void check_validity(Data &g_Data) {
    if (!g_Data.error.length()) {
        server_data(g_Data);

    }
    
}