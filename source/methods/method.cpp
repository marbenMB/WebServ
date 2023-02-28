#include "../../include/method.hpp"

method::method(/* args */)
{
}

method::~method()
{
}
void method::setStatuscode(int sCode)
{
    this->statuscode = sCode;
}

void method::setHost(std::string host)
{
    this->host = host;
}
void method::setreason_phrase(std::string reason_phrase)
{
    this->reason_phrase = reason_phrase;
}
void method::setRequest_URI(std::string request_URI)
{
    this->request_URI = request_URI;
}
void method::setHttp_version(std::string http_version)
{
    this->http_version = http_version;
}

int const &method::getStatuscode(void) const
{
    return (this->statuscode);
}

std::string const &method::getHost(void) const
{
    return (this->host);
}

std::string const &method::getreason_phrase(void) const
{
    return (this->reason_phrase);
}
std::string const &method::getRequest_URI(void) const
{
    return (this->request_URI);
}
std::string const &method::getHttp_version(void) const
{
    return (this->http_version);
}

std::string const &method::getRootPath(void) const
{
    return (this->root_path);
}

void method::setRootPath(std::string root_path)
{
    this->root_path = root_path;
}

std::string const &method::getResponseBody(void) const
{
    return (this->responseBody);
}
void method::setResponseBody(std::string reqBody)
{
    this->responseBody = reqBody;
}
std::map<std::string, std::string> const &method::getContent_Type(void) const
{
    return (this->Content_Type);
}
std::string const &method::getTransfer_Encoding(void) const
{
    return (this->Transfer_Encoding); //
}
int const &method::getClient_max_body_size(void) const
{
    return (this->client_max_body_size);
}

int const &method::getContent_Length(void) const
{
    return (this->Content_Length);
}

int const &method::getsocketID(void) const
{
    return (this->socketID);
}
void method::setsocketID(int socketId)
{
    this->socketID = socketId;
}

void method::createresponse(void /*  content_type, code_status, reason_phrase, body*/)
{
    std::string status_line;
    std::string fields;
    std::map<std::string, std::string> Content_Type = this->getContent_Type();

    // status Line :
    status_line = this->getHttp_version();
    status_line.append(" ");
    status_line.append(std::to_string(this->getStatuscode()));
    status_line.append(" ");
    status_line.append(this->getreason_phrase());
    status_line.append("\r\n");

    // Fields :
    fields.append("Content-Type: ");
    fields.append(Content_Type["type"]);
    fields.append("\r\n");
    fields.append("Content-Length: ");
    fields.append(std::to_string(this->getResponseBody().size()));
    fields.append("\r\n\r\n");
    status_line.append(fields);

    // Body :
    status_line.append(this->getResponseBody());

    // std::cout << std::endl << std::endl << status_line << std::endl;
    this->setResponseBody(status_line);
}

std::string _CREATEresponse(std::map<std::string, std::string> content_type, int code_status, std::string reason_phrase, std::string body)
{
    std::string fields;
    // status Line :
    std::string status_line = "HTTP/1.1";
    status_line.append(" ");
    status_line.append(std::to_string(code_status)); // c++11
    status_line.append(" ");
    status_line.append(reason_phrase);
    status_line.append("\n\r");

    // Fields :
    fields.append("Content-Type: ");
    fields.append(content_type["type"]);
    fields.append("\n\r");
    fields.append("Content-Length: ");
    fields.append(std::to_string(body.size()));
    fields.append(CRLF);
    status_line.append(fields);

    // Body :
    status_line.append(body);

    std::cout <<MAUVE << "Response :\n\r" <<  status_line << END_CLR << std::endl;
    return (status_line);
}

int const &method::getRedirect_status(void) const
{
    return this->redirect_status;
}
void method::setRedirect_status(int redirect_status)
{
    this->redirect_status = redirect_status;
}
std::string const &method::getredirect_URL(void) const
{
    return (this->redirect_URL);
}
void method::setredirect_URL(std::string redirect_URL)
{
    this->redirect_URL = redirect_URL;
}
