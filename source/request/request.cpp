#include "../../include/request.hpp"

request::request(int socketID, std::string requestHeader) : Content_Length(-1)
{
    std::string _requestBody;
    // * Init vars :
    this->requirements = true;
    this->setsocketID(socketID);
    // * parse Header :
    if (this->Verifying_Header(requestHeader) == false) this->requirements = false;
    if (this->Verifying_Body(_requestBody)  == false) this->requirements = false;

}


void request::sand(int socketID, std::string body){
    int n;
   
    n = write(socketID, body.c_str(), strlen(body.c_str()));
    if (n < 0){
        perror("ERROR writing to socket");
        close(socketID);
        exit(1);
    }
    else{
        close(socketID);
    }
}

bool request::Verifying_Body(std::string req){
    (void)req;
    return true;
}

bool request::Verifying_Header(std::string req)
{
    std::vector<std::string> requestHeaders = split(req, "\r\n");
    std::vector<std::string>::iterator itH = requestHeaders.begin();
    std::vector<std::string> spl;


    spl = split((std::string)itH[0], " ");
    this->req_method = spl[0];
    this->request_URI = spl[1];
    this->http_version = spl[2];
    while (++itH != requestHeaders.end())
    {
        spl = split((std::string)*itH, ": ");
        if(spl[0].compare("Host") == 0){this->host = spl[1];}
        if(spl[0].compare("User-Agent") == 0){this->User_Agent = spl[1];}
        if(spl[0].compare("Accept-Encoding") == 0){this->Accept_Encoding = spl[1];}
        if(spl[0].compare("Connection") == 0){this->Connection = spl[1];}
        if(spl[0].compare("Content-Length") == 0){this->Content_Length = atoi(spl[1].c_str());}
        if(spl[0].compare("Content-Type") == 0){this->Content_Type = spl[1];}
        if(spl[0].compare("Content-Transfer-Encoding") == 0){this->Content_Transfer_Encoding = spl[1];}
        if(spl[0].compare("Transfer-Encoding") == 0){this->Transfer_Encoding = spl[1];}
        if(spl[0].compare("Accept") == 0){this->Accept = spl[1];}
    }
    if (this->req_method.empty() || this->host.empty() || this->request_URI.empty() || this->http_version.empty())
        return false;

    // std::cout << " this->method : |" << this->req_method << "|" << std::endl;
    // std::cout << " this->host : |" << this->host << "|" << std::endl;
    // std::cout << " this->request_URI : |" << this->request_URI << "|" << std::endl;
    // std::cout << " this->http_version : |" << this->http_version << "|" << std::endl;
    // std::cout << " this->Connection : |" << this->Connection << "|" << std::endl;
    // std::cout << " this->Content_Length : |" << this->Content_Length << "|" << std::endl;
    // std::cout << " this->Content_Type : |" << this->Content_Type << "|" << std::endl;
    // std::cout << " this->Transfer_Encoding : |" << this->Transfer_Encoding << "|" << std::endl;
    // std::cout << " this->Accept : |" << this->Accept << "|" << std::endl;
    return true;
}

request::~request()
{
}