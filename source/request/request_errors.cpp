#include "../../include/request.hpp"
#include "../../include/ft_cgi.hpp"

void set_error(method* &resp, int code_status) {
    std::string file;
    std::string error;
    std::stringstream code_status_str;
    std::string status_code;
    code_status_str << code_status;
    file = "./var/errors/40x.html";
    if (code_status == 404) {
        error = "This page isn't available";
        status_code = "404 ";
        resp->setreason_phrase("Not Found");
    }
    else {
        error = "Error in the servre";
        status_code = "500 ";
        resp->setreason_phrase("Internal Server Error");
    }
    std::ifstream inFile;
    std::string buffer;
    std::string _body;

    resp->setStatuscode(code_status);
    resp->setContent_Type("html/text");
    inFile.open(file, std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</title>\n\r");
        }
        if (buffer.find("<h1 id=\"error_title\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append(status_code);
            buffer.append(resp->getreason_phrase());
            buffer.append("</h1>\n\r");
        }
        if (buffer.find("<h2 id=\"error_disc\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h2 id=\"error_disc\">");
            buffer.append(error);
            buffer.append("</h2>\n\r");
        }
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    resp->setResponseBody(_body);
    resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str())));
}

method * request::CGI::runCGI(request req) const throw(){
    char** envp;
    std::vector<char*> env_vec;
    std::string _body;
    method *resp;
    std::string Cookie;

    set_env(req, env_vec);
    envp = &env_vec[0];
    resp = new Error(req);
    int status;
    status = run_cgi(envp, _body);
    if (req.getcookie().empty())
        Cookie = generate_client_id();
    else
        Cookie = req.getcookie();
    if (status == SUCCESS) {
        resp->setStatuscode(200);
        resp->setreason_phrase("OK");
        resp->setContent_Type("text/html");
        resp->setResponseBody(_body);
        resp->addHeader("Content-Type", "text/html");
        resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str()) + 1));
        resp->addHeader("Set-Cookie", Cookie);
    }
    else
        set_error(resp, status);
    return resp;
}
method * request::BadRequest::createError(request req) const throw(){
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    std::string _filename;
    std::string error_disc("This page isn't available");
    method *resp;

    resp = new Error(req);
    // std::cout << "Bad Request***" << std::endl;
    resp->setStatuscode(400);
    resp->setreason_phrase(req.getReason("400"));
    resp->setContent_Type("text/html");
    _filename = req.getDefault_40x().empty() ? "./var/errors/40x.html" : req.getDefault_40x();
    inFile.open(_filename, std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</title>\n\r");
        }
        if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</h1>\n\r");
        }
        if (buffer.find("<h2 id=\"error_disc\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h2 id=\"error_disc\">");
            buffer.append(error_disc);
            buffer.append("</h2>\n\r");
        }
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    resp->setResponseBody(_body);
    resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str())));

    return resp;
}
method * request::NotImplemented::createError(request req) const throw(){
     method *resp = new Error(req);
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    std::string error_disc("This page is not Implemented");


    // std::cout << "Not Implemented" << std::endl;
    resp->setStatuscode(501);
    resp->setreason_phrase(req.getReason("501"));
    resp->setContent_Type("text/html");
    inFile.open("var/srcs/Notimplimented.html", std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
         if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append("501 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</title>\n\r");
        }
       if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</h1>\n\r");
        }
        if (buffer.find("<h2 id=\"error_disc\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h2 id=\"error_disc\">");
            buffer.append(error_disc);
            buffer.append("</h2>\n\r");
        }
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    resp->setResponseBody(_body);
 resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str())));
    return resp;

}
method * request::NotAllowed::createError(request req) const throw(){
    method *resp = new Error(req);
    // std::cout << "Not Allowed" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    std::string error_disc("This page isn't allowed");

    resp->setStatuscode(405);
    resp->setreason_phrase(req.getReason("405"));
    resp->setContent_Type("text/html");
    inFile.open("./var/errors/40x.html", std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</title>\n\r");
        }
       if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</h1>\n\r");
        }
        if (buffer.find("<h2 id=\"error_disc\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h2 id=\"error_disc\">");
            buffer.append(error_disc);
            buffer.append("</h2>\n\r");
        }
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    resp->setResponseBody(_body);
    resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str())));
    return resp;

}
method * request::NotFound::createError(request req) const throw(){
    char _dir[PATH_MAX];
    method *resp = new Error(req);
    // std::cout << "Not Found" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    std::string error_disc("This page Not Found");

    resp->setStatuscode(404);
    resp->setreason_phrase(req.getReason("404"));
    resp->setContent_Type("text/html");
    realpath("./var/errors/40x.html", _dir);
    // std::cout <<"*****> |" << _dir << std::endl;
    inFile.open(_dir, std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
         if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append("404 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</title>\n\r");
        }
       if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</h1>\n\r");
        }
        if (buffer.find("<h2 id=\"error_disc\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h2 id=\"error_disc\">");
            buffer.append(error_disc);
            buffer.append("</h2>\n\r");
        }
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    resp->setResponseBody(_body);
     resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str())));
    return resp;
}
method * request::Forbiden::createError(request req) const throw(){
    method *resp = new Error(req);
    // std::cout << "Forbiden" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    std::string error_disc("This page Forbiden");

    resp->setStatuscode(403);
   resp->setreason_phrase(req.getReason("403"));
    resp->setContent_Type("text/html");
    inFile.open("./var/errors/40x.html", std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
         if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append("403 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</title>\n\r");
        }
       if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</h1>\n\r");
        }
        if (buffer.find("<h2 id=\"error_disc\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h2 id=\"error_disc\">");
            buffer.append(error_disc);
            buffer.append("</h2>\n\r");
        }
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    resp->setResponseBody(_body);
     resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str())));
    return resp;
}
method * request::InternalServerError::createError(request req) const throw(){
    method *resp = new Error(req);
    // std::cout << "Internal Server Error" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    std::string error_disc("Error in the servre");

    resp->setStatuscode(500);
    resp->setreason_phrase(req.getReason("500"));
    resp->setContent_Type("text/html");
    inFile.open("./var/errors/50x.html", std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
         if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append("500 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</title>\n\r");
        }
       if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append("405 ");
            buffer.append(resp->getreason_phrase());
            buffer.append("</h1>\n\r");
        }
        if (buffer.find("<h2 id=\"error_disc\">") != std::string::npos){
            buffer.clear();
            buffer.append("<h2 id=\"error_disc\">");
            buffer.append(error_disc);
            buffer.append("</h2>\n\r");
        }
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    resp->setResponseBody(_body);
    resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(strlen(resp->getResponseBody().c_str())));
    return resp;
}