#include "../../include/WebServer.hpp"
#include <iostream>
#include <sstream>

char* ft_join(char *str1, const char *str2) {
    strcat(str1, str2);
    return str1;
}

void convert_body_size(std::string &size, std::string number, char type) {
    
    std::ostringstream s_num;
    long num = std::atoi(number.c_str());

    if (type == 'G')
        s_num << (num * 1024 * 1024);
    else if (type == 'K')
        s_num << num;
    else if (type == 'M')
        s_num << (num * 1024);
    size = s_num.str();
}

void server_data(Data &g_Data) {
    typedef std::map<std::string, std::vector<std::string> >::iterator srv_data_it;
    // typedef std::vector<std::map<std::string,std::map<std::string, std::vector<std::string> > > >::iterator ser_loc_it;
    // typedef std::map<std::string,std::map<std::string, std::vector<std::string> > >::iterator ser_loc_map_it;
    // typedef std::map<std::string, std::vector<std::string> >::iterator map_loc_val_it;
    // typedef std::vector<std::string>::iterator fin_val;
    struct addrinfo                                                    hints, *res;
    int                                                                status;
    struct stat                                                        sb;
    std::string                                                        root_path;
    int                                                               check_error_page = 0;
    char                                                              type;
    std::string                                                       number;
    int                                                               index;

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
                    for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end(); ++value_it) {
                        if ((check_error_page = (*value_it).find("."))  && check_error_page != -1) {
                            if (!(stat((root_path + (*value_it)).c_str(), &sb) == 0) || !S_ISREG(sb.st_mode)) {
                                g_Data.error = "error : The error page not found";
                                break;
                            }
                        }
                    }
                }
                else if (server_data_it->first == "client_max_body_size")
                {
                    for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end(); ++value_it) {
                        // std::cout << *value_it << std::endl;
                        type = (*value_it)[(*value_it).length() - 1];
                        if (type == 'M' || type == 'K' || type == 'G') {
                            number = *value_it;
                            number.erase((*value_it).length() - 1, 1);
                            index = number.find_first_not_of("0123456789");
                            if (index != -1) {
                                g_Data.error = "the body size not valid";
                                break;
                            }
                            else
                                convert_body_size(*value_it, number, type);
                            std::cout << *value_it << std::endl;

                        }
                        else {
                            g_Data.error = "the body size not valid";
                            break;
                        }
                    }
                }

            }
        }
        //////
        // for (ser_loc_it server_loc_it = it->locations.begin(); server_loc_it != it->locations.end(); ++server_loc_it) {
        //     for(ser_loc_map_it server_loc_map_it = server_loc_it->begin(); server_loc_map_it != server_loc_it->end(); ++server_loc_map_it) {
        //         for (map_loc_val_it map = server_loc_map_it->second.begin(); map != server_loc_map_it->second.end(); ++map) {
        //                 std::cout << map->first << " : " << std::endl;
        //             for (fin_val last_val = map->second.begin(); last_val != map->second.end(); ++last_val) {
        //                 std::cout << "           " << *last_val << std::endl;
        //             }
        //         }
        //     }
        // }
    }
}

// void server_location(Data &g_Data) {
//     typedef std::vector<std::map<std::string,std::map<std::string, std::vector<std::string> > > >::iterator locations = g_Data.;

// }



void check_validity(Data &g_Data) {
    if (!g_Data.error.length()) {
        server_data(g_Data);
        // server_location(g_Data);

    }
    
}