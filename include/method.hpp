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
    int redirect_status;
    std::string redirect_URL;
protected:
public:
    void setStatus(int);
    method();
    std::string const &getreason_phrase(void) const;
    int const &getStatuscode(void) const;
    std::string const &getResponseBody(void) const;
    virtual ~method();
    virtual int execute_method(request _request) = 0;
    void setResponseBody(std::string reqBody);
    void setreason_phrase(std::string host);
    void setStatuscode(int sCode);
    //  redirect
    int const &getRedirect_status(void) const;
    void setRedirect_status(int redirect_status);
    std::string const &getredirect_URL(void) const;
    void setredirect_URL(std::string redirect_URL);
    // ! Header for response :
    void addHeader(std::string, std::string);
    std::map<std::string, std::string> const &getHeader(void) const;
};

class _Get : public method
{

public:
    _Get(request & rhs);
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
public:
    _Post(request & rhs);
    ~_Post();
    int execute_method(request _request);
};



class Error : public method
{
public:
    Error(request rhs);
    ~Error(){};
    int execute_method(request _request);
};

#endif