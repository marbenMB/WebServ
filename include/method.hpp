#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>

#include "./request.hpp"

class request;
class method
{
private:
    // ? config file :
    int isAllowed;
    int client_max_body_size;
    int redirect_status;
    std::string redirect_URL;

    int statuscode;
    int socketID;
    std::string host;
    std::string reason_phrase;
    std::string request_URI;
    std::string http_version;
    std::string root_path;
    std::string responseBody;

    int Content_Length;
    std::map<std::string, std::string> Content_Type;
    std::string Content_Transfer_Encoding;
    std::string Transfer_Encoding;

    int autoindex;

public:
    method();
    std::string const &getHost(void) const;
    std::string const &getreason_phrase(void) const;
    std::string const &getRootPath(void) const;
    std::string const &getRequest_URI(void) const;
    std::string const &getHttp_version(void) const;
    int const &getStatuscode(void) const;
    std::string const &getResponseBody(void) const;
    // this atributs for  Post method
    std::map<std::string, std::string> const &getContent_Type(void) const;
    std::string const &getTransfer_Encoding(void) const;
    int const &getClient_max_body_size(void) const;
    int const &getContent_Length(void) const;
    int const &getsocketID(void) const;
    int const &getAutoIndex() const;
    void setAutoIndex(int autoindex);
    void setsocketID(int socketId);
    void setRootPath(std::string root_path);
    void setHost(std::string host);
    void setreason_phrase(std::string host);
    void setRequest_URI(std::string request_URI);
    void setHttp_version(std::string http_version);
    void setStatuscode(int sCode);

    void setContent_Length(int Content_Length);
    void setClient_max_body_size(int);
    void setContent_Type(std::string Content_Type);
    void setTransfer_Encoding(std::string Transfer_Encoding);

    virtual ~method();
    void error(int statusCode, std::string reason_phrase);
    void createresponse(void);
    virtual int execute_method(request _request) = 0;
    void setResponseBody(std::string reqBody);
    //  redirect
    int const &getRedirect_status(void) const;
    void setRedirect_status(int redirect_status);
    std::string const &getredirect_URL(void) const;
    void setredirect_URL(std::string redirect_URL);

    // virtual bool intmethod(void) = 0;
};

class get : public method
{

public:
    get(request rhs);
    ~get();
    int execute_method(request _request);
};

class deleteMethod : public method
{

public:
    deleteMethod(request rhs);
    ~deleteMethod();
    int execute_method(request _request);
};

class Post : public method
{
private:
    std::vector<std::string> requestBody;
    std::map<std::string, std::string> Content_Disposition;
    std::string File_Type;

public:
    Post(request rhs);
    ~Post();
    int execute_method(request _request);
    bool parseBody_Content_Length();
    bool parseBody_Transfer_Encoding();
    std::vector<std::string> const &getRequestBody(void) const;
    void setRequestBody(std::vector<std::string> reqBody);
};

class NotAllowed : public method
{
public:
    NotAllowed(request rhs);
    ~NotAllowed();
    int execute_method(request _request);
};

#endif