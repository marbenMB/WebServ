#include "../../include/request.hpp"

request::request(
    int status,
    ServerConf *server,
    std::string _request,
    std::string &response) : autoindex(AUTOINDEX_OFF),
                             __post(NOT_ALLOWED),
                             __delete(NOT_ALLOWED),
                             __get(NOT_ALLOWED),
                             __method_status(IMPLIMENTED),
                             request_status(status) 
{
    std::string _requestBody;
    std::string _requestHeader;
    method *reqmethod;
    time_t time_z;
    struct tm* timeinfo;
    char buffer[80];

    try
    {
        try
        {
            size_t splitIndex = _request.find(CRLF_2);
            _requestHeader.clear();
            _requestBody.clear();
            if (splitIndex == std::string::npos)
                _requestHeader = _request.substr(0, _request.length());
            else
            {
                _requestHeader = _request.substr(0, splitIndex);
                _requestBody = _request.substr(splitIndex + 4, _request.length());
            }

            // * Init vars :
            this->setRedirect_status(-1);
            this->setcompare_URI("");
            this->is_cgi = false;
            this->default_10x = ERROR_PATH;
            this->default_20x = ERROR_PATH;
            this->default_30x = ERROR_PATH;
            this->default_40x = ERROR_PATH;
            this->default_50x = ERROR_PATH;


            this->Verifying_Header(_requestHeader);
            this->Retrieving_requested_resource(server);
            if ((this->_findHeader(REQUEST_METHOD).compare("POST") == 0) && (this->__method_status != NOT_ALLOWED))
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
    // Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
     time(&time_z);
    timeinfo = localtime(&time_z);
    strftime(buffer, 80, "%A, %d %B %Y %H:%M:%S GMT", timeinfo);
    std::string _Time(buffer);
    reqmethod->addHeader("Date", _Time);
    reqmethod->addHeader("Cache-Control", "no-cache");
    if (!this->_findHeader("User-Agent").empty())
        reqmethod->addHeader("User-Agent", this->_findHeader("User-Agent"));
    reqmethod->addHeader("Server", SERVER_NAME);
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
    if (_fileInfo.ContentType["type"].compare("application/x-www-form-urlencoded") != 0 && _fileInfo.ContentType["type"].compare("multipart/form-data") != 0)
        throw _Exception(NOT_IMPLEMENTED);
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
    if ((unsigned long long)req.length() !=  _fileInfo.contentLength )
        throw _Exception(BAD_REQUEST);
    if ((unsigned long long)req.length() > this->client_max_body_size)
        throw _Exception(REQUEST_ENTITY_TOO_LARGE);
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
        std::cout << "3iw hmmmmm\n" ;
    }
    this->Content_Type = _fileInfo.ContentType;
}

void request::initializationFILES(std::vector<std::string> filesVECTER)
{
    std::vector<std::pair<std::string, std::string> > _files;
    std::pair<std::string, std::string>  _filesContent;
    std::vector<std::string> tmp;
    std::vector<std::string> file_header;
    std::vector<std::string> content_disposition;
    std::vector<std::string>::iterator it = filesVECTER.begin();
    size_t pos;

    while (it != filesVECTER.end()){
        try{
        pos = it[0].find(CRLF_2);
        if (pos != std::string::npos){
            tmp.push_back(it[0].substr(0, pos));
            tmp.push_back(it[0].substr(pos + 4, it[0].length()));
        }
        else {
            throw std::invalid_argument("makayench Bady ...");
        }

        file_header = split((std::string)tmp[0], CRLF);
        if (file_header.size() != 2){
            throw std::invalid_argument("makaynach smiya dyal file ...");
        }
        content_disposition = split(file_header[0], "; ");
        std::string filename;
        if (content_disposition.size() > 2 && content_disposition[2].length() > 11) // if post is not empty
        {

            int endfilename = content_disposition[2].length() - 11;

            filename.append(content_disposition[2].substr(10, endfilename));
            _filesContent.first = filename;
            _filesContent.second = tmp[1];
            _files.push_back(_filesContent);
        }
        else{throw std::invalid_argument("3iw haschi khawi");}}
        catch(const std::exception& e){std::cerr << e.what() << '\n';}
        it++;
    }
    if (_files.size())this->req_body = _files;
    else{throw  _Exception(BAD_REQUEST);}
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
    if (_findHeader(HTTP_VERSION).compare("HTTP/1.1") != 0 && request_status == WELL)
            throw _Exception(BAD_REQUEST);
    if ((_findHeader(REQUEST_URI).empty() ||
        _findHeader(REQUEST_METHOD).empty() ||
        _findHeader(HTTP_VERSION).empty() ||
        _findHeader("Host").empty()) && request_status == WELL) throw _Exception(BAD_REQUEST);
     if (request_status == TIMEOUT)
        throw _Exception(REQUEST_TIME_OUT);

    
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
    time_t time_z;
    struct tm* timeinfo;
    char buffer[80];

    time(&time_z);
    timeinfo = localtime(&time_z);
    strftime(buffer, 80, "%H:%M:%S ", timeinfo);
    std::string _Time(buffer);

    color_status.clear();
    if (vars.getStatuscode() >= 200 && vars.getStatuscode() < 300)
        color_status.append(GREEN);
    else if (vars.getStatuscode() >= 300 && vars.getStatuscode() < 400)
        color_status.append(BLUE);
    else if (vars.getStatuscode() >= 400 && vars.getStatuscode() < 500)
        color_status.append(RED);
    else if (vars.getStatuscode() >= 500 && vars.getStatuscode() < 600)
        color_status.append(MAUVE);
        
    std::string _host(_findHeader("Host").substr(0,  _findHeader("Host").find(":")));
    std::string _meth( _findHeader(REQUEST_METHOD));
    std::string _req_uri( _findHeader(REQUEST_URI));

    _req_uri = _req_uri.empty() ? "URIND" : _req_uri;
    _meth = _meth.empty() ? "MND" : _meth;
    _host = _host.empty() ? "HND": _host;
    
    std::cout << color_status << _Time << ": " <<  _host << " " <<  _meth << " HTTP/1.1 " << vars.getStatuscode() << " " << vars.getreason_phrase() << " " << _req_uri << " " << vars.getResponseBody().length() << END_CLR << std::endl;
}

method *request::execute_request(void)
{
    method *reqmethod = NULL;
    std::string __body;
   if (this->__method_status != NOT_IMPLEMENTED){
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
        std::string URI( _findHeader(REQUEST_URI));
        url_decode(this->_requestHeaders[REQUEST_URI]);
        size_t spliteRequestURI = URI.find("?");
        if (spliteRequestURI != std::string::npos){
            this->_setHeaderReq(REQUEST_URI, URI.substr(0, spliteRequestURI));
            this->_setHeaderReq(PARAMS, URI.substr(spliteRequestURI + 1, URI.length()));
            url_decode(this->_requestHeaders[PARAMS]);
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

std::string ft_to_string(int entier){
    std::stringstream buf;
    buf << entier;
    return buf.str();
}