#ifndef FT_TOOLS_HPP
#define FT_TOOLS_HPP

#include "WebServer.hpp"
#include <iostream>
#include <sstream>

char* ft_join(char *str1, const char *str2);
int Valid_vector_size( std::vector<std::string> vec, size_t size);
bool is_char_in_str(std::string str, char c);
void convert_body_size(std::string &size, std::string number, char type);
std::vector<std::string> get_server_data(Data &g_Data, std::string key);
void print_Data(Vect_Serv server_list);
void trim(std::string& string, std::string value);
int Valid_vector_return_size(std::vector<std::string> vec);

#endif