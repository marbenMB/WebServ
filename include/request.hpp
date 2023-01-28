#ifndef REQUEST_H
# define REQUEST_H

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <fstream> 
#include "./WebServer.hpp"

#include "./method.hpp"


/*

There are many header fields that can be included in an HTTP/1.1 request, here are some of the most commonly used ones:

Accept: Specifies the acceptable content types for the response.
Accept-Charset: Specifies the acceptable character sets for the response.
Accept-Encoding: Specifies the acceptable content encodings for the response.
Accept-Language: Specifies the acceptable natural languages for the response.
Authorization: Contains authentication information for the user agent.
Cache-Control: Specifies the caching behavior of the request.
Connection: Specifies the connection options for the request.
Content-Length: Specifies the size of the request body.
Content-Type: Specifies the media type of the request body.
Cookie: Contains a list of cookies to be sent with the request.
Host: Specifies the host and port number of the resource being requested.
If-Modified-Since: Specifies that the requested resource should only be sent if it has been modified since the specified date.
If-None-Match: Specifies that the requested resource should only be sent if its entity tag does not match the specified tag.
Referer: Specifies the URI of the referring resource.
User-Agent: Identifies the user agent making the request.

**/
class request
{
private:
    bool requirements;
    int socketID;
    std::string root;
    std::string bufferBody;


    // Requirements Fields :
    std::string req_method;
    std::string host;
    std::string request_URI;
    std::string http_version;
    // Optional Fields :
    std::string Accept;
    std::string Accept_Charset;
    std::string Accept_Encoding;
    std::string Accept_Language;
    std::string Authorization;
    std::string Cache_Control;
    std::string Connection;
    std::string Cookie;
    std::string If_Modified_Since;
    std::string If_None_Match;
    std::string Referer;
    std::string User_Agent;
    std::vector<std::string> requestBody;



    // post requirements
    int Content_Length;
    std::string Content_Type;
    std::string Content_Transfer_Encoding;
    std::string Transfer_Encoding;

    request(){};

    bool parseHeader(void); 
    bool parseBody(void); 


    std::vector<std::string> message ;


public:
    request(int socketID, std::string req);
    std::vector<std::string> &execute(std::string body, Data * _confdata);
    void sand(int socketID, std::string body);


    ~request();
    bool Verifying_Header(std::string req);
    bool executeAction();
    std::string getmethod() const
    {
        return this->req_method;
    }

    std::string const & gethost() const;
    std::string const & getrequest_URI() const;
    std::string const & gethttp_version() const;
    int const &getsocketID(void) const;
    void setsocketID(int socketId);
    std::string const &getContent_Type(void) const;
    std::string const &getTransfer_Encoding(void) const;
    int const &getContent_Length(void) const;

    std::vector<std::string> const &getRequestBody(void) const;
    void setRequestBody(std::vector<std::string> reqBody);

    
};
std::vector<std::string> split(const std::string &str, const std::string &delimiter);
std::string trimFront(const std::string &s, std::string trim);


#endif