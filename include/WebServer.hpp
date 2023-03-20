#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#define BLUE "\x1b[36m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define MAUVE "\x1b[33m"
#define END_CLR "\033[0m"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <stack>
#include <libc.h>
#include <netdb.h>
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>
#include "classes.hpp"
#include "ConfigFile.hpp"
#include "ft_tools.hpp"


///////////////// config macros ////////

#define WHITE_SPACE " \t"
#define SPACE ' '
#define SEMI_COL ";"
#define IP_NUM "0123456789:."
#define NUM "0123456789"
#define SIZE_TYPE "MKGBmkgb"
#define GO_PATH "/Users/aboulhaj/.brew/bin/go"
#define HOME_PATH "/Users/aboulhaj"

///////////////////////////////////////

///////////////////// Parent class ////////////////////

class Data {
    public:
        Data() {};
        Vect_Serv server_list;
        ConfigFile configuration;
        std::string error;
};

#endif