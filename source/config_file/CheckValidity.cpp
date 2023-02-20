#include "../../include/WebServer.hpp"
#include <iostream>
#include <sstream>


char* ft_join(char *str1, const char *str2) {
    strcat(str1, str2);
    return str1;
}

int Valid_vector_size( std::vector<std::string> vec) {
    if (vec.size() > 1)
        return (0);
    return (1);
}

bool is_char_in_str(std::string str, char c) {
    return (str.find(c) != std::string::npos);
}

void convert_body_size(std::string &size, std::string number, char type) {
    
    std::ostringstream s_num;
    long num = std::atoi(number.c_str());

    if (type == 'G' || type == 'g')
        s_num << (num * 1024 * 1024);
    else if (type == 'K' || type == 'k')
        s_num << num;
    else if (type == 'M' || type == 'm')
        s_num << (num * 1024);
    size = s_num.str();
}

std::vector<std::string> get_server_data(Data &g_Data, std::string key) {
    std::vector<std::string> V_ret;

    typedef std::map<std::string, std::vector<std::string> >::iterator srv_data_it;
    for (std::vector<ServerConf>::iterator it = g_Data.server_list.begin(); it != g_Data.server_list.end(); ++it) {
        for (srv_data_it S_data_it = it->server_data.begin(); S_data_it != it->server_data.end(); ++S_data_it) {
            if (S_data_it->first == key)
                V_ret = S_data_it->second;
        } 
    }
    return (V_ret);
}

void ConfigFile::valid_listen(Data &g_Data, std::string value) {
    struct addrinfo hints ,*res;
    std::string     port, host;
    int             status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    status = value.find_first_not_of(IP_NUM);
    if (status != -1) {
        g_Data.error = "WebServer: [emerg] host not found in \"";
        g_Data.error += value + "\" of the \"listen\" directive in " + this->filename;
    }
    else if ((status = value.find(":")) != -1) {
        host = value.substr(0, value.find(":"));
        port = value.substr(value.find(":") + 1, value.length() - 1);
        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
            g_Data.error = "WebServer: [emerg] host not found in \"" + value + "\" of the \"listen\" directive in ";
            g_Data.error += this->filename;
        }
    }
    else if ((status = value.find(".")) != -1) {
        if (getaddrinfo(value.c_str(), NULL, &hints, &res) != 0) {
            g_Data.error = "WebServer: [emerg] host not found in \"" + value + "\" of the \"listen\" directive in ";
            g_Data.error += this->filename;
        }

    }
    else
        if (getaddrinfo(NULL, value.c_str(), &hints, &res) != 0) {
            g_Data.error = "WebServer: [emerg] host not found in \"" + value + "\" of the \"listen\" directive in ";
            g_Data.error += this->filename;
        }

}

void ConfigFile::valid_body_size(Data &g_Data, std::map<std::string, std::vector<std::string> >::iterator server_data_it) {
    char type;
    int index;
    std::string number;

    if (Valid_vector_size(server_data_it->second)) {
        std::vector<std::string>::iterator value_it = server_data_it->second.begin();
            type = (*value_it)[(*value_it).length() - 1];
            if (is_char_in_str(SIZE_TYPE, type)) {
                number = *value_it;
                number.erase((*value_it).length() - 1, 1);
                index = number.find_first_not_of("0123456789");
                if (index != -1) {
                    g_Data.error = "WebServer: [emerg] \"" + server_data_it->first + "\" directive invalid value in ";
                    g_Data.error += this->filename;
                    // break;
                }
                else
                    convert_body_size(*value_it, number, type);
            }
            else {
                g_Data.error = "WebServer: [emerg] \"" + server_data_it->first + "\" directive invalid value in ";
                g_Data.error += this->filename;
                // break;
            }
    }
    else {
        g_Data.error = "WebServer: [emerg] invalid number of arguments in \"" + server_data_it->first;
        g_Data.error += "\" in " + this->filename;
    }
}

void ConfigFile::server_data(Data &g_Data) {
    typedef std::map<std::string, std::vector<std::string> >::iterator srv_data_it;
    // typedef std::vector<std::map<std::string,std::map<std::string, std::vector<std::string> > > >::iterator ser_loc_it;
    // typedef std::map<std::string,std::map<std::string, std::vector<std::string> > >::iterator ser_loc_map_it;
    // typedef std::map<std::string, std::vector<std::string> >::iterator map_loc_val_it;
    // typedef std::vector<std::string>::iterator fin_val;
    // int                                                                status;
    // struct stat                                                        sb;
    std::string                                                        root_path;
    // int                                                               check_error_page = 0;
    // char                                                              type;
    std::string                                                       number;
    // int                                                               index;
    

    std::vector<ServerConf> server_list = g_Data.server_list; 
    

    // if (get_server_data(g_Data, "server_name").begin() != get_server_data(g_Data, "server_name").end()) {
    //     for (std::vector<std::string>::iterator value_it = get_server_data(g_Data, "server_name").begin(); value_it != get_server_data(g_Data, "server_name").end(); ++value_it) {
    //         std::cout << "** "<< *value_it << std::endl;
    //     }
    // }
    // else
    

    // for (std::vector<ServerConf>::iterator it = server_list.begin(); it != server_list.end(); ++it) {
    //     for (srv_data_it server_data_it = it->server_data.begin(); server_data_it != it->server_data.end(); ++server_data_it) {
    //         for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end(); ++value_it) {
    //             if (server_data_it->first == "root") {
    //                 if (stat((*value_it).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
    //                     root_path = *value_it;
    //                     if (root_path[root_path.length() - 1] != '/')
    //                         root_path += "/";
    //                 }
    //                 else {
    //                     g_Data.error = "error : The root value is not valide";
    //                     return ;
    //                 }
    //             }
    //         }
    //     }
    // }
    // get

    for (std::vector<ServerConf>::iterator it = server_list.begin(); it != server_list.end()  && !g_Data.error.length(); ++it) {
        for (srv_data_it server_data_it = it->server_data.begin(); server_data_it != it->server_data.end()  && !g_Data.error.length(); ++server_data_it) {
            for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end() && !g_Data.error.length(); ++value_it) {
                if (server_data_it->first == "listen") 
                    valid_listen(g_Data, (*value_it));
                else if (server_data_it->first == "client_max_body_size")
                    valid_body_size(g_Data, server_data_it);
                
                
                
                
                // else if (server_data_it->first == "root")
                //     continue;
                // else
                // {
                //     g_Data.error = server_data_it->first;
                //     g_Data.error += " teest";
                //     break;
                // }

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



void ConfigFile::check_validity(Data &g_Data) {
    if (!g_Data.error.length()) {
        server_data(g_Data);
        // server_location(g_Data);

    }
    
}