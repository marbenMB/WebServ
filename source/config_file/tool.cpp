#include "../../include/ft_tools.hpp"

char* ft_join(char *str1, const char *str2) {
    strcat(str1, str2);
    return str1;
}

void trim(std::string& string, std::string value) {
	size_t first = string.find_first_not_of(value);
	string.erase(0, first);
	size_t last = string.find_last_not_of(value);
	size_t len = string.length() - (last - 1);
	string.erase(last + 1, len);
}

int Valid_vector_size( std::vector<std::string> vec, size_t check) {
    if (vec.size() > check || vec.size() == 0)
        return (0);
    return (1);
}

int Valid_vector_return_size(std::vector<std::string> vec) {
    if ((vec.size() % 2) != 0 || (vec.size() == 0))
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
        s_num << (num * 1024 * 1024 * 1024);
    else if (type == 'M' || type == 'm')
        s_num << (num * 1024 * 1024);
    else if (type == 'K' || type == 'k')
        s_num << (num * 1024);
    else if (type == 'B' || type == 'b')
        s_num << num;
    size = s_num.str();
}

std::vector<std::string> get_server_data(Data &g_Data, std::string key) {
    std::vector<std::string> V_ret;

    for (Vect_Serv::iterator it = g_Data.server_list.begin(); it != g_Data.server_list.end(); ++it) {
        for (map_vector_it S_data_it = it->server_data.begin(); S_data_it != it->server_data.end(); ++S_data_it) {
            if (S_data_it->first == key)
                V_ret = S_data_it->second;
        } 
    }
    return (V_ret);
}

void print_Data(Vect_Serv server_list) {

    for (Vect_Serv::iterator it = server_list.begin(); it != server_list.end(); ++it) {
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