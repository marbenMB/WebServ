#include "../../include/request.hpp"
#include "../../include/ft_cgi.hpp"

void set_error(method* &resp, int code_status) {
    std::string file;
    std::string error;
    std::stringstream code_status_str;
    std::string status_code;
    code_status_str << code_status;
    file = ERROR_PATH;
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
    resp->addHeader("Content-Length", std::to_string(_body.length()));
}

method * request::CGI::runCGI(request req){
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
        resp->addHeader("Content-Length", std::to_string(_body.length()));
        resp->addHeader("Set-Cookie", Cookie);
    }
    else
       throw _Exception(status);
    return resp;
}
