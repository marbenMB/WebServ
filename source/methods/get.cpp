#include "../../include/method.hpp"
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>


_Get::_Get(request & _request)
{
    std::ifstream inFile;
    std::string line = "";
    std::string buffer;
    struct stat STATInfo;
    DIR *dirp;
    struct dirent *dp;
    std::string item_name;
    std::string item_URL;
    std::string item_absPATH;
    std::string item_size;
    std::string item_createDATE;
    std::string filename;

    filename.clear();
    filename.append(_request._findHeader(REQUEST_URI));
    if (_request.getRedirect_status() != -1)
    {
        this->setStatus(_request.getRedirect_status());
        filename.clear();
        filename.append(_request.getroot());
        filename.append(_request.getredirect_URL());

        _request._setHeaderReq(REQUEST_URI, filename);
        // set Header : 
        this->addHeader("Location", _request.getredirect_URL());
        line.clear();
    }
    else if (Is_cgi(filename)){ throw request::CGI();}
    else if (stat(filename.c_str(), &STATInfo) != 0){ // not exist
        throw  _Exception(NOT_FOUND);
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFREG) { // is file   S_ISREG(fileStat.st_mode)
        
        // if (Is_cgi(_request.getredirect_URL())){ throw request::CGI();} 
        std::stringstream ssbuf;
        this->setStatus(OK);
        inFile.open(filename.c_str(), std::ifstream::in);
        ssbuf << inFile.rdbuf();
        line.append(ssbuf.str());
        inFile.close();
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFDIR) { // is dir
        filename.append(_request.getdefaultIndex());
        inFile.open(filename.c_str(), std::ifstream::in);
        if (Is_cgi(filename)){ 
            _request._setHeaderReq(REQUEST_URI, _request.getdefaultIndex());
            throw request::CGI();
        }
        else if (!inFile.is_open() && _request.getAutoIndex() == AUTOINDEX_ON) // run AutoIndex 
        {
            std::string pathdir(_request._findHeader(REQUEST_URI));
            
            line.clear();
            inFile.clear();
            inFile.open(AOTUINDEX_PATH, std::ifstream::in);
            if (!inFile.is_open())
                throw _Exception(INTERNAL_SERVER_ERROR); // r************
            while (std::getline(inFile, buffer))
            {
                if (buffer.find("<title>") != std::string::npos) // title
                {
                    // *  <title> ${pathdir} </title>
                    // std::cout << "TITLE" << buffer << std::endl;
                    buffer.clear();
                    buffer.append("     <title>");
                    buffer.append(pathdir, pathdir.length());
                    buffer.append("</title>\n");
                }
                if (buffer.find("<tbody id=") != std::string::npos) // start of the  table body
                {
                    // *  <tbody id="tbody">
                    line.append(buffer);
                    break;
                }
                line.append(buffer);
            }

            inFile.close();
            dirp = opendir(pathdir.c_str());
            if (dirp == NULL){throw _Exception(INTERNAL_SERVER_ERROR);}
            std::string request_URITmp;

            request_URITmp.append(_request._findHeader(REQUEST_URI));
            (request_URITmp[request_URITmp.length() - 1] == '/')
                ? request_URITmp.erase(request_URITmp.length() - 1, request_URITmp.length())
                : request_URITmp;

            while ((dp = readdir(dirp)) != NULL)
            {
                /*
                    <tr>
                        <td data-value="${item_name}"><a class="icon file" draggable="true" href="${item_path}">${item_name}</a></td>
                        <td class="detailsColumn" data-value="${item_size}">${item_size} B</td>
                        <td class="detailsColumn" data-value="1672423565">${item_createDate}</td>
                    </tr>
                **/
                item_name.clear();
                item_name.append(dp->d_name, strlen(dp->d_name));
                // std::cout << "item_name :" << item_name << std::endl;
                // ? ./var/

                item_absPATH.clear();
                // item_absPATH.append(_request.getroot());
                item_absPATH.append(_request._findHeader(REQUEST_URI));
                item_absPATH.append("/");
                item_absPATH.append(item_name);
         
                item_URL.clear();

                if (_request.getcompare_URI().compare("/") != 0){
                    item_URL.append(_request.getcompare_URI());
                }
                item_URL.append(item_absPATH.substr( _request.getroot().length(), item_absPATH.length()));
                // std::cout << "item_URL :" << item_URL << std::endl;

                struct stat STATFile;
                std::string filePATH;
                if (stat(item_absPATH.c_str(), &STATFile) != 0)
                {
                    std::cout << " |" << filePATH << "| file Not found \n";
                    // continue;
                }
                item_size.clear();
                item_size.append(ft_to_string(STATFile.st_size));

                item_createDATE.clear();
                item_createDATE.append(ctime(&STATFile.st_mtime));

                std::string d_nameTmp(dp->d_name);
                if (d_nameTmp.compare(".") != 0 || d_nameTmp.compare("..") != 0)
                {
                    filePATH.clear();
                    filePATH.append(request_URITmp);
                    filePATH.append("/");
                    filePATH.append(dp->d_name);
                    // std::cout << "filePATH :" << filePATH << std::endl;
                    // std::cout << "FILE PATH :" << filePATH << std::endl;
                    if (stat(filePATH.c_str(), &STATFile) != 0)
                    {
                        std::cout << " |" << filePATH << "| file Not found \n";
                        continue;
                    }
                    line.append("<tr> <td data-value=\"");
                    line.append(dp->d_name);
                    line.append("\"><a class=\"icon file\" draggable=\"true\" href=\"");
                    filePATH.erase(0, strlen(_request.getroot().c_str()));
                    // line.append(_request.getcompare_URI(), _request.getcompare_URI().length());
                    line.append(item_URL);
                    line.append("\">");
                    line.append(dp->d_name);
                    line.append("</a></td>");
                    line.append("<td class=\"detailsColumn\" >");
                    line.append(ft_to_string(STATFile.st_size));
                    line.append(" B</td>");
                    line.append("<td class=\"detailsColumn\" data-value=\"1672423565\"> ");
                    line.append(ctime(&STATFile.st_mtime));
                    line.append("</td></tr>\n\r");
                }
            }

            if (closedir(dirp) == -1)
            {
                perror("closedir");
                throw _Exception(INTERNAL_SERVER_ERROR);
            }
            dp = NULL;
            line.append("</tbody></table></body></html>");
            this->setStatus(OK);
           
        }
        else if (inFile.is_open())
        {
            // std::cout << "Open PATH : " << filename << std::endl;
            this->setStatuscode(200);
            this->setreason_phrase("Ok");
            buffer.clear();
            line.clear();
            while (std::getline(inFile, buffer))
            {
                // std::cout << buffer << std::endl;
                line.append(buffer);
            }
            // std::cout << "</Line >" << std::endl;
            inFile.close();
        }
        else throw  _Exception(FORBIDDEN);
    }
    else
    {
        std::cout << "Chi7aja Khra ************* * * * * * * * * * \n";
    }
    
    
    this->setResponseBody(line);
    this->execute_method(_request);
    
}

_Get::~_Get(){}
int _Get::execute_method(request _request)
{
    std::string extension;
    size_t pos = _request._findHeader(REQUEST_URI).rfind(".");
    if (pos == 0)
        extension = "html";
    else 
        extension = _request._findHeader(REQUEST_URI).substr(pos + 1, _request._findHeader(REQUEST_URI).length());
    this->addHeader("Content-Type", Assets::__getType(extension));
    this->addHeader("Content-Length", ft_to_string(this->getResponseBody().length()));
    return 1;
}