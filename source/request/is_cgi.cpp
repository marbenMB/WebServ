#include "../../include/request.hpp"
bool Is_cgi(std::string str)
{
    size_t pos = 0;
    if (((pos = str.rfind(".py")) != std::string::npos && (pos + 3) == str.length()) || ((pos = str.rfind(".go")) != std::string::npos && (pos + 3) == str.length()))
        return true;
    return false;
}