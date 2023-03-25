#include "../../include/request.hpp"

std::string request::getCGIbody( void ) const{
    return this->CGIbody;
}

std::string const &request::getroot() const
{
    
    return (this->root);
}
std::string const &request::getdefaultIndex() const
{
    return (this->default_index);
}

std::string request::getFastcgiPass( void ) const{
    return this->fastcgi_pass;
}

void request::setRequestBody(std::vector<std::string> reqBody)
{
    this->requestBody = reqBody;
}

int const &request::getAutoIndex() const{
    return this->autoindex;
}
void request::setAutoIndex(int autoindex){
    this->autoindex = autoindex;
}


std::string const & request::getDefault_40x( void ){
    return this->default_40x;
}

std::string const & request::getDefault_30x( void ){
    return this->default_30x;
}
std::string const & request::getDefault_50x( void ){
    return this->default_50x;
}
int request::getAllowedPost() const{
    return this->__post;
}
int request::getAllowedDelete() const{
    return this->__delete;
}
int request::getAllowedGet() const{
    return this->__get;
}

std::vector<std::pair<std::string, std::string> > const & request::getReqBody( void ) const
{
    return this->req_body;
}
std::string const & request::getcompare_URI( void ) const{
    return this->compare_URI;
}
void request::setcompare_URI(std::string compare_URI){
    this->compare_URI = compare_URI;
}
void request::addType(std::string key, std::string value){
    this->_typs[key] = value;
}
std::string const &request::getType(std::string key){
    return this->_typs[key];
}

bool request::getIs_cgi( void ){
    return this->is_cgi;
}

void request::setIs_cgi( int index ){
    this->is_cgi = index;
}
std::string const & request::getUpload_store_PATH( void ) const{
    return this->upload_store;
}
bool request::getCGIstatus( void ) const{
    return this->is_cgi;
}
std::string const &request::getReason(std::string key){
    return this->_statusCode[key];
}

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