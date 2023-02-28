#include "../../include/request.hpp"
#include <stdlib.h>
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


int request::findLocation(std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location){
    char _dir[PATH_MAX];
    // char* _subdir;
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator locations_iterator = location.begin();
    int locationId = -1;
    int index = 0;
    bool _cgi = false;
    std::string compare_URI;
    std::string str;
    compare_URI.clear();
    size_t pos = 0;
    // size_t pos = this->getrequest_URI().find_last_of(".py");
    // pos = this->getrequest_URI().find_last_of(".go");
    if ((pos = this->getrequest_URI().rfind(".py")) != std::string::npos && (pos + 3) == this->getrequest_URI().length())
    {
        _cgi = true;
        compare_URI.append("\%.py$");
    }
    else if ((pos = this->getrequest_URI().rfind(".go")) != std::string::npos && (pos + 3) == this->getrequest_URI().length())
    {
         _cgi = true;
        compare_URI.append("\%.go$");
    }
    else  
    {
        // compare_URI.append("/");
        // if (!this->request_URI.empty())
        compare_URI.append(this->request_URI);
    }
    // compare_URI.append(this->request_URI);
     //  * init string |> /srcs/dir001/dir0011/test.txt
    // std::cout << "++> URI :" << compare_URI << std::endl;

    while (locations_iterator != location.end())
    {
        // ***> Create iterator for location Data
        std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_iterator = locations_iterator->begin();
        // std::cout << "  +>" << location_iterator->first << "<| >compare_URI :" << compare_URI<<"<|" << std::endl;
        str.clear();
        str.append(location_iterator->first);
        // std::cout << "str : |" << str << "| compare_URI : |" << compare_URI <<"| " <<  std::endl;
        if (str.compare(compare_URI) == 0){locationId = index;break;}
        ++locations_iterator;
        index++;
        if (locations_iterator == location.end() && locationId == -1){  // if '/srcs/dir001/dir0011/test.txt' not found ;
            size_t start = compare_URI.find_last_of("/"); // find the last '/'
            // std::cout << "  #|>" << start <<std::endl;
            if (start != std::string::npos && compare_URI.compare("/") != 0){ // if the's  '/'
                locations_iterator = location.begin(); // init iterator to the begin
                index = 0; // init index count to the 0 for anther tour
                if (!start){start += 1;}// for the last one means request URI is  '/' ;becuse we should remove the last '/'
                compare_URI.erase(start, compare_URI.length() - 1); // now remove after ''/
            }
        }
    }
    if (!_cgi) // remove the mutch string 
    {
        std::cout << "locationId :" << locationId << std::endl;
        std::cout << "----URI1 (str) :" << str << std::endl;
        if (locationId != -1)
        {
            // std::cout << "----URI :" << this->getrequest_URI() << std::endl;
            // std::cout << "----URI2 (str) :" << str << std::endl;
            std::string __erraseTmp(this->getrequest_URI()) ;
            if (__erraseTmp.compare("/") != 0)
                __erraseTmp.erase(0,str.length());
            std::string __URI(this->getroot());
            __URI.append("/");
            __URI.append(__erraseTmp);
            realpath(__URI.c_str(), _dir);
            std::cout << "__erraseTmp : |" << _dir << std::endl; 
            this->setrequest_URI(_dir);

        }

        // std::string _tmp;
        // // std::cout << "----befor :" << this->getrequest_URI() << std::endl;
        // _tmp.append(this->getroot());
        // _tmp.append("/");
        // _tmp.append(this->getrequest_URI());
        // // realpath(_tmp.c_str(), _dir);
        // std::string __reqURI(_dir);
        // // if (realpath_status == NULL)
        // //     throw InternalServerError();
        // std::cout << "realpath :" << _dir << std::endl;
        // _tmp.clear();
        // _tmp.append(this->getroot());

        // _tmp.append(this->getrequest_URI().substr(str.length(), this->getrequest_URI().length()));
        // this->setrequest_URI(__reqURI);
        std::cout << "Final request_URI :" << this->getrequest_URI() << std::endl;

    }
    if (_cgi && locationId == -1)
        locationId = _NO_CGI_LOCATION ; 
    
    //  if location is -1 that means request is not CGI and , location not found
    //  if location is _CGI that means request is  CGI and , location not found



    // std::cout << "URI :" << str << std::endl;

    return locationId;
}



std::vector<std::pair<std::string, std::string> > const & request::getReqBody( void ) const
{
    return this->req_body;
}

void request::setrequest_URI(std::string uri){
    this->request_URI = uri;
}

std::string const &request::getquery_string() const{
    return this->query_string;
}
void request::setquery_string(std::string query_string){
    this->query_string = query_string;
}