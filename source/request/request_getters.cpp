#include "../../include/request.hpp"
std::string const &request::gethost() const
{
    return (this->host);
}
std::string const &request::getroot() const
{
    return (this->root);
}
std::string const &request::getdefaultIndex() const
{
    return (this->default_index);
}
std::string const &request::getrequest_URI() const
{
    return (this->request_URI);
}
std::string const &request::gethttp_version() const { return (this->http_version); }
std::vector<std::string> const &request::getRequestBody(void) const{return (this->requestBody);}
void request::setRequestBody(std::vector<std::string> reqBody)
{
    this->requestBody = reqBody;
}

std::string const &request::getContent_Type(void) const
{
    return (this->Content_Type);
}

std::string const &request::getTransfer_Encoding(void) const
{
    return (this->Transfer_Encoding);
}

int const &request::getContent_Length(void) const
{
    return (this->Content_Length);
}
int const &request::getsocketID(void) const
{
    return (this->socketID);
}

void request::setsocketID(int socketId)
{
    this->socketID = socketId;
}