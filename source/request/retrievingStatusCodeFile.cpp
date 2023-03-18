#include "../../include/request.hpp"
bool request::retrievingsatatuscodeFile(void)
{
    std::ifstream file;
    std::string buffer;
    std::vector<std::string> _split;

    file.open(STATUS_CODE_PATH, std::ifstream::in);
    if (!file.is_open())
        throw _Exception(INTERNAL_SERVER_ERROR);
    while (std::getline(file, buffer))
    {
        _split = split(buffer, ": ");
        // std::cout << _split[0] << " :" << _split[1] << std::endl;
        this->_statusCode[_split[0]] = _split[1];
    }
    return true;
}