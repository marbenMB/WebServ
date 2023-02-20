#include "../../include/WebServer.hpp"

int ConfigFile::valid_error(Data &g_Data, std::string error) {
    int number = std::stoi(error);
    if (number >= 300 && number <= 599)
        return (1);
    g_Data.error = "WebServer: [emerg] value \"" + error + "\" must be between 300 and 599 in ";
    g_Data.error += this->filename + ':' + std::to_string(this->line_index);
    return (0);
}

void ConfigFile::key_value(ServerConf &server,Data &g_Data, KeyValue v) {

    std::string variable;
    (void) g_Data;

    trim(v.value, " \t");
    while ((v.index = v.value.find(' ')) != -1) {
        variable = v.value.substr(0, v.index);
        trim(variable, " \t");
        if (variable.length())
            server.server_data[v.key].push_back(variable);
        v.value.erase(0, v.index + 1);
    }
    trim(v.value, " \t");
    if (v.value.length()) {
        // g_Data.error = "WebServer: [emerg] invalid number of arguments in \"";
        // g_Data.error += v.key + "\" directive in " + this->filename + ':' + std::to_string(this->line_index);
        server.server_data[v.key].push_back(v.value);
    }
}

void ConfigFile::key_value_error(ServerConf &server,Data &g_Data, KeyValue v) {

    std::string variable;
    int find_error = 0;

    trim(v.value, " \t");
    while ((v.index = v.value.find(' ')) != -1) {
        find_error = 1;
        variable = v.value.substr(0, v.index);
        trim(variable, " \t");
        if (variable.length() && (variable.find_first_not_of(NUM) == std::string::npos) && valid_error(g_Data, variable))
            server.server_data[v.key].push_back(variable);
        else if (!g_Data.error.length()){
            g_Data.error = "WebServer: [emerg] invalid value \"" + variable + "\" in ";
            g_Data.error += this->filename + ':' + std::to_string(this->line_index);
        }
        v.value.erase(0, v.index + 1);
    }
    trim(v.value, " \t;");
    if (!v.value.length() || !find_error) {
        g_Data.error = "WebServer: [emerg] invalid number of arguments in \"";
        g_Data.error += v.key + "\" directive in " + this->filename + ':' + std::to_string(this->line_index);
    }
    else
        server.server_data[v.key].push_back(v.value);
}

void ConfigFile::fill_vector_variable(Data &g_Data, ServerConf &server, KeyValue v) {
    if (v.key == "listen")
        key_value(server, g_Data, v);
    else if (v.key == "server_name")
        key_value(server, g_Data, v);
    else if (v.key == "root")
        key_value(server, g_Data, v);
    else if (v.key == "index")
        key_value(server, g_Data, v);
    else if (v.key == "error_page")
        key_value_error(server, g_Data, v);
    else if (v.key == "location")
        location(g_Data, server, v);
    else if (v.key == "client_max_body_size")
        key_value(server, g_Data, v);
    else if (v.key == "server")
    {
        g_Data.error = "WebServer: [emerg] unexpected end of file, expecting\"}\" in ";
        g_Data.error += this->filename + ':' + std::to_string(this->line_index);
    }
    else {
        g_Data.error = "WebServer: [emerg] unknown directive \"";
        g_Data.error += v.key + "\" in " + this->filename + ':' + std::to_string(this->line_index);
    }
}
