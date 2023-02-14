#include "../../include/request.hpp"

request::request(int socketID, Data *server, std::string request, std::vector<std::string> & response) : Content_Length(-1),
                                                                    autoindex(AUTOINDEX_OFF),
                                                                    __post(NOT_ALLOWED),
                                                                    __delete(NOT_ALLOWED),
                                                                    __get(NOT_ALLOWED)
{
    std::string _requestBody;
    std::string _reaponseBody;
    method * reqmethod;
    
    // * Init vars :
    this->requirements = true;
    this->setsocketID(socketID);
    this->setRedirect_status(-1);
    // * parse Header :
    // 1 - cheack for Header 
    std::cout << MAUVE << "   @VERIFYING  Header" << END_CLR << std::endl;
    this->requirements = this->Verifying_Header(request);
    // 2 - check for body 
    std::cout << MAUVE << "   @VERIFYING  Body" << END_CLR << std::endl;
    if (this->getmethod().compare("POST") == 0 && this->requirements)
        this->Verifying_Body(_requestBody);
    // 3 - Retrieving the requested resource [config File and Data] :
    std::cout << MAUVE << "   @RETRIEVING requested resource" << END_CLR << std::endl;
    this->Retrieving_requested_resource(server);
    // 4 - execute request ; if this->requirements = true;
    std::cout << MAUVE << "   @EXECUTE request" << END_CLR << std::endl;
    reqmethod = this->execute_request();
    // 5 - get status of execution :

    // 6 - create response ;
    std::cout << MAUVE << "   @CREATE response" << END_CLR << std::endl;
    _reaponseBody = _CREATEresponse(
        reqmethod->getContent_Type(),
        reqmethod->getStatuscode(),
        reqmethod->getreason_phrase(),
        reqmethod->getResponseBody()
        );
    std::cout << MAUVE << "   @PUSH socket Id" << END_CLR << std::endl;
    response.push_back(std::to_string(this->getsocketID()));
    std::cout << MAUVE << "   @PUSH response Body" << END_CLR << std::endl;
    response.push_back(_reaponseBody);
    // response = this->create_response();
    // 7 - print server status
    std::string color_status;
    if (reqmethod->getStatuscode() == 200 )
        color_status = GREEN;
    else
        color_status = RED;
    std::cout << color_status << "127.0.0.1 " << this->getmethod() << " HTTP/1.1 " << reqmethod->getStatuscode() << " " << reqmethod->getreason_phrase() << " " << this->getrequest_URI() << END_CLR << std::endl;
    delete reqmethod;
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
    if (this->req_method.empty() || this->host.empty() || this->request_URI.empty() || this->http_version.empty()){
        this->requirements =  false;
        return false;
    }
    
    if (this->request_URI.find(".php") != std::string::npos)
        std::cout << RED <<"  >PHP CGI" << END_CLR << std::endl;
    if (this->request_URI.find(".py") != std::string::npos)
        std::cout << RED <<"  >PY CGI" << END_CLR << std::endl;
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