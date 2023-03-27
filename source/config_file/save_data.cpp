#include "../../include/WebServer.hpp"

void ConfigFile::key_value(ServerConf &server, KeyValue v) {

    std::string variable;

    trim(v.value, " \t");
    while ((v.index = v.value.find(' ')) != -1) {
        variable = v.value.substr(0, v.index);
        trim(variable, " \t");
        if (variable.length())
            server.server_data[v.key].push_back(variable);
        v.value.erase(0, v.index + 1);
    }
    trim(v.value, " \t");
    if (v.value.length())
        server.server_data[v.key].push_back(v.value);
}

void ConfigFile::key_value_error_page(ServerConf &server,Data &g_Data, KeyValue v) {

    std::string variable;
    int find_error = 0;

    trim(v.value, " \t");
    while ((v.index = v.value.find(' ')) != -1) {
        find_error = 1;
        variable = v.value.substr(0, v.index);
        trim(variable, " \t");
        if (variable.length() && (variable.find_first_not_of(NUM) == std::string::npos) && valid_error_page(g_Data, variable))
            server.server_data[v.key].push_back(variable);
        else if (!g_Data.error.length()){
            g_Data.error = "WebServer: [emerg] invalid value \"" + variable + "\" in ";
            g_Data.error += this->filename + ':' + ft_to_string(this->line_index);
        }
        v.value.erase(0, v.index + 1);
    }
    trim(v.value, " \t;");
    if (!v.value.length() || !find_error) {
        g_Data.error = "WebServer: [emerg] invalid number of arguments in \"";
        g_Data.error += v.key + "\" directive in " + this->filename + ':' + ft_to_string(this->line_index);
    }
    else if (v.value.find(".html") == std::string::npos) {
        g_Data.error = "WebServer: [emerg] " + v.value + " is not a error file ('file.html') in \"";
        g_Data.error += this->filename + ':' + ft_to_string(this->line_index);
    }
    else
        server.server_data[v.key].push_back(v.value);
}

void ConfigFile::location(Data &g_Data, ServerConf &server, KeyValue v)
{
    std::map<std::string, std::vector<std::string> >                        location_var;
    std::map<std::string,std::map<std::string, std::vector<std::string> > > location;
    std::string                                                             variable;
    std::string                                                             path;

    trim(v.value, " \t'[]");
    if (v.value[v.value.length() - 1] == '{') {
        path = v.value.substr(0, v.value.length() - 1);
        trim(path, " \t'[]");
        v.value = "{";
    }
    else {
        path = v.value;
        while (std::getline(this->_in_file, v.line)) {
            this->line_index++;
            trim(v.line, " \t'[]");
            if(v.line.length() == 0 || v.line[0] == '#')
                continue;
            else {
                v.value = v.line;
                break;
            }
        }
    }
    if (path.length() && v.value == "{") {
        while (std::getline(this->_in_file, v.line)){
            this->line_index++;
            trim(v.line, " \t:'[]");
            if(v.line.length() == 0 || v.line[0] == '#')
                continue;
            if (v.line == "}")
                break;
            v.index = v.line.find(" ");
            if(v.index == -1) {
                g_Data.error = "WebServer: [emerg] invalida number of arguments in \"";
                g_Data.error += v.line + "\" directive in " + this->filename + ':' + ft_to_string(this->line_index);
            }
            else {
                v.key = v.line.substr(0, v.index);
                v.line.erase(0, v.index + 1);
                trim(v.line, " \t:'[]");
                v.value = v.line;
                check_semicolon(g_Data, v);
                trim(v.value, " \t:'[] ;");
                v.line = v.value;
                while ((v.index = v.line.find(" ")) != -1) {
                    v.value = v.line.substr(0, v.index);
                    trim(v.value, " \t:',[]");
                    location_var[v.key].push_back(v.value);
                    v.line.erase(0, v.index + 1);
                    trim(v.line, " \t:',[]");
                }
                trim(v.line, " \t:',[]");
                v.value = v.line;
                location_var[v.key].push_back(v.value);
            }
        }
        location[path] = location_var;
        server.locations.push_back(location);
    }
    else {
        g_Data.error = "syntax error in the location bloc in";
        g_Data.error += this->filename + ':' + ft_to_string(this->line_index);
    }


}

void ConfigFile::fill_vector_variable(Data &g_Data, ServerConf &server, KeyValue v) {
    if (v.key == "listen")
        key_value(server, v);
    else if (v.key == "server_name")
        key_value(server, v);
    else if (v.key == "root")
        key_value(server, v);
    else if (v.key == "index")
        key_value(server, v);
    else if (v.key == "error_page")
        key_value_error_page(server, g_Data, v);
    else if (v.key == "location")
        location(g_Data, server, v);
    else if (v.key == "client_max_body_size")
        key_value(server, v);
    else if (v.key == "server")
    {
        g_Data.error = "WebServer: [emerg] unexpected end of file, expecting\"}\" in ";
        g_Data.error += this->filename + ':' + ft_to_string(this->line_index);
    }
    else {
        g_Data.error = "WebServer: [emerg] unknown directive \"";
        g_Data.error += v.key + "\" in " + this->filename + ':' + ft_to_string(this->line_index);
    }
}
