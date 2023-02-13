#include "../../include/method.hpp"
NotAllowed::NotAllowed(request rhs)
{

    this->setHost(rhs.gethost());
    this->setRequest_URI("/Users/mmasstou/Desktop/webserV/var/errors/not_allowed.html");
    this->setHttp_version(rhs.gethttp_version());
    this->setStatuscode(405);
    this->setreason_phrase("Not Allowed");
    this->setRootPath(rhs.getroot());
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

NotAllowed::~NotAllowed()
{
}

int NotAllowed::execute_method(request _request)
{
    (void)_request;
    std::string buffer;
    std::string line;
    line.clear();
    buffer.clear();
    this->setContent_Type("text/html");
    std::ifstream inFilemessage;
    inFilemessage.open("/Users/mmasstou/Desktop/webserV/var/errors/not_allowed.html", std::ifstream::in);
    while (std::getline(inFilemessage, buffer))
    {
        // std::cout << buffer << std::endl;
        line.append(buffer);
    }
    this->setResponseBody(line);
    return false;
}