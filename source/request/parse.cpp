#include "../../include/request.hpp"

std::vector<std::string> &request::execute(std::string body)
{

    method *reqmethod = nullptr;
    std::vector<std::string> rBody = split(body, "\r\n\r\n");
    this->message.push_back(std::to_string(this->socketID));
    // std::cout << "\nmethod : " << this->getmethod() << std::endl;
    ;

    if (this->req_method.compare("GET") == 0)
        reqmethod = new get(*this);
    else if (this->req_method.compare("DELETE") == 0)
        reqmethod = new deleteMethod(*this);
    else if (this->req_method.compare("POST") == 0)
    {
        this->setRequestBody(rBody);
        reqmethod = new Post(*this);
    }
    else 
    {
        
    }
    if (reqmethod->getStatuscode() != 200)
    {
        // std::cout << "\e[1;31m+>Bad Request\n\e[0m";
        reqmethod->setRequest_URI("40x.html");
        std::ifstream inFile;
        std::string line;
        // std::string responseBody;
        std::string filename = "/Users/mmasstou/Desktop/webserv/data/errors/40x.html";
        // check config file if the method is allowed:
        // std::cout << "Request Path :" << filename << std::endl;
        // read from server :
        inFile.open(filename, std::ifstream::in);
        body = "";
        while (std::getline(inFile, line))
        {
            body.append(line);
        }
        this->requirements = false;
        reqmethod->setResponseBody(body);
    }
    reqmethod->createresponse();
    // std::cout << "\n    Body :\n"
            //   << reqmethod->getResponseBody() << std::endl;
    this->message.push_back(reqmethod->getResponseBody());

    // std::cout << "\n    ++> SocketId :" << this->message[0] << "\n    ++>Message :" << this->message[1] << std::endl;
    // std::cout <<"\n" << reqmethod->getHost()<< " " <<  reqmethod->getStatuscode() << " " ;
    // * print status : HTTP/1.1 200 OK
    std::cout <<"["<< this->gethost()<<"] " << this->gethttp_version() << " " << reqmethod->getStatuscode() << " " << reqmethod->getreason_phrase()<<" " << this->request_URI << std::endl;
    delete reqmethod;
    return (this->message);
}
