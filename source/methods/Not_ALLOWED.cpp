#include "../../include/method.hpp"
NotImplemented::NotImplemented(request rhs)
{
    if (rhs.getrequirements()){
    this->setHost(rhs.gethost());
    this->setRequest_URI("/Users/mmasstou/Desktop/webserV/var/errors/not_allowed.html");
    this->setHttp_version(rhs.gethttp_version());
    this->setStatuscode(405);
    this->setreason_phrase("Not Allowed");
    this->setRootPath(rhs.getroot());
    }
    this->execute_method(rhs);
    
/**
    *
    *! Verifying_Header that the request method is indeed "GET" and the URI is valid.
    *! Retrieving the requested resource from the server.
    *! Creating the response headers, including the "Content-Type" header to indicate the format of the response and any other headers relevant to the response.
    *! Creating the response message body, which should contain the requested resource.
    *! Sending the complete response message back to the client.
*/
}

NotImplemented::~NotImplemented()
{
}

int NotImplemented::execute_method(request _request)
{
    (void)_request;
    std::string buffer;
    std::string line;
    line.clear();
    buffer.clear();
    this->setContent_Type("text/html");
    std::ifstream inFilemessage;

    if (_request.getmethod().compare("GET") != 0 && _request.getmethod().compare("POST") != 0 && _request.getmethod().compare("DELETE") != 0){
        // method Not Implimented
        this->setStatuscode(501);
        this->setreason_phrase("Not Implemented");
        line.append("./var/srcs/Notimplimented.html");
    }
    else if (
       ( (_request.getmethod().compare("GET") == 0 && _request.getAllowedGet() == NOT_ALLOWED) ||
        ( _request.getmethod().compare("POST") == 0 && _request.getAllowedPost() == NOT_ALLOWED) ||
        ( _request.getmethod().compare("DELETE") == 0 && _request.getAllowedDelete() == NOT_ALLOWED))
        && _request.getrequirements() == true){
        // method Not Allowed
        line.clear();
        this->setStatuscode(405);
        this->setreason_phrase("Method Not Allowed");
        line.append("./var/srcs/not_allowed.html");
    }
    else{
        // Bad request
        line.clear();
        this->setStatuscode(400);
        this->setreason_phrase("Bad Request");
        line.append("./var/srcs/bad_request.html");
    }
    inFilemessage.open(line, std::ifstream::in);
    line.clear();
    while (std::getline(inFilemessage, buffer))
    {
        line.append(buffer);
    }
    this->setResponseBody(line);
    inFilemessage.close();
    return false;
}