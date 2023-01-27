#include "../../include/method.hpp"

deleteMethod::deleteMethod(request rhs)
{
    this->setHost(rhs.gethost());
    this->setRequest_URI(rhs.getrequest_URI());
    this->setHttp_version(rhs.gethttp_version());
    this->setStatuscode(200);
    this->setRootPath("/Users/mmasstou/Desktop/webserv/data");
    this->execute_method();
}
deleteMethod::~deleteMethod()
{
}
int deleteMethod::execute_method(void)
{
    std::ifstream inFile;
    // std::string responseBody;
    // check config file if the method is allowed:
    if (this->getRequest_URI().compare("/") == 0)
        return 1;
    std::string filename = this->getRootPath() + this->getRequest_URI();
    // std::cout << "Request Path :" << filename << std::endl;
    // read from server :
    int i = remove(filename.c_str());
    if (i)
        this->setStatuscode(400);
    this->setResponseBody("");
    // std::cout <<"\n\n\n\nresponseBody :\n"<< this->getResponseBody() << std::endl;
    return 1;
}