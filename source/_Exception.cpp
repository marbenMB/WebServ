/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _Exception.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasstou <mmasstou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:47:27 by mmasstou          #+#    #+#             */
/*   Updated: 2023/03/17 18:46:55 by mmasstou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/_Exception.hpp"

_Exception::_Exception(int code){
    std::cout << "_Exception constarctor called !" << std::endl;
    this->_ExceptionCode = code;
}

_Exception::~_Exception(){}

void 
_Exception::set_Body(std::string body){
    this->_body = body;
}
void 
_Exception::generateBody(std::string reason_phrase) {
    std::ifstream inFile;
    std::string buffer;
    std::string _body;

    inFile.open(ERROR_PATH, std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append(std::to_string(this->_ExceptionCode));
            buffer.append(" ");
            buffer.append(reason_phrase);
            buffer.append("</title>\n\r");
        }
        if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append(std::to_string(this->_ExceptionCode));
            buffer.append(" ");
            buffer.append(reason_phrase);
            buffer.append("</h1>\n\r");
        }
       
        _body.append(buffer);
        _body.append("\n\r");
    }
    inFile.close();
    this->set_Body(_body);
 }

 
method * 
_Exception::what(request req) throw(){
    

    

    std::ifstream inFile;
    std::string buffer;
    std::string reason(req.getReason(std::to_string(this->_ExceptionCode)));
    method *resp;

    resp = new Error(req);
    // std::cout << "Bad Request***" << std::endl;
    resp->setStatuscode(this->_ExceptionCode);
    resp->setreason_phrase(reason);
    resp->setContent_Type("text/html");
    this->generateBody(reason);
    resp->setResponseBody(this->_body);
    resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", std::to_string(resp->getResponseBody().length()));
    return resp; 
}
