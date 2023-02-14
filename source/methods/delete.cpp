#include "../../include/method.hpp"

deleteMethod::deleteMethod(request rhs)
{
    this->setHost(rhs.gethost());
    // this->setRequest_URI(rhs.getrequest_URI());
    this->setHttp_version(rhs.gethttp_version());
    this->setStatuscode(200);
    this->setreason_phrase("OK");
    this->setRootPath(rhs.getroot());
    this->setResponseBody("");
    this->setRedirect_status(rhs.getRedirect_status());
    this->setredirect_URL(rhs.getredirect_URL());
    this->setContent_Type("text/html");
    this->execute_method(rhs);
}

deleteMethod::~deleteMethod()
{
}

int deleteMethod::execute_method(request _request)
{
    std::ifstream inFile;
    std::ifstream inFilemessage;
    std::string line = "";
    std::string buffer;
    struct stat STATInfo;
    (void)_request;
    // std::string responseBody;
    // check config file if the method is allowed:
    std::string filename;
    filename.clear();
    filename.append(this->getRootPath());
    filename.append(_request.getrequest_URI());

    if (stat(filename.c_str(), &STATInfo) != 0){ // 404 not Found
        this->setStatuscode(404);
        this->setreason_phrase("Not Found");
        line.clear();
        line.append("./var/srcs/notfound.html");
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFREG){ // 202 No Content
        int remove_status = remove(filename.c_str());
        if (remove_status != 0){
            this->setStatuscode(403);
            this->setreason_phrase("Forbidden");
            line.clear();
            line.append("./var/srcs/forbidden.html");
        }
        else{
            this->setStatuscode(202);
            this->setreason_phrase("No Content");
            line.clear();
            line.append("./var/srcs/NoContent.html");
        }
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFDIR){ // 403 Forbidden
        this->setStatuscode(403);
        this->setreason_phrase("Forbidden");
        line.clear();
        line.append("./var/srcs/forbidden.html");
    }
    inFilemessage.open(line, std::ifstream::in);
    line.clear();
    buffer.clear();
    // std::cout << "<Line URi='"<< this->getRequest_URI() <<"' root='"<< this->getRootPath()<<"' filename='"<<filename <<"'>" << std::endl;
    while (std::getline(inFilemessage, buffer)){
        line.append(buffer);
    }
    this->setResponseBody(line);
    return 1;
}