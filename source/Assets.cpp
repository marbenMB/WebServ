/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assets.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasstou <mmasstou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 19:13:05 by mmasstou          #+#    #+#             */
/*   Updated: 2023/03/21 10:08:38 by mmasstou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Assets.hpp"

std::map<std::string, std::string> Assets::_errorList;
std::map<std::string, std::string> Assets::_mimeTypes;

Assets::~Assets(){}
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
        _split = split(buffer, SPLIT_MEME_TYPE);
        Assets::_mimeTypes[_split[1]] = _split[0];
    }
    file.close();
    Assets::_errorList.clear();
    file.open(STATUS_CODE_PATH, std::ifstream::in);
    while (std::getline(file, buffer))
    {
        _split = split(buffer, SPLIT_CODE_ERROR);
        Assets::_errorList[_split[0]] = _split[1];
    }
    file.close();
}

std::string 

Assets::getError(int code)
{
    return Assets::_errorList[ft_to_string(code)];
}

std::string 
Assets::__getType(std::string extension)
{
    if (Assets::_mimeTypes[extension].empty())
        return (DEFAULT_TYPE);
    return Assets::_mimeTypes[extension];
}
