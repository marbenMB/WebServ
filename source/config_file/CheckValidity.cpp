#include "../../include/WebServer.hpp"

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

void ConfigFile::valid_body_size(Data &g_Data, map_srv_data_it server_data_it) {
    char type;
    int index;
    std::string number;

    if (Valid_vector_size(server_data_it->second, 1)) {
        std::vector<std::string>::iterator value_it = server_data_it->second.begin();
            type = (*value_it)[(*value_it).length() - 1];
            if (is_char_in_str(SIZE_TYPE, type)) {
                number = *value_it;
                number.erase((*value_it).length() - 1, 1);
                index = number.find_first_not_of("0123456789");
                if (index != -1) {
                    g_Data.error = "WebServer: [emerg] \"" + server_data_it->first + "\" directive invalid value in ";
                    g_Data.error += this->filename;
                }
                else
                    convert_body_size(*value_it, number, type);
            }
            else {
                g_Data.error = "WebServer: [emerg] \"" + server_data_it->first + "\" directive invalid value in ";
                g_Data.error += this->filename;
            }
    }
    else {
        g_Data.error = "WebServer: [emerg] invalid number of arguments in \"" + server_data_it->first;
        g_Data.error += "\" in " + this->filename;
    }
}

void ConfigFile::server_location(Data &g_Data) {

    Vect_Serv &server_list = g_Data.server_list;

    for (Vect_Serv::iterator it = server_list.begin(); it != server_list.end()  && !g_Data.error.length(); ++it) {
        for (vector_location_it locations_it = it->locations.begin(); locations_it != it->locations.end(); ++locations_it) {
            for (map_path_location_it location_path_it = locations_it->begin(); location_path_it != locations_it->end(); ++location_path_it) {
                for (map_location_it location_data_it = location_path_it->second.begin(); location_data_it != location_path_it->second.end(); ++location_data_it) {
                    if (location_data_it->first == "allow_methods")
                        check_allow_method(g_Data, location_data_it);
                    else if (location_data_it->first == "root")
                        check_root_location(g_Data, location_data_it);
                    else if (location_data_it->first == "return")
                        check_return_location(g_Data, location_data_it);
                    else if (location_data_it->first == "autoindex")
                        check_autoindex_location(g_Data, location_data_it);
                }
            }
        }
    }
}

void ConfigFile::server_data(Data &g_Data) {
    std::string   root_path;
    std::string   number;
    
  
    Vect_Serv &server_list = g_Data.server_list; 
    


    for (Vect_Serv::iterator it = server_list.begin(); it != server_list.end()  && !g_Data.error.length(); ++it) {
        for (map_srv_data_it server_data_it = it->server_data.begin(); server_data_it != it->server_data.end()  && !g_Data.error.length(); ++server_data_it) {
            for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end() && !g_Data.error.length(); ++value_it) {
                if (server_data_it->first == "listen") 
                    valid_listen(g_Data, (*value_it));
                else if (server_data_it->first == "client_max_body_size")
                    valid_body_size(g_Data, server_data_it);
            }
        }
    }
}

void ConfigFile::check_validity(Data &g_Data) {
    if (!g_Data.error.length()) {
        server_data(g_Data);
        server_location(g_Data);

    }
    
}