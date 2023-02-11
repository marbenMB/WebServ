#include "../../include/method.hpp"

deleteMethod::deleteMethod(request rhs)
{
    this->setHost(rhs.gethost());
    this->setRequest_URI(rhs.getrequest_URI());
    this->setHttp_version(rhs.gethttp_version());
    this->setStatuscode(200);
    this->setreason_phrase("OK");
    this->setRootPath(rhs.getroot());
    this->setResponseBody("");
    this->setRedirect_status(rhs.getRedirect_status());
    this->setredirect_URL(rhs.getredirect_URL());
    this->setContent_Type(rhs.getContent_Type());
    this->execute_method();
}

deleteMethod::~deleteMethod()
{
}

int deleteMethod::execute_method(void)
{
    std::ifstream inFile;
    std::ifstream inFilemessage;
    std::string line = "";
    std::string buffer;
    // std::string responseBody;
    // check config file if the method is allowed:
    if (this->getRequest_URI().compare("/") == 0)
        return 1;
    if (this->getContent_Type().empty())
    {
        this->setContent_Type("text/plain");
    }
    std::string filename = "";
    filename.append(this->getRootPath());
   
    filename.append(this->getRequest_URI());
    // inFile.open( "/Users/mmasstou/projects/WebServ/var/errors/20x.html", std::ifstream::in);
    std::cout << "Delete file  :" << filename << std::endl;
    // read from server :
    int i = remove(filename.c_str());
    if (i)
    {
        this->error(404, "Not Found");
        inFilemessage.open("/Users/mmasstou/projects/WebServ/var/errors/40x.html", std::ifstream::in);
    }
    else
    {
        if (this->getRedirect_status() != -1){
            this->error(301, "redirect");
            inFilemessage.open(this->getRootPath() + this->getredirect_URL(), std::ifstream::in);
        }
        else{
            this->error(301, "Delete success");
            inFilemessage.open("/Users/mmasstou/projects/WebServ/var/errors/success.html", std::ifstream::in);
        }
    }
    // std::cout << "<Line URi='"<< this->getRequest_URI() <<"' root='"<< this->getRootPath()<<"' filename='"<<filename <<"'>" << std::endl;
    while (std::getline(inFilemessage, buffer))
    {
        // std::cout << buffer << std::endl;
        line.append(buffer);
    }
    // std::cout << "</Line >" << std::endl;

    this->setResponseBody(line);
    // exit(1);
    // std::cout <<"\n\n\n\nresponseBody :\n"<< this->getResponseBody() << std::endl;
    return 1;
}