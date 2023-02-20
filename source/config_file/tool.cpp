#include "../../include/WebServer.hpp"


void print_Data(std::vector<ServerConf> server_list) {

    for (std::vector<ServerConf>::iterator it = server_list.begin(); it != server_list.end(); ++it) {
        for (std::map<std::string, std::vector<std::string> >::iterator server_data_it = it->server_data.begin(); server_data_it != it->server_data.end(); ++server_data_it) {
            for (std::vector<std::string>::iterator value_it = server_data_it->second.begin(); value_it != server_data_it->second.end(); ++value_it) {
                std::cout << server_data_it->first << ": " << *value_it << std::endl;
            }
        }
    for (std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator locations_it = it->locations.begin(); locations_it != it->locations.end(); ++locations_it) {
        for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_data_it = locations_it->begin(); location_data_it != locations_it->end(); ++location_data_it) {
            std::cout << "Location: " << location_data_it->first << std::endl;
            for (std::map<std::string, std::vector<std::string> >::iterator location_data_it2 = location_data_it->second.begin(); location_data_it2 != location_data_it->second.end(); ++location_data_it2) {
                // std::cout << location_data_it2->first << ": ";
                for (std::vector<std::string>::iterator value_it2 = location_data_it2->second.begin(); value_it2 != location_data_it2->second.end(); ++value_it2) {
                    std::cout << location_data_it2->first << ": " << *value_it2 << std::endl;
                }
            }
        }
    }
}
}