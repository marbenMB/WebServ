#include "../../include/request.hpp"

request::request(int socketID, Data *server, std::string _request, std::vector<std::string> &response) : Content_Length(-1),
                                                                                                        autoindex(AUTOINDEX_OFF),
                                                                                                        __post(NOT_ALLOWED),
                                                                                                        __delete(NOT_ALLOWED),
                                                                                                        __get(NOT_ALLOWED)
{
    std::string _requestBody;
    std::string _requestHeader;
    std::string _reaponseBody;
    method *reqmethod;
    // std::vector<std::string> req_vector = split(request, "\r\n\r\n");

    size_t splitIndex = _request.find("\r\n\r\n");
     _requestHeader.clear();
    _requestBody.clear();
    if (splitIndex == std::string::npos){
        _requestHeader = _request.substr(0, _request.length());
    }
    else
    {
         _requestHeader = _request.substr(0, splitIndex);
         _requestBody = _request.substr(splitIndex + 4, _request.length());
    }
    // std::cout << "BODY****>" << _request << std::endl;
    // _requestHeader.clear();
    // _requestBody.clear();
    // _requestHeader.append(req_vector[0]);
    // std::cout << _requestHeader << std::endl;

    // int i = 0;

    // while (++i < (int)req_vector.size())
    // {
    //     _requestBody.append(req_vector[i]);
    //     if (i != (int)req_vector.size() - 1)
    //         _requestBody.append("\n\r\n\r");
    //     // std::cout << RED << "<req id=" << i << " size=" << requestheader[i].size() << ">\n"
    //     //           << END_CLR << requestheader[i] << RED << "\n</req>" << END_CLR << std::endl;
    // }
    // * Init vars :
    // this->requirements = true;
    this->setsocketID(socketID);
    this->setRedirect_status(-1);
    this->setcompare_URI("");
    this->is_cgi = false;
    try
    {
        this->uploadType();
        this->retrievingsatatuscodeFile();
        this->Verifying_Header(_requestHeader);
        this->Retrieving_requested_resource(server);
        if (this->getmethod().compare("POST") == 0)
        {
            // exit(1);
            this->Verifying_Body(_requestBody);
        }
        reqmethod = this->execute_request();

    }
    catch(request::BadRequest & e){ reqmethod = e.createError(*this);}
    catch(request::NotImplemented & e){ reqmethod = e.createError(*this);}
    catch(request::NotAllowed & e){ reqmethod = e.createError(*this);}
    catch(request::NotFound & e){ reqmethod = e.createError(*this);}
    catch(request::Forbiden & e){ reqmethod = e.createError(*this);}
    catch(request::InternalServerError & e){ reqmethod = e.createError(*this);}
    catch(request::CGI & e){ reqmethod = e.runCGI(*this);}
    
    // * parse Header :
    // 1 - cheack for Header
    // std::cout << MAUVE << "   @VERIFYING  Header" << END_CLR << std::endl;
    // 3 - Retrieving the requested resource [config File and Data] :
    // std::cout << MAUVE << "   @RETRIEVING requested resource" << END_CLR << std::endl;
    // 2 - check for body
    // std::cout << MAUVE << "   @VERIFYING  Body" << END_CLR << std::endl;
    // 4 - execute request ; if this->requirements = true;
    // std::cout << MAUVE << "   @EXECUTE request" << END_CLR << std::endl;
    // 5 - get status of execution :

    // 6 - create response ;
    // std::cout << MAUVE << "   @CREATE response" << END_CLR << std::endl;
    // exit(1);
    // std::cout << RED << "NOT :|" << reqmethod->getResponseBody() << std::endl;

    _reaponseBody = _CREATEresponse(
        reqmethod->getHeader(),
        reqmethod->getStatuscode(),
        reqmethod->getreason_phrase(),
        reqmethod->getResponseBody());
    std::ofstream _outfile;


    _outfile.open("file", std::ifstream::out);

    _outfile << _reaponseBody ;


    // std::cout << MAUVE << "**>|" << _reaponseBody << END_CLR << std::endl;
    response.push_back(std::to_string(this->getsocketID()));
    // std::cout << MAUVE << "   @PUSH response Body" << END_CLR << std::endl;
    response.push_back(_reaponseBody);
    // response = this->create_response();
    // 7 - print server status
    std::string color_status;
    if (reqmethod->getStatuscode() == 200)
        color_status = GREEN;
    else
        color_status = RED;
    std::cout << color_status << "127.0.0.1 " << this->getmethod() << " HTTP/1.1 " << reqmethod->getStatuscode() << " " << reqmethod->getreason_phrase() << " " << this->getrequest_URI() << END_CLR <<std::endl ;
    delete reqmethod;
}

void request::sand(int socketID, std::string body)
{
    int n;

    n = write(socketID, body.c_str(),body.length());
    if (n < 0)
    {
        perror("ERROR writing to socket");
        close(socketID);
        // exit(1);
    }
    else
    {
        close(socketID);
    }
}

