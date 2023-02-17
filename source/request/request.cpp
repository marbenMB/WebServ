#include "../../include/request.hpp"

request::request(int socketID, Data *server, std::string request, std::vector<std::string> &response) : Content_Length(-1),
                                                                                                        autoindex(AUTOINDEX_OFF),
                                                                                                        __post(NOT_ALLOWED),
                                                                                                        __delete(NOT_ALLOWED),
                                                                                                        __get(NOT_ALLOWED)
{
    std::string _requestBody;
    std::string _requestHeader;
    std::string _reaponseBody;
    method *reqmethod;
    std::vector<std::string> req_vector = split(request, "\r\n\r\n");
    // std::cout << "BODY****>" << request << std::endl;
    _requestHeader.clear();
    _requestBody.clear();
    _requestHeader.append(req_vector[0]);

    int i = 0;

    while (++i < (int)req_vector.size())
    {
        _requestBody.append(req_vector[i]);
        if (i != (int)req_vector.size() - 1)
            _requestBody.append("\n\r\n\r");
        // std::cout << RED << "<req id=" << i << " size=" << requestheader[i].size() << ">\n"
        //           << END_CLR << requestheader[i] << RED << "\n</req>" << END_CLR << std::endl;
    }

    // * Init vars :
    this->requirements = true;
    this->setsocketID(socketID);
    this->setRedirect_status(-1);
    // * parse Header :
    // 1 - cheack for Header
    std::cout << MAUVE << "   @VERIFYING  Header" << END_CLR << std::endl;
    this->requirements = this->Verifying_Header(_requestHeader);
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
        reqmethod->getResponseBody());
    std::cout << MAUVE << "   @PUSH socket Id" << END_CLR << std::endl;
    response.push_back(std::to_string(this->getsocketID()));
    std::cout << MAUVE << "   @PUSH response Body" << END_CLR << std::endl;
    response.push_back(_reaponseBody);
    // response = this->create_response();
    // 7 - print server status
    std::string color_status;
    if (reqmethod->getStatuscode() == 200)
        color_status = GREEN;
    else
        color_status = RED;
    std::cout << color_status << "127.0.0.1 " << this->getmethod() << " HTTP/1.1 " << reqmethod->getStatuscode() << " " << reqmethod->getreason_phrase() << " " << this->getrequest_URI() << END_CLR << std::endl;
    delete reqmethod;
}

void request::sand(int socketID, std::string body)
{
    int n;

    n = write(socketID, body.c_str(), strlen(body.c_str()));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        close(socketID);
        exit(1);
    }
    else
    {
        close(socketID);
    }
}

bool request::Verifying_Body(std::string req)
{
    /**
        HTTP/1.1 206 Partial Content
        Date: Wed, 15 Nov 1995 06:25:24 GMT
        Last-Modified: Wed, 15 Nov 1995 04:58:08 GMT
        Content-Length: 1741
        Content-Type: multipart/byteranges; boundary=THIS_STRING_SEPARATES

        --THIS_STRING_SEPARATES
        Content-Type: application/pdf
        Content-Range: bytes 500-999/8000

        ...the first range...
        --THIS_STRING_SEPARATES
        Content-Type: application/pdf
        Content-Range: bytes 7000-7999/8000

        ...the second range
        --THIS_STRING_SEPARATES--
    */

    // init the boundary
    std::map<std::string, std::string> ContentType;
    std::string STRINGSEPARATES;
    std::string EndSTRINGSEPARATES;
    std::string request;
    std::vector<std::string> tmp = split(Content_Type, ";");
    std::pair<std::string, std::string> tmp_fileIt;
    ContentType["type"] = (std::string)tmp[0];
    if (tmp.size() == 2)
        ContentType["boundary"] = trimFront((std::string)tmp[1], "boundary=");

    STRINGSEPARATES.clear();
    STRINGSEPARATES.append("--");
    STRINGSEPARATES.append(ContentType["boundary"]);
    EndSTRINGSEPARATES.clear();
    EndSTRINGSEPARATES.append(STRINGSEPARATES);
    EndSTRINGSEPARATES.append("--");
    // std::cout <<"ContentType['type']    " << ContentType["type"] << std::endl;
    // std::cout <<"ContentType['boundary']" << ContentType["boundary"] << std::endl;
    // std::cout <<"STRINGSEPARATES        " << STRINGSEPARATES << std::endl;
    // std::cout <<"EndSTRINGSEPARATES     " << EndSTRINGSEPARATES << std::endl;
    // if ((int)req.length() != this->getContent_Length()){this->requirements = false;return false;}
    // std::cout << req << std::endl;
    if (ContentType["type"].compare("multipart/form-data") == 0)
    {
        if (!ContentType["boundary"].length()){this->requirements = false;return false;}
        if (req.find(STRINGSEPARATES) == std::string::npos){this->requirements = false;std::cout << "STRINGSEPARATES: makayen " << std::endl;}
        if (req.find(EndSTRINGSEPARATES) == std::string::npos){this->requirements = false;std::cout << "EndSTRINGSEPARATES: makayen " << std::endl;}
        tmp.clear();
        request = split(req, EndSTRINGSEPARATES)[0];
        STRINGSEPARATES.append("\r\n");
        tmp = split(request, STRINGSEPARATES);
        std::vector<std::string>::iterator it = tmp.begin();
        int index = 1;
        std::vector<std::string>::iterator tmp_IT;
        int indexs, nndex;
        while (it != tmp.end())
        {
            // std::cout << RED <<"boundary["<<index<<"]"<<END_CLR << it[0] << std::endl;
            std::vector<std::string> _file = split(it[0], "\n\r");
            std::cout << "\nBODY :\n" << _file[0] << std::endl ;
            // std::cout << "_file size :" << _file.size() << std::endl;
            /**
            *   ! _file[0]
                Content-Disposition: form-data; name=""; filename="get.cpp"
                Content-Type: text/x-c
            *   * _file[1]
                file Body
            */
            indexs = 1;
            tmp_IT = _file.begin();
            while (tmp_IT != _file.end())
            {
                //  std::cout << RED <<"_file["<< indexs <<"] :"<< END_CLR<< *tmp_IT << std::endl;
                ++tmp_IT;indexs++;
            }
            std::vector<std::string> __fileHeader = split(_file[0], "\n");
            // __fileHeader = split(__fileHeader[0], "\n");
            // std::cout << "__fileHeader :" << __fileHeader.size() << std::endl;
            /**
             * !   __fileHeader[0] :Content-Disposition: form-data; name=""; filename="request.hpp"
             * *   __fileHeader[1] :Content-Type: application/octet-stream
             */
            indexs = 1;
            tmp_IT = __fileHeader.begin();
            std::vector<std::string> ContentDisposition_vect = split(split(tmp_IT[0], ":")[1], ";");
            while (tmp_IT != __fileHeader.end())
            {
                // std::cout << RED << "__fileHeader[" << indexs << "] :" << END_CLR << *tmp_IT << std::endl;
                ++tmp_IT;indexs++;
            }
            std::vector<std::string>::iterator ContentDisposition_iter = ContentDisposition_vect.begin();
            nndex = 1;
            // std::cout <<RED<< "Name =" <<END_CLR<< ContentDisposition_vect[1] << std::endl;
            while (ContentDisposition_iter != ContentDisposition_vect.end())
            {
                // std::cout << RED << "ContentDisposition[" << nndex << "] :" << END_CLR << *ContentDisposition_iter << std::endl;
                /**
                 * *ContentDisposition[1] :form-data
                 * *ContentDisposition[2] :name=""
                 * !ContentDisposition[3] :filename="request.hpp"
                 */
                if ((*ContentDisposition_iter).find("filename=") != std::string::npos){
                    std::string filenameee(ContentDisposition_iter[0]);
                    filenameee.erase(0, 10);
                    int filename_length = filenameee.length();
                    filenameee.erase(filename_length - 2, filename_length);
                    tmp_fileIt.first = filenameee;
                    tmp_fileIt.second = _file[1];
                    this->req_body.push_back(tmp_fileIt);

                    // std::cout <<RED<< "Body       =" <<END_CLR<< _file[1] << std::endl;
                }
                ContentDisposition_iter++;nndex++;
            }
            // ContentDisposition_iter += 2;
          
            ++it;++index;
        }
    }
    else{ // theres no boundary
    }

    return true;
}

