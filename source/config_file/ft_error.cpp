#include "../../include/WebServer.hpp"

void ft_error_server_bloc(Data &g_Data, KeyValue v, std::string filename, int line_index) {
    if (v.key == "{"){
        g_Data.error = "WebServer: [emerg] unexpected \"{\" in ";
        g_Data.error += filename + ':' + std::to_string(line_index);
    }
    else if (v.key != "server")
    {
        g_Data.error = "WebServer: [emerg] unknown directive \"";
        g_Data.error += v.key + "\" in " + filename + ':' + std::to_string(line_index);
    }
    else {
        g_Data.error = "WebServer: [emerg] directive \"server\" has no opening \"{\" in ";
        g_Data.error += filename + ':' + std::to_string(line_index);
    }
}

void ConfigFile::check_semicolon(Data &g_Data, KeyValue &v) {
    if (v.key != "location" && v.value.find_first_of(";") != (v.value.length() - 1)) {
        g_Data.error = "WebServer: [emerg] directive \"";
        g_Data.error += v.key + "\" is not terminated by \";\" in " + this->filename + ':' + std::to_string(this->line_index);              
    }
    else
    {
        trim(v.value, ";");
        // std::cout << v.value;
    }
}