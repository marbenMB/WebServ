#include "../../include/request.hpp"

method * request::BadRequest::createError(request req) const throw(){
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    method *resp;


    resp = new Error(req);
    // std::cout << "Bad Request***" << std::endl;
    resp->setStatuscode(404);
    resp->setreason_phrase("Not Found");
    resp->setContent_Type("html/text");
    inFile.open(req.getDefault_40x(), std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        _body.append(buffer);
    }
    inFile.close();
    resp->setResponseBody(_body);
    return resp;
}
method * request::NotImplemented::createError(request req) const throw(){
     method *resp = new Error(req);
    std::ifstream inFile;
    std::string buffer;
    std::string _body;

    // std::cout << "Not Implemented" << std::endl;
    resp->setStatuscode(501);
    resp->setreason_phrase("Not Implemented");
    resp->setContent_Type("html/text");
    inFile.open("var/srcs/Notimplimented.html", std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        _body.append(buffer);
    }
    inFile.close();
    resp->setResponseBody(_body);

    return resp;

}
method * request::NotAllowed::createError(request req) const throw(){
    method *resp = new Error(req);
    // std::cout << "Not Allowed" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    resp->setStatuscode(405);
    resp->setreason_phrase("Not Allowed");
    resp->setContent_Type("html/text");
    inFile.open(req.getDefault_40x(), std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        _body.append(buffer);
    }
    inFile.close();
    resp->setResponseBody(_body);

    return resp;

}
method * request::NotFound::createError(request req) const throw(){
    method *resp = new Error(req);
    // std::cout << "Not Found" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    resp->setStatuscode(404);
    resp->setreason_phrase("Not Found");
    resp->setContent_Type("html/text");
    inFile.open(req.getDefault_40x(), std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        _body.append(buffer);
    }
    inFile.close();
    resp->setResponseBody(_body);
    return resp;
}

method * request::Forbiden::createError(request req) const throw(){
    method *resp = new Error(req);
    // std::cout << "Forbiden" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    resp->setStatuscode(403);
    resp->setreason_phrase("Forbiden");
    resp->setContent_Type("html/text");
    inFile.open(req.getDefault_40x(), std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        _body.append(buffer);
    }
    inFile.close();
    resp->setResponseBody(_body);
    return resp;
}

method * request::InternalServerError::createError(request req) const throw(){
    method *resp = new Error(req);
    // std::cout << "Internal Server Error" << std::endl;
    std::ifstream inFile;
    std::string buffer;
    std::string _body;
    resp->setStatuscode(500);
    resp->setreason_phrase("Internal Server Error");
    resp->setContent_Type("html/text");
    inFile.open(req.getDefault_50x(), std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        _body.append(buffer);
    }
    inFile.close();
    resp->setResponseBody(_body);
    return resp;
}