bool request::Verifying_Header(std::string req)
{
   std::cout << "Header :"<< req << std::endl;
    std::vector<std::string> requestHeaders = split(req, "\r\n");
    std::vector<std::string>::iterator itH = requestHeaders.begin();
    std::vector<std::string> spl;

    spl = split((std::string)itH[0], " ");
    this->req_method = spl[0];
    this->request_URI = spl[1];
    this->http_version = spl[2];
    while (++itH != requestHeaders.end())
    {
        spl.clear();
        spl = split((std::string)*itH, ": ");
        if (spl[0].compare("Host") == 0){this->host = spl[1];}
        if (spl[0].compare("User-Agent") == 0){this->User_Agent = spl[1];}
        if (spl[0].compare("Accept-Encoding") == 0){this->Accept_Encoding = spl[1];}
        if (spl[0].compare("Connection") == 0){this->Connection = spl[1];}
        if (spl[0].compare("Content-Length") == 0){this->Content_Length = atoi(spl[1].c_str());}
        if (spl[0].compare("Content-Type") == 0){this->Content_Type = spl[1];}
        if (spl[0].compare("Content-Transfer-Encoding") == 0){this->Content_Transfer_Encoding = spl[1];}
        if (spl[0].compare("Transfer-Encoding") == 0){this->Transfer_Encoding = spl[1];}
        if (spl[0].compare("Accept") == 0){this->Accept = spl[1];}
    }
    if (this->req_method.empty() || this->host.empty() || this->request_URI.empty() || this->http_version.empty())
    {
        this->requirements = false;
        return false;
    }
    if (this->request_URI.find(".php") != std::string::npos)
        std::cout << RED << "  >PHP CGI" << END_CLR << std::endl;
    if (this->request_URI.find(".py") != std::string::npos)
        std::cout << RED << "  >PY CGI" << END_CLR << std::endl;
    std::cout << std::endl;
    // std::cout << " this->method : |" << this->req_method << "|" << std::endl;
    // std::cout << " this->host : |" << this->host << "|" << std::endl;
    std::cout << " this->request_URI : |" << this->request_URI << "|" << std::endl;
    // std::cout << " this->http_version : |" << this->http_version << "|" << std::endl;
    // std::cout << " this->Connection : |" << this->Connection << "|" << std::endl;
    // std::cout << " this->Content_Length : |" << this->Content_Length << "|" << std::endl;
    std::cout << " this->Content_Type : |" << this->Content_Type << "|" << std::endl;
    // std::cout << " this->Transfer_Encoding : |" << this->Transfer_Encoding << "|" << std::endl;
    // std::cout << " this->Accept : |" << this->Accept << "|" << std::endl;
    return true;
}

request::~request()
{
}