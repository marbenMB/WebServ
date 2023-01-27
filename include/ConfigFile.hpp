#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "WebServer.hpp"
#include "classes.hpp"

class Data;
class KeyValue;
class ServerConf;

class ConfigFile {
    private:
        std::ifstream	_in_file;
    public:
        ConfigFile() {};
        ~ConfigFile();
        void parcing_file(std::string confilename, Data &g_Data);
        void getdata(Data &g_Data);
        bool check_braces();
        void server_block(Data &g_Data, KeyValue v);
        void location(Data &g_Data, ServerConf &server, KeyValue v);
};

void trim(std::string& string, std::string value);
void check_validity(Data &g_Data);


#endif