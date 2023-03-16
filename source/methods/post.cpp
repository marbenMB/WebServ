#include "../../include/method.hpp"

Post::Post(request rhs)
{
    this->setHost(rhs.gethost());
    this->setRequest_URI(rhs.getrequest_URI());
    this->setHttp_version(rhs.gethttp_version());
    this->setsocketID(rhs.getsocketID());
    this->setRootPath("/Users/mmasstou/Desktop/webserv/data");
    this->setRequestBody(rhs.getRequestBody());
    this->setContent_Length(rhs.getContent_Length());
    this->setContent_Type(rhs.getContent_Type());
    this->setTransfer_Encoding(rhs.getTransfer_Encoding());
    this->execute_method(rhs);
    this->setStatuscode(201);
    this->setreason_phrase("Created");
}
Post::~Post()
{
}
int Post::execute_method(request _request)
{
    std::string buffer;
    std::string body;
    std::string filename;
    std::ofstream outFile;
    std::ifstream inFile;
    filename.append(CREATE_SUCCESS_FILE);
    // bool _execute = false;
    std::map<std::string, std::string> tmp = this->getContent_Type();
    // this->parseBody();
    if (_request.getRedirect_status() != -1)
    {

        /*
        
        
        <!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>301 Redirction</title></head><body><h1>301 Redirction</h1></body></html>
        
        **/ 
        this->setStatuscode(_request.getRedirect_status());
        this->setreason_phrase(_request.getReason(std::to_string(_request.getRedirect_status())));
        filename.clear();
        filename.append(_request.getroot());
        filename.append(_request.getredirect_URL());

        _request.setrequest_URI(filename);
        // set Header : 
        this->addHeader("Location", _request.getredirect_URL());
        body.append("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>");
        body.append(std::to_string(this->getStatuscode()));
        body.append(" Redirction</title></head><body><h1>");
        body.append(std::to_string(this->getStatuscode()));
        body.append(" Redirction</h1></body></html>");
        // inFile.open(filename, std::ifstream::in);
        // while (std::getline(inFile, buffer))
        // {
        //     // std::cout << buffer << std::endl;
        //     line.append(buffer);
        // }
        // // std::cout << "</Line >" << std::endl;
        // inFile.close();
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
            filename.append("/");
            filename.append((*file_It).first);
            // std::cout <<" abs path to the file created :" << filename << std::endl;
            outFile.open(filename, std::ifstream::out);
            if (!outFile.is_open()){ // cant open this file  Internal Server Error
                this->setStatuscode(500);
                 this->setreason_phrase(_request.getReason("500"));
                filename.clear();
                filename.append("./var/errors/50x.html");
                file_It = file.begin();
                while (file_It != file.end()){
                    filename.clear();
                    filename.append(_request.getUpload_store_PATH());
                    filename.append("/");
                    filename.append((*file_It).first);
                    remove(filename.c_str());
                    ++file_It;
                }
            }

            outFile << (*file_It).second;
            // std::cout <<RED<< "filenameee =" <<END_CLR<< (*file_It).first << std::endl;
            // std::cout <<RED<< "Body       =" <<END_CLR<< (*file_It).second << std::endl;
            outFile.close();
            ++file_It;
        }
        if (file_It == file.end()){
            // forbiden
            this->setStatuscode(201);
            this->setreason_phrase(_request.getReason("201"));
            filename.clear();
            filename.append("./var/srcs/success.html");
            body.clear();
            std::stringstream ssbuf;
             inFile.open(filename, std::ifstream::in);
            ssbuf << inFile.rdbuf();
            body.append(ssbuf.str());
            // while (std::getline(inFile, buffer))
            // {
            //     // std::cout << buffer << std::endl;
            //     line.append(buffer);
            // }
            inFile.close();

        }

    }
    
    this->setResponseBody(body);
    this->addHeader("Cache-Control", "no-cache");
    this->addHeader("Content-Type",Assets::__getType("html"));
    this->addHeader("Content-Length", std::to_string(this->getResponseBody().length()));
    return true;
}

bool Post::parseBody()
{
    // std::cout << "parseBody_Content_Length\n";
    std::vector<std::string> reqbody;
    std::map<std::string, std::string> tmp = this->getContent_Type();

    reqbody = this->getRequestBody();
    // std::cout << "Body size :" <<reqbody.size() << std::endl;
    std::cout << "get +> <Content_Type size='" << tmp.size() << "' type='" << tmp["type"] << "'   boundary='" << tmp["boundary"] << "'>" << std::endl;
    return 1;
}
bool Post::parseBody_Transfer_Encoding()
{
    std::cout << "parseBody_Transfer_Encoding\n";
    return 1;
}
std::vector<std::string> const &Post::getRequestBody(void) const
{
    return (this->requestBody);
}
void Post::setRequestBody(std::vector<std::string> reqBody)
{
    this->requestBody = reqBody;
}
void method::setContent_Length(int Content_Length)
{
    this->Content_Length = Content_Length;
}
void method::setClient_max_body_size(int Client_max_body_size)
{
    this->client_max_body_size = Client_max_body_size;
}
void method::setContent_Type(std::string Content_Type)
{
    std::vector<std::string> tmp = split(Content_Type, ";");
    // std::cout << "set +> <Content_Type size='" << tmp.size() << "' type='" << tmp[0] << "'   boundary='" << tmp[1] << "'>" << std::endl;
    this->Content_Type["type"] = (std::string)tmp[0];
    if (tmp.size() == 2)
        this->Content_Type["boundary"] = trimFront((std::string)tmp[1], "boundary=");
}
void method::setTransfer_Encoding(std::string Transfer_Encoding)
{
    this->Transfer_Encoding = Transfer_Encoding;
}

void Post::setFilename(std::string value)
{
    this->_filename = value;
}
void Post::setName(std::string value)
{
    this->_name = value;
}
void Post::setContent(std::string value)
{
    this->_content = value;
}
 std::string const &Post::getFilename( void ){
    return this->_filename;
 }
std::string const &Post::getName( void ){
    return this->_name;
}
std::string const &Post::getContent( void ){
    return this->_content;
}