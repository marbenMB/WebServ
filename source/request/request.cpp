#include "../../include/request.hpp"

request::request(
    int status,
    ServerConf *server,
    std::string _request,
    std::string &response) : autoindex(AUTOINDEX_OFF),
                             __post(NOT_ALLOWED),
                             __delete(NOT_ALLOWED),
                             __get(NOT_ALLOWED),
                             __noImplimented(NOT_ALLOWED)
{
    std::string _requestBody;
    std::string _requestHeader;
    method *reqmethod;
    // std::vector<std::string> req_vector = split(request, "\r\n\r\n");

	std::cout << _request << std::endl;
    try
    {
        try
        {
            /* code */
         
            size_t splitIndex = _request.find(CRLF_2);
            _requestHeader.clear();
            _requestBody.clear();
            if (splitIndex == std::string::npos)
            {
                _requestHeader = _request.substr(0, _request.length());
            }
            else
            {
                _requestHeader = _request.substr(0, splitIndex);
                _requestBody = _request.substr(splitIndex + 4, _request.length());
            }

            // * Init vars :
            // this->requirements = true;

            this->setRedirect_status(-1);
            this->setcompare_URI("");
            this->is_cgi = false;

            this->uploadType();
            this->retrievingsatatuscodeFile();

            this->Verifying_Header(_requestHeader);
			if (status == TIMEOUT)
                throw _Exception(BAD_REQUEST);
            this->Retrieving_requested_resource(server);
            if (this->_findHeader(REQUEST_METHOD).compare("POST") == 0)
                this->Verifying_Body(_requestBody);
            reqmethod = this->execute_request();

        }
        catch(const std::exception& e){throw _Exception(INTERNAL_SERVER_ERROR); }
    }
    catch (_Exception &e){reqmethod = e.what(*this);}
    catch (request::CGI &e)
    {
        try{reqmethod = e.runCGI(*this);}
        catch (_Exception &e){reqmethod = e.what(*this);}
    }
    response = _CREATEresponse(
        reqmethod->getHeader(),
        reqmethod->getStatuscode(),
        reqmethod->getreason_phrase(),
        reqmethod->getResponseBody());
    printServerLogs(*reqmethod);
    delete reqmethod;
}

void request::Verifying_Body(std::string req)
{
    if (req.empty())
        throw _Exception(BAD_REQUEST);
   
    // init the boundary
    t_bodyfile _fileInfo;

    // get contant type :
    _fileInfo._string = _findHeader("Content-Type");
    if (_fileInfo._string.empty()) // check if empty !
        throw _Exception(BAD_REQUEST);

    // find boundary inside content Type : synthax +> Content-Type: multipart/form-data; boundary=[boundary string]
    _fileInfo.tmp_vector = split(_fileInfo._string, ";");
    _fileInfo.ContentType["type"] = (std::string)_fileInfo.tmp_vector[0];

    if (_fileInfo.tmp_vector.size() >= 2)
        _fileInfo.ContentType["boundary"] = trimFront((std::string)_fileInfo.tmp_vector[1], "boundary=");
    
    _fileInfo._string = _findHeader("Content-Length");
    if (_fileInfo._string.empty())
        throw _Exception(BAD_REQUEST);

    _fileInfo.contentLength = atoll(_fileInfo._string.c_str());

    // boundary
    _fileInfo._string = _fileInfo.ContentType["boundary"];
    if (!_fileInfo._string.empty()){
        // start boundary :
        _fileInfo._boundary_start.clear();
        _fileInfo._boundary_start.append("--");
        _fileInfo._boundary_start.append(_fileInfo._string.c_str());
        // end boundary :
        _fileInfo._boundary_end.clear();
        _fileInfo._boundary_end.append(_fileInfo._boundary_start);
        _fileInfo._boundary_end.append("--");
    }
    if ((unsigned long long)req.length() !=  _fileInfo.contentLength || (unsigned long long)req.length() > this->client_max_body_size)
    {
        std::cout << "ANA HANA\n";

        std::cout << "req.length() :" << req.length() << std::endl;
        std::cout << " _fileInfo.contentLength :" <<  _fileInfo.contentLength << std::endl;
        std::cout << "this->client_max_body_size :" << this->client_max_body_size << std::endl;
        throw _Exception(BAD_REQUEST);
    }
    if ( _fileInfo.ContentType["type"].compare("application/x-www-form-urlencoded") == 0)
    {
        url_decode(req);
        this->CGIbody.clear();
        this->CGIbody.append(req);
    }
    else if ( _fileInfo.ContentType["type"].compare("multipart/form-data") == 0)
    {
       
        if (req.find(_fileInfo._boundary_start) == std::string::npos)
            throw _Exception(BAD_REQUEST);
        if (req.find(_fileInfo._boundary_end) == std::string::npos)
            throw _Exception(BAD_REQUEST);

        req = req.substr(0, req.find(_fileInfo._boundary_end));

        _fileInfo.tmp_vector = split(req, _fileInfo._boundary_start);

        //  initialization files
       
        initializationFILES( _fileInfo.tmp_vector);
    }
    else{ // theres no boundary
        std::cout << " :" << _fileInfo.contentLength << "| " << "chi7aja khra ******* * * * * \n";
    }
	this->Content_Type = _fileInfo.ContentType;

}

