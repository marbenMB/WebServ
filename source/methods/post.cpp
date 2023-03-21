#include "../../include/method.hpp"

_Post::_Post(request rhs)
{
    
    this->execute_method(rhs);
}
_Post::~_Post()
{
}
int _Post::execute_method(request _request)
{
    std::string buffer;
    std::string body;
    std::string filename;
    std::ofstream outFile;
    std::ifstream inFile;
    filename.append(CREATE_SUCCESS_FILE);
    // bool _execute = false;
    std::map<std::string, std::string> tmp = _request.getContent_Type();
    // this->parseBody();
    if (_request.getRedirect_status() != -1)
    {
        this->setStatuscode(_request.getRedirect_status());
        this->setreason_phrase(_request.getReason(ft_to_string(_request.getRedirect_status())));
        filename.clear();
        filename.append(_request.getroot());
        filename.append(_request.getredirect_URL());

        _request._setHeaderReq(REQUEST_URI, filename);
        // set Header : 
        this->addHeader("Location", _request.getredirect_URL());
        body.append("");
    }
    else if (tmp["type"].compare("application/x-www-form-urlencoded") == 0){
        if (_request.getCGIstatus()){ throw request::CGI();}
    }
    else if (tmp["type"].compare("multipart/form-data") == 0)
    {
        std::vector<std::pair<std::string, std::string> > file = _request.getReqBody();
        std::vector<std::pair<std::string, std::string> >::iterator file_It = file.begin();
        while (file_It != file.end())
        {
            filename.clear();
            filename.append(_request.getUpload_store_PATH());
            struct stat statInfo;
            if (stat(filename.c_str(), &statInfo) != 0){
               filename.clear();
               filename.append(UPLOAD_STORE);
            }
            filename.append("/");
            filename.append((*file_It).first);
            // std::cout <<" abs path to the file created :" << filename << std::endl;
            outFile.open(filename, std::ifstream::out);
            if (!outFile.is_open()){ // cant open this file  Internal Server Error
               throw _Exception(INTERNAL_SERVER_ERROR);
            }
            outFile << file_It[0].second;
            outFile.close();
            ++file_It;
        }
        if (file_It == file.end()){
            // forbiden
            this->setStatuscode(201);
            this->setreason_phrase(_request.getReason("201"));
            filename.clear();
            filename.append(CREATE_SUCCESS_FILE);
            body.clear();
            std::stringstream ssbuf;
            inFile.open(filename, std::ifstream::in);
            ssbuf << inFile.rdbuf();
            body.append(ssbuf.str());
            inFile.close();
        }
    }
    this->setResponseBody(body);
    this->addHeader("Cache-Control", "no-cache");
    this->addHeader("Content-Type",Assets::__getType("html"));
    this->addHeader("Content-Length", ft_to_string(this->getResponseBody().length()));
    _request._setHeaderReq(REQUEST_URI, filename);
    return true;
}


void _Post::setRequestBody(std::vector<std::string> reqBody)
{
    this->requestBody = reqBody;
}

void _Post::setFilename(std::string value)
{
    this->_filename = value;
}
void _Post::setName(std::string value)
{
    this->_name = value;
}
void _Post::setContent(std::string value)
{
    this->_content = value;
}
 std::string const &_Post::getFilename( void ){
    return this->_filename;
 }
std::string const &_Post::getName( void ){
    return this->_name;
}
std::string const &_Post::getContent( void ){
    return this->_content;
}