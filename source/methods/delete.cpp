#include "../../include/method.hpp"

_Delete::_Delete(request _request)
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
    filename.append(_request._findHeader(REQUEST_URI));
    if (stat(filename.c_str(), &STATInfo) != 0){ // 404 not Found => the file dose not existe 
        throw  _Exception(NOT_FOUND);
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFREG){ // 202 No Content => the file exist 
        int remove_status = remove(filename.c_str());
        if (remove_status != 0){ // this file can removed becouse of permistion
            throw  _Exception(FORBIDDEN);
        }
        else{ // the file that passed he dir is removed success ; we shuld check a redirection in confg file
            this->setStatus(NO_CONTENT);
            line.clear();
            line.append(NO_CONTENT_PATH);
        }
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFDIR){ // 403 Forbidden => the path that passed is for Folder and we shuld not remove it
       throw  _Exception(FORBIDDEN);
    }
    inFilemessage.open(line.c_str(), std::ifstream::in);  // for ech case the line string is append a path for the file that we chuld get from the html to rander it to bady response
    line.clear(); // after we open the file , clear the string to append it data of the file
    buffer.clear(); // clear the buffer also for security resen :)
    while (std::getline(inFilemessage, buffer)){
        line.append(buffer);
    }

    inFilemessage.close();
    this->setResponseBody(line); // set the data that previce read from the file to the body response 
    this->execute_method(_request);
}

_Delete::~_Delete(){}
int _Delete::execute_method(request _request){
    (void)_request;
    this->addHeader("Content-Type",Assets::__getType("html"));
    this->addHeader("Content-Length", ft_to_string(this->getResponseBody().length()));
    return 0;
}