void request::url_decode(std::string &url)
{
    std::ostringstream unescaped;
    int i = -1;
    while (url[++i])
    {
        if (url[i] == '%')
        {
            std::string hex;
            char *end;
            hex += url[++i];
            hex += url[++i];
            char decoded = strtol(hex.c_str(), &end, 16);
            if (*end != '\0')
                throw _Exception(BAD_REQUEST);
            unescaped << decoded;
        }
        else if (url[i] == '+')
            unescaped << ' ';
        else
            unescaped << url[i];
    }
    url = unescaped.str();
}

void request::Verifying_Header(std::string req)
{
    std::vector<std::string> requestHeaders = split(req, CRLF);

    initializationRequestHeaders(requestHeaders);
    if (_findHeader(HTTP_VERSION).compare("HTTP/1.1") != 0)
            throw _Exception(BAD_REQUEST);
    if (_findHeader(REQUEST_URI).empty() ||
        _findHeader(REQUEST_METHOD).empty() ||
        _findHeader(HTTP_VERSION).empty() ||
        _findHeader("Host").empty()) throw _Exception(BAD_REQUEST);
}

request::~request(){}

std::string request::_findHeader(std::string _header){
        return this->_requestHeaders[_header];
};
void request::_setHeaderReq(std::string key, std::string value){
    this->_requestHeaders[key] = value;
}

void request::printServerLogs(method const & vars){
    std::string color_status;
    if (vars.getStatuscode() == 200)
        color_status = GREEN;
    else
        color_status = RED;
    std::cout << color_status <<  _findHeader("Host").substr(0,  _findHeader("Host").find(":")) << " " <<  _findHeader(REQUEST_METHOD) << " HTTP/1.1 " << vars.getStatuscode() << " " << vars.getreason_phrase() << " " <<  _findHeader(REQUEST_URI) << " " << vars.getResponseBody().length() << END_CLR << std::endl;
}

method *request::execute_request(void)
{
    method *reqmethod = nullptr;
    std::string __body;
   if (this->__noImplimented == NOT_ALLOWED){
        if (_findHeader(REQUEST_METHOD).compare("GET") == 0 && this->__get == ALLOWED)
            reqmethod = new _Get(*this);
        else if (_findHeader(REQUEST_METHOD).compare("DELETE") == 0 && this->__delete == ALLOWED)
            reqmethod = new _Delete(*this);
        else if (_findHeader(REQUEST_METHOD).compare("POST") == 0 && this->__post == ALLOWED)
            reqmethod = new _Post(*this);
        else
            throw  _Exception(METHOD_NO_ALLOWED);
    }
    else 
        throw  _Exception(NOT_IMPLEMENTED);
    return reqmethod;
}

 void request::initializationRequestHeaders(std::vector<std::string> req){
        std::vector<std::string> _split;
        std::vector<std::string>::iterator it = req.begin();
        std::string it_value(*it);

        _split = split(it_value, " ");
        if (_split.size() < 2)
            throw _Exception(BAD_REQUEST); 

        this->_requestHeaders[REQUEST_METHOD] = _split[0];
        this->_requestHeaders[REQUEST_URI] = _split[1];
        this->_requestHeaders[HTTP_VERSION] = _split[2];

       
        url_decode(this->_requestHeaders[REQUEST_URI]);

        size_t spliteRequestURI = this->_requestHeaders[REQUEST_URI].find("?");
        if (spliteRequestURI != std::string::npos){
            this->_requestHeaders[REQUEST_URI] = this->_requestHeaders[REQUEST_URI].substr(spliteRequestURI + 1, this->_requestHeaders[REQUEST_URI].length());
            this->_requestHeaders[PARAMS] = this->_requestHeaders[REQUEST_URI].substr(0, spliteRequestURI);
        }

        while (++it != req.end())
        {
            _split = split((std::string)*it, ": ");
            this->_requestHeaders[_split[0]] = _split[1];
        }
    }

std::map<std::string, std::string> const &request::getContent_Type(void) const
{
    return (this->Content_Type);
}
