/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _Exception.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasstou <mmasstou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:33:08 by mmasstou          #+#    #+#             */
/*   Updated: 2023/03/23 16:53:45 by mmasstou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __EXCEPTION
#define __EXCEPTION

#include "map"
#include <iostream>
#include <cstring>

#include "Assets.hpp"
#include "method.hpp"
#include "request.hpp"

enum _ErrorsCode
{
    OK = 200,
    CREATED,
    NO_CONTENT = 204,
    MOVED_PERMANENTLY = 301,
    TEMPORARY_REDIRECT = 307,
    BAD_REQUEST = 400,
    FORBIDDEN = 403,
    NOT_FOUND,
    METHOD_NO_ALLOWED,
    REQUEST_TIME_OUT = 408,
    LENGHT_REQUIRED = 411,
    REQUEST_ENTITY_TOO_LARGE = 413,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED,
    BAD_GATEWAY,
    GATEWAY_TIME_OUT = 504
};

class request;
class method;
class _Exception
{
private:
    int _ExceptionCode;
    std::string _filename;
    std::string reason_phrase;
    std::string _body;
    void generateBody();
    void set_Body(std::string);

public:
    _Exception(int code);
    ~_Exception();
    method *what(request req) throw();
};

#endif