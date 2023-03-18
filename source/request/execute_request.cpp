#include "../../include/request.hpp"

method *request::execute_request(void)
{
    method *reqmethod = nullptr;
    std::string __body;

   if (this->__noImplimented == NOT_ALLOWED){
        if (this->req_method.compare("GET") == 0 && this->__get == ALLOWED)
            reqmethod = new _Get(*this);
        else if (this->req_method.compare("DELETE") == 0 && this->__delete == ALLOWED)
            reqmethod = new _Delete(*this);
        else if (this->req_method.compare("POST") == 0 && this->__post == ALLOWED)
            reqmethod = new _Post(*this);
        else
            throw  _Exception(METHOD_NO_ALLOWED);
    }
    else 
        throw  _Exception(NOT_IMPLEMENTED);
    return reqmethod;
}