bool request::Verifying_Body(std::string req)
{
    /** 
     * ! Body synthax 
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
    //  ! check redirection && CGI :

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

   
    // std::cout <<"ContentType['type']    " << ContentType["type"] << std::endl;
    // std::cout <<"ContentType['boundary']" << ContentType["boundary"] << std::endl;
    // std::cout <<"STRINGSEPARATES        " << STRINGSEPARATES << std::endl;
    // std::cout <<"EndSTRINGSEPARATES     " << EndSTRINGSEPARATES << std::endl;
    // if ((int)req.length() != this->getContent_Length()){this->requirements = false;return false;}
    // std::cout << req << std::endl;
    if ((unsigned long long)req.length() != this->getContent_Length() || (unsigned long long)req.length() > this->client_max_body_size){
        throw BadRequest();
    }
    if (ContentType["type"].compare("application/x-www-form-urlencoded") == 0){
        this->CGIbody.clear();
        this->CGIbody.append(req);
    }
    else if (ContentType["type"].compare("multipart/form-data") == 0)
    {
        if (ContentType["boundary"].length() != 0){
            // std::cout << "REQUEST :" << req <<std::endl;
            STRINGSEPARATES.clear();
            STRINGSEPARATES.append("--");
            STRINGSEPARATES.append(ContentType["boundary"]);
            EndSTRINGSEPARATES.clear();
            EndSTRINGSEPARATES.append(STRINGSEPARATES);
            EndSTRINGSEPARATES.append("--");
            if (req.find(STRINGSEPARATES) == std::string::npos || req.find(EndSTRINGSEPARATES) == std::string::npos){

                throw BadRequest();
            }

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
                std::vector<std::string> _file = split(it[0], CRLF_2);
                // std::cout << "\nBODY :\n" << _file[1] << std::endl ;
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
                std::vector<std::string> __fileHeader = split(_file[0], CRLF);
              
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
                    std::cout << RED << "ContentDisposition[" << nndex << "] :" << END_CLR << *ContentDisposition_iter << std::endl;
                    /**
                     * *ContentDisposition[1] :form-data
                     * *ContentDisposition[2] :name=""
                     * !ContentDisposition[3] :filename="request.hpp"
                     */
                    if ((*ContentDisposition_iter).find("filename=") != std::string::npos){
                        std::string filenameee(ContentDisposition_iter[0]);
                        filenameee.erase(0, 10);
                        int filename_length = filenameee.length();
                        filenameee.erase(filename_length - 1, filename_length);
                        tmp_fileIt.first = filenameee;
                        tmp_fileIt.second = _file[1];
                        this->req_body.push_back(tmp_fileIt);
                    }
                    ContentDisposition_iter++;nndex++;
                }
                ++it;++index;
            }
        }
    }
    else{ // theres no boundary
    }
    return true;
}

void request::url_decode(std::string &url) {
    std::ostringstream unescaped;
    int i = -1;
    while (url[++i]) {
        if (url[i] == '%') {
            std::string hex;
            char *end;
            hex += url[++i];
            hex += url[++i];
            char decoded = strtol(hex.c_str(), &end, 16);
            if (*end != '\0')
                throw BadRequest();
            unescaped << decoded;
        }
        else if (url[i] == '+')
            unescaped << ' ';
        else
            unescaped << url[i];
    }
    url = unescaped.str();
}

bool request::Verifying_Header(std::string req)
{
//    std::cout << "Header :"<< req << std::endl;
    std::vector<std::string> requestHeaders = split(req, "\r\n");
    std::vector<std::string>::iterator itH = requestHeaders.begin();
    std::vector<std::string> spl;

    spl = split((std::string)itH[0], " ");
    this->req_method = spl[0];
   
    std::string _request_URI = spl[1];

    // std::cout << "request_URI : |" << _request_URI << std::endl; 
    url_decode(_request_URI);
    // char _ABSdir[PATH_MAX];
    // char _SubDir[PATH_MAX];
    // realpath("./var", _ABSdir);
    // std::string Tmp("./var");
    // Tmp.append(_request_URI);
    // realpath(Tmp.c_str(), _SubDir);
    // _ABSdir
    size_t spliteRequestURI =  _request_URI.find("?");
    if (spliteRequestURI != std::string::npos){

        this->setquery_string(_request_URI.substr(spliteRequestURI + 1, _request_URI.length()));
        this->setrequest_URI(_request_URI.substr(0, spliteRequestURI));
    }
    else{this->setrequest_URI(_request_URI); }

    // std::cout << "request_URI : |" << this->getrequest_URI() << std::endl; 
    //     throw NotAllowed();
    // this->_error.setCode_status(404);
    // this->_error.setReason_phrase("Bad Request");
    // this->_error = Error(401, "Bad Request");

    this->http_version = spl[2];
    if (this->http_version.compare("HTTP/1.1") != 0)
        throw BadRequest();
    // std::cout << "\nsetquery_string :" << this->getquery_string() << std::endl;
    // std::cout << "setrequest_URI :" << this->getrequest_URI() << std::endl;
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
        // this->_error = Error(401, "Bad Request");
        this->requirements = false;
        throw BadRequest();
        return false;
    }


    // if (this->request_URI.find(".go") != std::string::npos)
    //     std::cout << RED << "  >Go CGI" << END_CLR << std::endl;
    // if (this->request_URI.find(".py") != std::string::npos)
    //     std::cout << RED << "  >PY CGI" << END_CLR << std::endl;
    // std::cout << std::endl;
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
