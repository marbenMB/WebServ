#include "../../include/request.hpp"

int const &request::getRedirect_status(void) const
{
    return this->redirect_status;
}
void request::setRedirect_status(int redirect_status)
{
    this->redirect_status = redirect_status;
}
std::string const &request::getredirect_URL(void) const
{
    return (this->redirect_URL);
}
void request::setredirect_URL(std::string redirect_URL)
{
    this->redirect_URL = redirect_URL;
}