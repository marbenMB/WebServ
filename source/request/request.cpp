#include "../../include/request.hpp"
std::string const &request::gethost() const
{
    return (this->host);
}
std::string const &request::getrequest_URI() const
{
    return (this->request_URI);
}
std::string const &request::gethttp_version() const { return (this->http_version); }
std::vector<std::string> const &request::getRequestBody(void) const
{
    return (this->requestBody);
}
void request::setRequestBody(std::vector<std::string> reqBody)
{
    this->requestBody = reqBody;
}

std::string const &request::getContent_Type(void) const
{
    return (this->Content_Type);
}

std::string const &request::getTransfer_Encoding(void) const
{
    return (this->Transfer_Encoding);
}

int const &request::getContent_Length(void) const
{
    return (this->Content_Length);
}
int const &request::getsocketID(void) const
{
    return (this->socketID);
}

void request::setsocketID(int socketId)
{
    this->socketID = socketId;
}
request::request(int socketID, std::string requestHeader) : Content_Length(-1)
{
    // std::cout << "socketID :" << socketID <<std::endl;
    // std::cout << "requestHeader :" << requestHeader <<std::endl;
    // * Init vars :
    this->requirements = true;
    this->root = "/Users/mmasstou/Desktop/webserv";
    this->setsocketID(socketID);

    // * parse Header :
    if (this->Verifying_Header(requestHeader) == false){
        this->requirements = false;
    }
}

request::~request()
{
}

void request::sand(int socketID, std::string body)
{
    // std::cout << "\n+>Body : \n" << body << std::endl ;
    // std::cout << "\n+>socket : \n" << socketID << std::endl ;

    // if (this->requirements == false)
    // {
    //     // std::cout << "\e[1;31m+> 400 Bad Request\n\e[0m";
    //     // std::cout << "mmasstou server :" << this->getrequest_URI() << std::endl;

    //     std::ifstream inFile;
    //     std::string line;
    //     // std::string responseBody;
    //     std::string filename = "/Users/mmasstou/Desktop/webserv/src/40x.html";
    //     // check config file if the method is allowed:
    //     // std::cout << "Request Path :" << filename << std::endl;
    //     // read from server :
    //     inFile.open(filename, std::ifstream::in);
    //     body = "";
    //     while (std::getline(inFile, line))
    //     {
    //         body += line;
    //     }
    //     // close(socketID);
    //     // return;
    // }
    // std::cout << "sand Body1 : \n" << body << std::endl ;

    // std::cout <<  this->getrequest_URI() ;

    int n;
    // response *resp = new response(buffer);
    /* Write a response to the client */
    // std::string header ("")
    // std::string body = ;
    // send(newsockfd,)
    // \r\n\r\nHello World webserv\r\n"
  

    // ttt = 
    // printf("the reaponse :\n+>%s<+\n",response);

    // std::cout << "\r\n\r\n\r\n\r\nbody["<<strlen(body.c_str())<<"] : \n" << body.c_str() << std::endl ;
    n = write(socketID, body.c_str(), strlen(body.c_str()));
    // n = write(newsockfd, resp->getBody(), 12);

    // n = write(newsockfd, "HTTP/1.1 200 OK\r\n, Content-Type: text/html\r\n\r\n Hello World\r\n", 59);

    if (n < 0)
    {
        perror("ERROR writing to socket");
        close(socketID);
        exit(1);
    }
    else
    {
        // printf("\n\e[1;32mResponce sand\e[1;32m\n");
        close(socketID);
    }
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

bool request::parseHeader(void)
{
    return true;
}

bool request::parseBody(void)
{
    return true;
}