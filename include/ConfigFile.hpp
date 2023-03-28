#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "WebServer.hpp"
#include "classes.hpp"

class Data;
class KeyValue;
class ServerConf;

typedef std::map<std::string, std::vector<std::string> > map_vector;
typedef std::vector<ServerConf> Vect_Serv;

typedef std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator vector_location_it;
typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator map_path_location_it;
typedef std::map<std::string, std::vector<std::string> >::iterator map_vector_it;

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
        bool ckeck_brackets(Data &g_Data);
        void server_block(Data &g_Data, KeyValue v);
        void location(Data &g_Data, ServerConf &server, KeyValue v);
        void fill_vector_variable(Data &g_Data, ServerConf &server, KeyValue v);
        void check_semicolon(Data &g_Data, KeyValue &v);
        void server_data(Data &g_Data);
        void check_validity(Data &g_Data);
        void key_value(ServerConf &server, KeyValue v);
        void key_value_error_page(ServerConf &server,Data &g_Data, KeyValue v);
        int  valid_error_page(Data &g_Data, std::string error);
        void valid_listen(Data &g_Data, std::string value);
        void valid_body_size(Data &g_Data, map_vector_it server_data_it);
        void server_location(Data &g_Data);
        void check_allow_method(Data &g_Data, map_vector_it location_data_it);
        void check_one_arg(Data &g_Data, map_vector_it location_data_it);
        void check_return_location(Data &g_Data, map_vector_it location_data_it);
        int  valid_return_status(Data &g_Data, std::string status);
        void check_autoindex_location(Data &g_Data, map_vector_it location_data_it);
        void set_default(Data &g_Data);
};

void ft_error_server_bloc(Data &g_Data, KeyValue v, std::string filename, int line_index);


void print_Data(Vect_Serv server_list);

#endif