#include "../../include/request.hpp"
#include "../../include/ft_cgi.hpp"

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
    if (req._findHeader("Cookie").empty())
        Cookie = generate_client_id();
    else
        Cookie = req._findHeader("Cookie");
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
