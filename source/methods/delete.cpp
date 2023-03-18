#include "../../include/method.hpp"

_Delete::_Delete(request rhs)
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

_Delete::~_Delete()
{
}

int _Delete::execute_method(request _request)
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
    filename.clear(); // clear for delete the garbidge value 
    // filename.append(this->getRootPath()); // append the root path that get it from the config file
    filename.append(_request.getrequest_URI());
    std::cout << "Filename :" << filename <<std::endl; // append the request URI that get it also from request line 
    // * stat is function that check a psth if exist and store the more Info for this path in 'STATInfo' that we passe as reference 
    // * if the result return != 0 => the path no exist 
    // * if the result == S_IFREG => the path is file 
    // * if the result == S_IFDIR => the path is dir (folder)
    if (stat(filename.c_str(), &STATInfo) != 0){ // 404 not Found => the file dose not existe 
        throw  _Exception(NOT_FOUND);
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFREG){ // 202 No Content => the file exist 
        int remove_status = remove(filename.c_str());
        if (remove_status != 0){ // this file can removed becouse of permistion
            throw  _Exception(FORBIDDEN);
        }
        else{ // the file that passed he dir is removed success ; we shuld check a redirection in confg file
            this->setStatuscode(202);
            this->setreason_phrase("No Content");
            line.clear();
            line.append("./var/srcs/NoContent.html");
        }
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFDIR){ // 403 Forbidden => the path that passed is for Folder and we shuld not remove it
       throw  _Exception(FORBIDDEN);
    }
    inFilemessage.open(line, std::ifstream::in);  // for ech case the line string is append a path for the file that we chuld get from the html to rander it to bady response
    line.clear(); // after we open the file , clear the string to append it data of the file
    buffer.clear(); // clear the buffer also for security resen :)
    // std::cout << "<Line URi='"<< this->getRequest_URI() <<"' root='"<< this->getRootPath()<<"' filename='"<<filename <<"'>" << std::endl;
    while (std::getline(inFilemessage, buffer)){
        line.append(buffer);
    }

    inFilemessage.close();
    this->setResponseBody(line); // set the data that previce read from the file to the body response 
    this->addHeader("Cache-Control", "no-cache");
    this->addHeader("Content-Type",Assets::__getType("html"));
    this->addHeader("Content-Length", std::to_string(this->getResponseBody().length()));
    return 1; 
}