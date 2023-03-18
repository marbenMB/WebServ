#ifndef REQUEST_H
#define REQUEST_H

#define BLUE "\x1b[36m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define MAUVE "\x1b[33m"
#define END_CLR "\033[0m"

#define ALLOWED 12054
#define NOT_ALLOWED 12055

#define AUTOINDEX_ON 548
#define AUTOINDEX_OFF 549
#define _NO_CGI_LOCATION 1337

#define SP " "
#define LF "\n"
#define CR "\r"
#define CRLF "\r\n"
#define CRLF_2 "\r\n\r\n"

// Assets :

#define AOTUINDEX_PATH "./var/assets/autoIndex.html"
#define UPLOAD_STORE "./var/upload/Default"
#define CREATE_SUCCESS_FILE "./var/srcs/success.html"
#define ERROR_PATH "./var/errors/40x.html"

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <fstream>
#include "./WebServer.hpp"

#include "./method.hpp"
#include "./_Exception.hpp"
#include <stdlib.h>

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

class method;
class request
{
private:
    int _ExceptionCode;
    bool requirements;
    int socketID;
    std::string root;
    std::string bufferBody;

    // default :
    std::string default_index;
    std::string default_10x;
    std::string default_20x;
    std::string default_30x;
    std::string default_40x;
    std::string default_50x;
    int redirect_status;
    std::string redirect_URL;

    // Requirements Fields :
    std::string req_method;
    std::string host;
    std::string request_URI;
    std::string query_string;
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
    unsigned long long Content_Length;
    unsigned long long client_max_body_size;

    std::string upload_store;
    std::string Content_Type;
    std::string Content_Transfer_Encoding;
    std::string Transfer_Encoding;
    int autoindex;
    std::vector<std::pair<std::string, std::string> > req_body;

    // cgi
    std::string CGIbody;
    std::string fastcgi_pass;
    std::string fastcgi_index;
    bool is_cgi;

    // allowed vars
    int __post;
    int __delete;
    int __get;
    int __noImplimented;
    request(){};
    std::vector<std::string> message;
    //  error

    // for location
    std::string compare_URI;
    std::map<std::string, std::string> _typs;
    std::map<std::string, std::string> _statusCode;

    void initializationFILES(std::vector<std::string> filesVECTER);

public:
    // Request Exception :
    int getExceptionCode();

    bool getIs_cgi(void);
    void addType(std::string, std::string);
    std::string const &getType(std::string);

    //  read Files
    bool retrievingsatatuscodeFile(void);
    std::string const &getReason(std::string);

    bool uploadType(void);

    void url_decode(std::string &url);
    request(int, ServerConf *, std::string, std::string &);
    std::vector<std::string> &execute(std::string body, Data *_confdata);
    method *execute_request(void);
    void Retrieving_requested_resource(ServerConf *server);
    void GETstatusOfexecution(method *req_method) const;
    std::vector<std::string> const &create_response();
    int findLocation(std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >);
    void sand(int socketID, std::string body);

    ~request();
    bool Verifying_Header(std::string req);
    bool Verifying_Body(std::string req);
    void checkForIndex(std::vector<std::string> vect);
    void checkForErrorPage(std::vector<std::string> vect);
    bool executeAction();
    void setAutoIndex(int autoindex);
    int const &getAutoIndex() const;
    std::string getmethod() const
    {
        return this->req_method;
    }
    bool getrequirements(void) const;
    std::string const &gethost() const;
    std::string const &getroot() const;
    std::string const &getcookie() const;
    std::string const &getreq_method() const;
    std::string getCGIbody() const;
    std::string const &getdefaultIndex() const;
    std::string const &getrequest_URI() const;
    void setrequest_URI(std::string);
    std::string const &getquery_string() const;
    void setquery_string(std::string);
    std::string const &gethttp_version() const;
    int const &getsocketID(void) const;
    void setsocketID(int socketId);
    std::string const &getContent_Type(void) const;
    std::string const &getTransfer_Encoding(void) const;
    unsigned long long const &getContent_Length(void) const;
    //  redirect
    int const &getRedirect_status(void) const;
    void setRedirect_status(int redirect_status);
    std::string const &getredirect_URL(void) const;
    void setredirect_URL(std::string redirect_URL);
    std::vector<std::string> const &getRequestBody(void) const;
    void setRequestBody(std::vector<std::string> reqBody);
    // Allowed method :
    int getAllowedPost() const;
    int getAllowedDelete() const;
    int getAllowedGet() const;

    std::vector<std::pair<std::string, std::string> > const &getReqBody(void) const;

    // error pages :
    std::string const &getDefault_40x(void);
    std::string const &getDefault_50x(void);
    std::string const &getDefault_30x(void);
    void print_vectINFO(std::vector<std::string>, std::string);

    // location
    std::string const &getcompare_URI(void) const;
    void setcompare_URI(std::string);

    // upload_store
    std::string const &getUpload_store_PATH(void) const;

    //  CGI :
    bool getCGIstatus(void) const;

    class CGI
    {
    public:
        method *runCGI(request req);
    };
};
std::vector<std::string> split(const std::string &str, const std::string &delimiter);
std::string trimFront(const std::string &s, std::string trim);
std::string trimBack(const std::string &s, std::string trim);
std::string _CREATEresponse(std::map<std::string, std::string> content_type, int code_status, std::string reason_phrase, std::string body);
bool is__subDir(const std::string, const std::string);
bool Is_cgi(std::string);
#endif