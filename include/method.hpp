#ifndef _METHOD_HPP_
#define _METHOD_HPP_

#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include "./request.hpp"
#include "./Assets.hpp"
#include "./_Exception.hpp"

class request;
class method
{
private:
    std::map<std::string, std::string> _Headers;
    std::string responseBody;
// ! checked 

    // errors vars
    int statuscode;
    std::string reason_phrase;

    // ? config file :
    int client_max_body_size;
    int redirect_status;
    std::string redirect_URL;

    int socketID;
    std::string host;
    std::string request_URI;
    std::string http_version;
    std::string root_path;

    int Content_Length;
    std::map<std::string, std::string> Content_Type;
    std::string Content_Transfer_Encoding;
    std::string Transfer_Encoding;

public:
    method();
    std::string const &getHost(void) const;
    std::string const &getreason_phrase(void) const;
    int const &getStatuscode(void) const;
    std::string const &getResponseBody(void) const;
// ! checked 

    // this atributs for  Post method
    std::map<std::string, std::string> const &getContent_Type(void) const;
    std::string const &getTransfer_Encoding(void) const;
    int const &getClient_max_body_size(void) const;
    int const &getContent_Length(void) const;
    int const &getsocketID(void) const;
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

    // ! Header for response :
    void addHeader(std::string, std::string);
    std::map<std::string, std::string> const &getHeader(void) const;

    // virtual bool intmethod(void) = 0;
};

class _Get : public method
{

public:
    _Get(request rhs);
    ~_Get();
    int execute_method(request _request);
};

class _Delete : public method
{

public:
    _Delete(request rhs);
    ~_Delete();
    int execute_method(request _request);
};

class _Post : public method
{
private:
    std::vector<std::string> requestBody;
    std::string _filename;
    std::string _name;
    std::string _content;

public:
    _Post(request rhs);
    ~_Post();
    int execute_method(request _request);
    void setRequestBody(std::vector<std::string> reqBody);
    // setter :
    void setFilename(std::string value);
    void setName(std::string value);
    void setContent(std::string value);
    // getter :
    std::string const &getFilename(void);
    std::string const &getName(void);
    std::string const &getContent(void);
};

class Error : public method
{
public:
    Error(request rhs);
    ~Error(){};
    int execute_method(request _request);
};

#endif