#include "../../include/WebServer.hpp"

void ConfigFile::parcing_file(std::string confilename, Data &g_Data){
    std::string sufx = ".conf";
    this->_in_file.open(confilename.c_str());
    if (this->_in_file.is_open()){
        if(confilename.rfind(sufx) == confilename.length() - sufx.length()) {
            this->filename = confilename;
            getdata(g_Data);
        }
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


bool ConfigFile::check_braces(Data &g_Data) {
    std::string line;
    int line_index = 0;
    std::stack<char> brace_stack;

    while (std::getline(this->_in_file, line)) {
        line_index++;
        for (unsigned int i = 0; i < line.length(); i++) {
            if (line[i] == '{') {
                brace_stack.push(line[i]);
            } else if (line[i] == '}') {
                if (brace_stack.empty()) {
                    
                    return false;
                }
                brace_stack.pop();
            }
        }
    }
    this->_in_file.clear();
    this->_in_file.seekg(0, std::ios::beg);
    if (!brace_stack.empty()) {
        g_Data.error = "WebServer: [emerg] unexpected end of file, expecting\"}\" in ";
        g_Data.error += this->filename + ':' + std::to_string(line_index);
    }
    return brace_stack.empty();
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
            if(v.line.length() == 0)
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

void ConfigFile::server_block(Data &g_Data, KeyValue v) {
    ServerConf server;

    if (v.key == "server" && v.value == "{") {
        while (std::getline(this->_in_file, v.line) && !g_Data.error.length()) {
            this->line_index++;
            trim(v.line, WHITE_SPACE);
            if(v.line.length() == 0)
                continue;
            if(v.line == "}")
                break;
            v.index = v.line.find(SPACE);
            if (v.index == -1) {
                g_Data.error = "WebServer: [emerg] invalid number of arguments in \"";
                g_Data.error += v.line + "\" directive in " + this->filename + ':' + std::to_string(this->line_index);
                break;
            }
            v.key = v.line.substr(0, v.index);
            v.value = v.line.substr(v.index, v.line.length());
            check_semicolon(g_Data, v);
            fill_vector_variable(g_Data, server, v);
        }
        if (!g_Data.error.length())
            g_Data.server_list.push_back(server);
    }
    else
        ft_error_server_bloc(g_Data, v, this->filename, this->line_index);
}




void ConfigFile::getdata(Data &g_Data) {
    KeyValue v;

    if (check_braces(g_Data)) {
        while (std::getline(this->_in_file, v.line) && !g_Data.error.length()) {
            line_index++;
            trim(v.line, WHITE_SPACE);
            if (v.line.length() == 0)
                continue;
            v.index = v.line.find(" ");
            v.key = v.line.substr(0, v.index);
            if (v.index == -1) {
                std::getline(this->_in_file, v.line);
                line_index++;
                v.index = v.line.find_first_not_of(WHITE_SPACE);
                while(v.index == -1) {
                    std::getline(this->_in_file, v.line);
                    line_index++;
                    v.index = v.line.find_first_not_of(WHITE_SPACE);
                }
                v.value = v.line;
            }
            else
                v.value = v.line.substr(v.index, v.line.length() - 1);
            trim(v.value, WHITE_SPACE);
            server_block(g_Data, v);
        }
        check_validity(g_Data);
        // print_Data(g_Data.server_list);
    }
}