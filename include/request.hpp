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
#include "./serverSide.hpp"
#include <stdlib.h>




// status Line :
#define REQUEST_URI "request_uri"
#define REQUEST_METHOD "request_method"
#define HTTP_VERSION "http_version"
#define PARAMS "params"
#define FAST_CGI_PASS "fastcgi_pass"
// request URI :

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
typedef struct _bodyfile{
    // request :
    std::map<std::string, std::string> ContentType;
    std::string _string;
    std::vector<std::string> tmp_vector;
    unsigned long long contentLength;

    // file :
    std::string _boundary_start;
    std::string _boundary_end;
    std::map<std::string, std::string> files;
} t_bodyfile;


class method;
class request
{
private:
    std::map<std::string, std::string> _requestHeaders;
    std::map<std::string, std::string> Content_Type;
    std::string compare_URI;

    std::vector<std::pair<std::string, std::string> > req_body;
    // config File atrs
    std::string root;
    unsigned long long client_max_body_size;
        // default :
    std::string default_index;
    std::string default_10x;
    std::string default_20x;
    std::string default_30x;
    std::string default_40x;
    std::string default_50x;
    int redirect_status;
    std::string redirect_URL;
    std::string upload_store;
        // cgi
    std::string CGIbody;
    std::string fastcgi_pass;
    std::string fastcgi_index;
    bool is_cgi;
        // allowed vars
    int autoindex;
    int __post;
    int __delete;
    int __get;
    int __noImplimented;

    // ! not checked .
    std::vector<std::string> requestBody;

    request(){};

    // for location
    std::map<std::string, std::string> _typs;
    std::map<std::string, std::string> _statusCode;



    void initializationRequestHeaders(std::vector<std::string> req);
    void initializationFILES(std::vector<std::string> filesVECTER);
    void Verifying_Header(std::string req);
    void Verifying_Body(std::string req);
    void printServerLogs(method const & vars);
    method *execute_request(void);

public:
    // ! cheacked :
    std::string _findHeader(std::string header);
    void _setHeaderReq(std::string key, std::string value);
    void url_decode(std::string &url);
    std::map<std::string, std::string> const &getContent_Type(void) const;

    // ! end cheack ;
    bool getIs_cgi(void);
    void addType(std::string, std::string);
    std::string const &getType(std::string);

    //  read Files
    bool retrievingsatatuscodeFile(void);
    std::string const &getReason(std::string);

    bool uploadType(void);

    request(int, ServerConf *, std::string, std::string &);
    std::vector<std::string> &execute(std::string body, Data *_confdata);
    void Retrieving_requested_resource(ServerConf *server);
    void GETstatusOfexecution(method *req_method) const;
    std::vector<std::string> const &create_response();
    int findLocation(std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >);
    void sand(int socketID, std::string body);

    ~request();
    
    void checkForIndex(std::vector<std::string> vect);
    void checkForErrorPage(std::vector<std::string> vect);
    void setAutoIndex(int autoindex);
    int const &getAutoIndex() const;
  
    std::string const &getroot() const;
    std::string getCGIbody() const;
    std::string const &getdefaultIndex() const;
    std::string getFastcgiPass() const;
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
        void cookie_session(request req, std::string &Cookie_value, int &is_valid);
    };
};
std::vector<std::string> split(const std::string &str, const std::string &delimiter);
std::string trimFront(const std::string &s, std::string trim);
std::string trimBack(const std::string &s, std::string trim);
std::string _CREATEresponse(std::map<std::string, std::string> content_type, int code_status, std::string reason_phrase, std::string body);
bool is__subDir(const std::string, const std::string);
bool Is_cgi(std::string);
#endif