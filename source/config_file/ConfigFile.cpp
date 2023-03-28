#include "../../include/WebServer.hpp"

void ConfigFile::parcing_file(std::string confilename, Data &g_Data){
    std::string sufx = ".conf";
    this->_in_file.open(confilename.c_str());
    if (this->_in_file.is_open()){
        if(confilename.rfind(sufx) != std::string::npos && confilename.rfind(sufx) == (confilename.length() - sufx.length())) {
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

bool ConfigFile::ckeck_brackets(Data &g_Data) {
    std::string line;
    int line_index = 0;
    std::stack<char> brace_stack;

    while (std::getline(this->_in_file, line)) {
        line_index++;
        trim(line, WHITE_SPACE);
        if (line[0] == '#')
            continue;
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
        g_Data.error += this->filename + ':' + ft_to_string(line_index);
    }
    return brace_stack.empty();
}



void ConfigFile::server_block(Data &g_Data, KeyValue v) {
    ServerConf server;

    if (v.key == "server" && v.value == "{") {
        while (std::getline(this->_in_file, v.line) && !g_Data.error.length()) {
            this->line_index++;
            trim(v.line, WHITE_SPACE);
            if(v.line.length() == 0 || v.line[0] == '#')
                continue;
            if(v.line == "}")
                break;
            v.index = v.line.find(SPACE);
            if (v.index == -1) {
                g_Data.error = "WebServer: [emerg] invalid number of arguments in \"";
                g_Data.error += v.line + "\" directive in " + this->filename + ':' + ft_to_string(this->line_index);
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
    line_index = 0;

    if (ckeck_brackets(g_Data)) {
        while (std::getline(this->_in_file, v.line) && !g_Data.error.length()) {
            line_index++;
            trim(v.line, WHITE_SPACE);
            if (v.line.length() == 0 || v.line[0] == COMMENT)
                continue;
            v.index = v.line.find(SPACE);
            v.key = v.line.substr(0, v.index);
            if (v.index == -1) {
                std::getline(this->_in_file, v.line);
                line_index++;
                trim(v.line, WHITE_SPACE);
                if (v.line[0] == COMMENT)
                    continue;
                v.index = v.line.find_first_not_of(WHITE_SPACE);
                while(v.index == -1 && std::getline(this->_in_file, v.line)) {
                    line_index++;
                    trim(v.line, WHITE_SPACE);
                    if (v.line[0] == COMMENT)
                        continue;
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
    }
    else {
        g_Data.error = "WebServer: [emerg] error in syntax missing of '{' or '}' in configfile.conf";
    }
}