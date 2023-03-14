#include "../../include/request.hpp"

bool is__subDir(const std::string Dir, const std::string subDir)
{
    
    bool status = true;

    char _Dir[PATH_MAX];
    char _subDir[PATH_MAX];

    realpath(Dir.c_str(), _Dir);
    realpath(subDir.c_str(), _subDir);

    // std::cout << "#DIR :" << _Dir << std::endl;
    // std::cout << "#SUBDIR :" << _subDir << std::endl;
    std::string tmp(_subDir);

    size_t pos = tmp.find(_Dir, 0);
    if (pos != 0){
        status = false;
        // std::cerr << "`"<<tmp<<"`Not sub Dir [P:"<< pos <<"]\n";
    }
    return status;
}