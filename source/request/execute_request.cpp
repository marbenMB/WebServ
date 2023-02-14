#include "../../include/request.hpp"

method *request::execute_request(void)
{
    method *reqmethod = nullptr;
    std::string __body;

    if (this->req_method.compare("GET") == 0 && this->__get == ALLOWED && this->requirements)
        reqmethod = new get(*this);
    else if (this->req_method.compare("DELETE") == 0 && this->__delete == ALLOWED && this->requirements)
    {
        reqmethod = new deleteMethod(*this);
    }
    else if (this->req_method.compare("POST") == 0 && this->__post == ALLOWED && this->requirements)
    {
        // this->setRequestBody(rBody);
        reqmethod = new Post(*this);
    }
    else
    {
        // std::cout << this->req_method << " method not allowed" << std::endl;
        // reqmethod->error(404, "Not Allowed");
        reqmethod = new NotImplemented(*this);
        std::cout << GREEN << "   Method : " << this->getmethod() << ":: NOT ALLOWED" << END_CLR << std::endl;
    } // the request not allowed
    return reqmethod;
}