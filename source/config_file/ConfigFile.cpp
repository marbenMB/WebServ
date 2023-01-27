#include "../../include/WebServer.hpp"

void ConfigFile::parcing_file(std::string confilename, Data &g_Data){
    std::string sufx = ".conf";
    this->_in_file.open(confilename.c_str());
    if (this->_in_file.is_open()){
        if(confilename.rfind(sufx) == confilename.length() - sufx.length())
            getdata(g_Data);
        else
            g_Data.error = "error in config file";
    }
    else
        g_Data.error = "error in config file";
}

ConfigFile::~ConfigFile() {
    this->_in_file.close();
}


//////////////////////////////////////////////////////

void trim(std::string& string, std::string value) {
	size_t first = string.find_first_not_of(value);
	string.erase(0, first);
	size_t last = string.find_last_not_of(value);
	size_t len = string.length() - (last - 1);
	string.erase(last + 1, len);
}


bool ConfigFile::check_braces() {
    std::string line;
    std::stack<char> brace_stack;

    while (std::getline(this->_in_file, line)) {
        for (unsigned int i = 0; i < line.length(); i++) {
            if (line[i] == '{') {
                brace_stack.push(line[i]);
            } else if (line[i] == '}') {
                if (brace_stack.empty() || brace_stack.top() != '{') {
                    return false;
                }
                brace_stack.pop();
            }
        }
    }
    this->_in_file.clear();
    this->_in_file.seekg(0, std::ios::beg);
    return brace_stack.empty();
}

void key_value(ServerConf &server,Data &g_Data, KeyValue v, char spartor) {
    std::string variable;
    (void)g_Data;

    trim(v.value, " \t");
    while ((v.index = v.value.find(spartor)) != -1) {
        variable = v.value.substr(0, v.index);
        trim(variable, " \t");
        server.server_data[v.key].push_back(variable);
        v.value.erase(0, v.index + 1);
    }
    trim(v.value, " \t;");
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
            trim(v.line, " \t'[]");
            if(v.line.length() == 0)
                continue;
            else {
                v.value = v.line;
                break;
            }
        }
    }
    if (path.length() && v.value == "{") {
        while (std::getline(this->_in_file, v.line))
        {
            trim(v.line, " \t:;'[]");
            if (v.line == "}")
                break;
            v.index = v.line.find(" ");
            
            if(v.index == -1) {
                v.key = v.line;
                v.value = "NaN";
                trim(v.key, " \t:'[]");
                location_var[v.key].push_back(v.value);
            }
            else {
                v.key = v.line.substr(0, v.index);
                v.line.erase(0, v.index + 1);
                trim(v.line, " \t:'[]");
                while ((v.index = v.line.find(" ")) != -1) {
                    variable = v.line.substr(0, v.index);
                    trim(variable, " \t:'[]");
                    location_var[v.key].push_back(variable);
                    v.line.erase(0, v.index + 1);
                    trim(v.line, " \t:'[]");
                }
                v.value = v.line;
                location_var[v.key].push_back(v.value);
            }
        }
        location[path] = location_var;
        server.locations.push_back(location);
    }
    else {
        g_Data.error = "syntax error";
    }


}

/////////////////////////////////////////////////////

void ConfigFile::server_block(Data &g_Data, KeyValue v) {
    ServerConf server;

    if (v.key == "server" && v.value == "{") {
        while (std::getline(this->_in_file, v.line) && !g_Data.error.length()) {
            trim(v.line, " \t");
            if(v.line.length() == 0)
                continue;
            if(v.line == "}")
                break;
            v.index = v.line.find(" ");
            if (v.index == -1) {
                std::cout << "-1 " << v.line << std::endl;
                continue;
            }
            v.key = v.line.substr(0, v.index);
            v.value = v.line.substr(v.index, v.line.length());
            if (v.key == "listen")
                key_value(server, g_Data, v, ',');
            else if (v.key == "host")
                key_value(server, g_Data, v, ' ');
            else if (v.key == "server_name")
                key_value(server, g_Data, v, ' ');
            else if (v.key == "root")
                key_value(server, g_Data, v, ' ');
            else if (v.key == "index")
                key_value(server, g_Data, v, ' ');
            else if (v.key == "error_page")
                key_value(server, g_Data, v, ' ');
            else if (v.key == "location")
                location(g_Data, server, v);
            else if (v.key == "client_max_body_size")
                key_value(server, g_Data, v, ' ');
            else {
                g_Data.error = "error in config file";
            }
        }
        
        if (!g_Data.error.length())
            g_Data.server_list.push_back(server);
    }
    else
    {
        g_Data.error = "error : server scope not found";
    }
}

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

void ConfigFile::getdata(Data &g_Data) {
    KeyValue v;

    if (check_braces())
    {
        while (std::getline(this->_in_file, v.line) && !g_Data.error.length()) {
            trim(v.line, " \t");
            if (v.line.length() == 0)
                continue;
            v.index = v.line.find(" ");
            v.key = v.line.substr(0, v.index);
            if (v.index == -1) {
                std::getline(this->_in_file, v.line);
                v.index = v.line.find_first_not_of(" \t");
                while(v.index == -1) {
                    std::getline(this->_in_file, v.line);
                    v.index = v.line.find_first_not_of(" \t");
                }
                v.value = v.line;
            }
            else
                v.value = v.line.substr(v.index, v.line.length() - 1);
            trim(v.value, " \t");
            server_block(g_Data, v);
        }
        check_validity(g_Data);
        // print_Data(g_Data.server_list);
    }
    else
        std::cout << "file not close" << std::endl;

}