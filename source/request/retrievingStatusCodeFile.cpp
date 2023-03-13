#include "../../include/request.hpp"
bool request::retrievingsatatuscodeFile(void)
{
    std::ifstream file;
    std::string buffer;
    std::vector<std::string> _split;

    file.open(STATUS_CODE_PATH, std::ifstream::in);
    if (!file.is_open())
        throw request::InternalServerError();
    while (std::getline(file, buffer))
    {
        _split = split(buffer, ": ");
        std::cout << " KEY :" << _split[1] << std::endl;
        std::cout << " VALUE :" << _split[0] << std::endl;
        
    }
    return true;
}