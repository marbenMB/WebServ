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
        std::string     filename;
        int             line_index;
    public:
        ConfigFile() {};
        ~ConfigFile();
        void parcing_file(std::string confilename, Data &g_Data);
        void getdata(Data &g_Data);
        bool check_braces(Data &g_Data);
        void server_block(Data &g_Data, KeyValue v);
        void location(Data &g_Data, ServerConf &server, KeyValue v);
        void fill_vector_variable(Data &g_Data, ServerConf &server, KeyValue v);
        void check_semicolon(Data &g_Data, KeyValue &v);
        void server_data(Data &g_Data);
        void check_validity(Data &g_Data);
        void key_value(ServerConf &server,Data &g_Data, KeyValue v);
        void key_value_error(ServerConf &server,Data &g_Data, KeyValue v);
        int  valid_error(Data &g_Data, std::string error);
        void valid_listen(Data &g_Data, std::string value);
        void valid_body_size(Data &g_Data, std::map<std::string, std::vector<std::string> >::iterator server_data_it);
};

void trim(std::string& string, std::string value);


void ft_error_server_bloc(Data &g_Data, KeyValue v, std::string filename, int line_index);


void print_Data(std::vector<ServerConf> server_list);

#endif