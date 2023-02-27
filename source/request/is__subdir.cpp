#include "../../include/request.hpp"

bool is__subDir(const std::string Dir, const std::string subDir)
{
    std::string tmp;
    bool status;
    status = false;
    tmp.append(Dir);
    tmp.append("/");
    status = (tmp.find(subDir) != std::string::npos) ? true : false;
    return status;
}