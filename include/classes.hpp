#ifndef CLASSES_HPP
#define CLASSES_HPP

#include "ConfigFile.hpp"

class KeyValue
{
public:
    std::string line;
    std::string key;
    std::string value;
    int index;
};

class ServerConf
{
public:
    std::map<std::string, std::vector<std::string> > server_data;
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > locations;
};

#endif