#include "../../include/WebServer.hpp"

void ft_error_server_bloc(Data &g_Data, KeyValue v, std::string filename, int line_index) {
    if (v.key == "{"){
        g_Data.error = "WebServer: [emerg] unexpected \"{\" in ";
        g_Data.error += filename + ':' + ft_to_string(line_index);
    }
    else if (v.key != "server")
    {
        g_Data.error = "WebServer: [emerg] unknown directive \"";
        g_Data.error += v.key + "\" in " + filename + ':' + ft_to_string(line_index);
    }
    else {
        g_Data.error = "WebServer: [emerg] directive \"server\" has no opening \"{\" in ";
        g_Data.error += filename + ':' + ft_to_string(line_index);
    }
}

void ConfigFile::check_semicolon(Data &g_Data, KeyValue &v) {
    if (v.key != "location" && v.value.find_first_of(";") != (v.value.length() - 1)) {
        g_Data.error = "WebServer: [emerg] directive \"";
        g_Data.error += v.key + "\" is not terminated by \";\" in " + this->filename + ':' + ft_to_string(this->line_index);              
    }
    else
        trim(v.value, ";");
}

int ConfigFile::valid_error_page(Data &g_Data, std::string error) {
    int number = std::atoi(error.c_str());
    if (number >= 300 && number <= 599)
        return (1);
    g_Data.error = "WebServer: [emerg] value \"" + error + "\" must be between 300 and 599 in ";
    g_Data.error += this->filename + ':' + ft_to_string(this->line_index);
    return (0);
}

int ConfigFile::valid_return_status(Data &g_Data, std::string status) {
    int number = std::atoi(status.c_str());
    if (number >= 300 && number <= 399)
        return (1);
    g_Data.error = "WebServer: [emerg] invalid return code \"" + status + "\" in ";
    g_Data.error += this->filename;
    return (0);
}

void ConfigFile::check_allow_method(Data &g_Data, map_vector_it location_data_it) {
    for (std::vector<std::string>::iterator value_it = location_data_it->second.begin(); value_it != location_data_it->second.end(); ++value_it) {
        if (*value_it == "GET" || *value_it == "POST" || *value_it == "DELETE")
            continue;
        else {
            g_Data.error = "WebServer: [emerg] invalid arguments \"" + *value_it + "\" in allow_methods in " + this->filename;
        }
    }
}

void ConfigFile::check_one_arg(Data &g_Data, map_vector_it location_data_it) {
    if (!Valid_vector_size(location_data_it->second, 1)) {
        g_Data.error = "WebServer: [emerg] invalid number of arguments in \"" + location_data_it->first;
        g_Data.error += "\" in " + this->filename;
    }
}

void ConfigFile::check_return_location(Data &g_Data, map_vector_it location_data_it) {
    int index = 0;
    if (!Valid_vector_return_size(location_data_it->second)) {
        g_Data.error = "WebServer: [emerg] invalid number of arguments in \"" + location_data_it->first;
        g_Data.error += "\" in " + this->filename;
    }
    else {
        for (std::vector<std::string>::iterator value_it = location_data_it->second.begin(); value_it != location_data_it->second.end(); ++value_it) {
            index++;
            if (index % 2) {
                if (!valid_return_status(g_Data, *value_it)) {
                    g_Data.error = "WebServer: [emerg] invalid return code \"" + *value_it;
                    g_Data.error += "\" in " + this->filename;
                }
            }
        }
    }
}

void ConfigFile::check_autoindex_location(Data &g_Data, map_vector_it location_data_it) {
    if (!Valid_vector_size(location_data_it->second, 1)) {
        g_Data.error = "WebServer: [emerg] invalid number of arguments in \"" + location_data_it->first;
        g_Data.error += "\" in " + this->filename;
    }
    else {
        if (*(location_data_it->second.begin()) != "on" && *(location_data_it->second.begin()) != "off") {
            g_Data.error = "WebServer: [emerg] invalid value \"" + *(location_data_it->second.begin()) + "\" in \"autoindex\" directive, it must be \"on\" or \"off\" in ";
            g_Data.error += this->filename;
        }
    }
}
