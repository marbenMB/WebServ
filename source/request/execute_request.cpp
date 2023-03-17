#include "../../include/request.hpp"

method *request::execute_request(void)
{
    method *reqmethod = nullptr;
    std::string __body;

   if (this->__noImplimented == NOT_ALLOWED){
        if (this->req_method.compare("GET") == 0 && this->__get == ALLOWED)
            reqmethod = new get(*this);
        else if (this->req_method.compare("DELETE") == 0 && this->__delete == ALLOWED)
            reqmethod = new deleteMethod(*this);
        else if (this->req_method.compare("POST") == 0 && this->__post == ALLOWED)
            reqmethod = new Post(*this);
        else
            throw  _Exception(METHOD_NO_ALLOWED);
    }
    else 
        throw  _Exception(NOT_IMPLEMENTED);
    return reqmethod;
}