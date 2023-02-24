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
int const &request::getAutoIndex() const{
    return this->autoindex;
}
void request::setAutoIndex(int autoindex){
    this->autoindex = autoindex;
}

bool request::getrequirements( void ) const{
    return this->requirements;
}
std::string const & request::getDefault_40x( void ){
    return this->default_40x;
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


int request::findLocation(std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location){
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator locations_iterator = location.begin();
    int done = -1;
    int index = 0;
    std::string compare_URI;
    compare_URI.clear();
    compare_URI.append(this->request_URI);
    while (locations_iterator != location.end())
    {
        // ***> Create iterator for location Data
        std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_iterator = locations_iterator->begin();
        // std::cout << "  +>" << location_iterator->first << "<| >compare_URI :" << compare_URI<<"<|" << std::endl;
        std::string str = location_iterator->first;
        if (str.compare(compare_URI) == 0){done = index;}
        ++locations_iterator;
        index++;
        if (locations_iterator == location.end() && done == -1){
            size_t start = compare_URI.find_last_of("/");
            // std::cout << "  #|>" << start <<std::endl;
            if (start != std::string::npos && compare_URI.compare("/") != 0){
                locations_iterator = location.begin();
                index = 0; //93,116,772
                if (!start){start += 1;}// for the last one
                compare_URI.erase(start, compare_URI.length() - 1);
            }
        }
    }
    return done;
}



std::vector<std::pair<std::string, std::string> > const & request::getReqBody( void ) const
{
    return this->req_body;
}

void request::setrequest_URI(std::string uri){
    this->request_URI = uri;
}