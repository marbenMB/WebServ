#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#define BLUE "\x1b[36m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define MAUVE "\x1b[33m"
#define END_CLR "\033[0m"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stack>
#include <netdb.h>
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>
#include "classes.hpp"
#include "ConfigFile.hpp"

///////////////////// Parent class ////////////////////

class Data {
    public:
        Data() {};
        std::vector<ServerConf> server_list;
        ConfigFile configuration;
        std::string error;
};

#endif