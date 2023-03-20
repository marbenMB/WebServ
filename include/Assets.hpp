/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assets.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasstou <mmasstou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:33:05 by mmasstou          #+#    #+#             */
/*   Updated: 2023/03/20 10:03:41 by mmasstou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSETS___
#define ASSETS___

#include "map"
#include <iostream>
#include <cstring>

#include "request.hpp"

#define MIME_TYPE_PATH "conf/mime.types"
#define STATUS_CODE_PATH "conf/status.code.conf"
#define DEFAULT_TYPE "application/octet-stream"
#define SPLIT_MEME_TYPE ": "
#define SPLIT_CODE_ERROR ": "
class Assets
{
private:
    static std::map<std::string, std::string> _errorList;
    static std::map<std::string, std::string> _mimeTypes;

public:
    static std::string getError(int code);
    static std::string __getType(std::string extension);
    Assets();
    ~Assets();
};

#endif