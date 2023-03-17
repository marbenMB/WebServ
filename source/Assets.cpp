/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assets.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasstou <mmasstou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 19:13:05 by mmasstou          #+#    #+#             */
/*   Updated: 2023/03/16 19:30:11 by mmasstou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Assets.hpp"

std::map<std::string, std::string> Assets::_errorList;
std::map<std::string, std::string> Assets::_mimeTypes;
    
Assets::Assets()
{
    // 
    std::ifstream file;
    std::string buffer;
    std::vector<std::string> _split;

    

    Assets::_mimeTypes.clear();
    file.open(MIME_TYPE_PATH, std::ifstream::in);
    while (std::getline(file, buffer))
    {
        _split = split(buffer, ": ");
        Assets::_mimeTypes[_split[1]] = _split[0];
    }
    Assets::_errorList.clear();
    file.open(STATUS_CODE_PATH, std::ifstream::in);
    while (std::getline(file, buffer))
    {
        _split = split(buffer, ": ");
        Assets::_errorList[_split[0]] = _split[1];
    }
}

Assets::~Assets()
{
}


std::string Assets::getError( int code ){
   return Assets::_errorList[std::to_string(code)];
}
std::string Assets::__getType(std::string extension){
    return Assets::_mimeTypes[extension];
}