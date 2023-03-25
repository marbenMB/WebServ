/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _Exception.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasstou <mmasstou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:47:27 by mmasstou          #+#    #+#             */
/*   Updated: 2023/03/22 15:34:29 by mmasstou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/_Exception.hpp"

_Exception::_Exception(int code){
    this->_ExceptionCode = code;
    this->reason_phrase = "";
}

_Exception::~_Exception(){}

void 
_Exception::set_Body(std::string body){
    this->_body = body;
}

void 
_Exception::generateBody() {
    std::ifstream inFile;
    std::string buffer;
    std::string _body;

    inFile.open(ERROR_PATH, std::ifstream::in);
    while (std::getline(inFile, buffer))
    {
        if (buffer.find("<title>") != std::string::npos){
            buffer.clear();
            buffer.append("<title> ");
            buffer.append(ft_to_string(this->_ExceptionCode));
            buffer.append(" ");
            buffer.append(reason_phrase);
            buffer.append("</title>\r\n");
        }
        if (buffer.find("id=\"error_title\"") != std::string::npos){
            buffer.clear();
            buffer.append("<h1 id=\"error_title\">");
            buffer.append(ft_to_string(this->_ExceptionCode));
            buffer.append(" ");
            buffer.append(reason_phrase);
            buffer.append("</h1>\r\n");
        }
        _body.append(buffer);
    }
    inFile.close();
    this->set_Body(_body);
 }

 
method * 
_Exception::what(request req) throw(){
    this->reason_phrase =  Assets::getError(this->_ExceptionCode);
    method *resp;

    resp = new Error(req);
    resp->setStatus(this->_ExceptionCode);
    this->generateBody();
    resp->setResponseBody(this->_body);
    resp->addHeader("Content-Type", "text/html");
    resp->addHeader("Content-Length", ft_to_string(resp->getResponseBody().length()));
    // system("leaks webServ");
    return resp; 
}
