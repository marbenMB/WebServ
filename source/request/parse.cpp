#include "../../include/request.hpp"

std::vector<std::string> &request::execute(std::string body, Data *_confdata)
{

    method *reqmethod = nullptr;
    std::vector<std::string> rBody = split(body, "\r\n\r\n");
    this->message.push_back(std::to_string(this->socketID));
    // std::cout << "\nmethod : " << this->getmethod() << std::endl;

    std::vector<ServerConf> serv = _confdata->server_list;
    std::vector<ServerConf>::iterator serv__it = _confdata->server_list.begin();
    std::map<std::string, std::vector<std::string> > it = _confdata->server_list.begin()->server_data;
    std::vector<std::string> root_vect = it["root"];
    std::vector<std::string> error_page_vect = it["root"];
    std::cout << "root_vect : " << root_vect[0] << std::endl;
    if (!root_vect[0].empty())
        this->root = root_vect[0];
    // while (it != serv__it->server_data.end())
    // {
    //         ++it;
    // }

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
    std::cout << "[" << this->gethost() << "] " << this->gethttp_version() << " " << reqmethod->getStatuscode() << " " << reqmethod->getreason_phrase() << " " << this->request_URI << std::endl;
    delete reqmethod;
    return (this->message);
}
