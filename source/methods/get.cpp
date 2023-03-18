#include "../../include/method.hpp"
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>


_Get::_Get(request rhs)
{
    
    this->execute_method(rhs);
    /**
     *
     *! Verifying_Header that the request method is indeed "GET" and the URI is valid.
     *! Retrieving the requested resource from the server.
     *! Creating the response headers, including the "Content-Type" header to indicate the format of the response and any other headers relevant to the response.
     *! Creating the response message body, which should contain the requested resource.
     *! Sending the complete response message back to the client.
     */
}

_Get::~_Get()
{
}
/* -------------------------------------------------------------------------- */
/*                            execute_method                            */
/* -------------------------------------------------------------------------- */

int _Get::execute_method(request _request)
{
    std::ifstream inFile;
    std::string line = "";
    std::string buffer;
    struct stat STATInfo;
    DIR *dirp;
    struct dirent *dp;



    // ! item_name
    // ! item_absPATH
    // ! item_URL
    // ! item_size
    // ! item_createDate
    std::string item_name;
    std::string item_URL;
    std::string item_absPATH;
    std::string item_size;
    std::string item_createDATE;


    // size_t pos = 0;
    // std::string responseBody;
    // check config file if the method is allowed:
    // if (this->getRequest_URI().compare("/") == 0){
    //     this->setRequest_URI("/index.html");
    // }
    std::string filename;
    filename.clear();
    filename.append(_request.getrequest_URI());
    if (_request.getRedirect_status() != -1)
    {
        /*
        <!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>301 Redirction</title></head><body><h1>301 Redirction</h1></body></html>
        **/ 
        this->setStatuscode(_request.getRedirect_status());
        this->setreason_phrase(_request.getReason(std::to_string(_request.getRedirect_status())));
        filename.clear();
        filename.append(_request.getroot());
        filename.append(_request.getredirect_URL());

        _request.setrequest_URI(filename);
        // set Header : 
        this->addHeader("Location", _request.getredirect_URL());
        line.append("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>");
        line.append(std::to_string(this->getStatuscode()));
        line.append(" Redirction</title></head><body><h1>");
        line.append(std::to_string(this->getStatuscode()));
        line.append(" Redirction</h1></body></html>");
    }
    else if (Is_cgi(filename)){ throw request::CGI();}
    else if (stat(filename.c_str(), &STATInfo) != 0){ // not exist
        throw  _Exception(NOT_FOUND);
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFREG) { // is file   S_ISREG(fileStat.st_mode)
        
        if (Is_cgi(_request.getredirect_URL())){ throw request::CGI();} 
        std::stringstream ssbuf;
        
        this->setStatuscode(OK);
        this->setreason_phrase(_request.getReason(std::to_string(this->getStatuscode())));
        inFile.open(filename, std::ifstream::in);
        ssbuf << inFile.rdbuf();
        line.append(ssbuf.str());
        // while (std::getline(inFile, buffer))
        // {
        //     // std::cout << buffer << std::endl;
        //     line.append(buffer);
        // }
        inFile.close();
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFDIR) { // is dir
        // std::cout << "+++++> : " << filename << std::endl;
        filename.append("/");
        filename.append(_request.getdefaultIndex());
        inFile.open(filename, std::ifstream::in);
        if (Is_cgi(_request.getdefaultIndex())){ throw request::CGI();}
        else if (!inFile.is_open() && _request.getAutoIndex() == AUTOINDEX_ON) // run AutoIndex 
        {
            std::string pathdir(_request.getrequest_URI());
            // if (pathdir.compare("/") != 0){
            //     pathdir.append("/..");
            // }
            // std::cout << "AUTOINDEX :" << _request.getrequest_URI() << std::endl;
            line.clear();
            inFile.clear();
            inFile.open(AOTUINDEX_PATH, std::ifstream::in);
            if (!inFile.is_open())
                throw request::InternalServerError(); // r************
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
                if (buffer.find("<h1 id=\"header\"") != std::string::npos) // index of
                {
                    //*  <h1 id="header">Index of /Users/mmasstou/Desktop/my-Store/</h1>
                    buffer.clear();
                    buffer.append("<h1 id=\"header\">Index of ");
                    buffer.append(pathdir);
                    buffer.append("</h1>");
                }
                if (buffer.find("<a id=") != std::string::npos) // index of href
                {
                    //* <a id="parentDirLink" class="icon up" href="${}/..">

                    buffer.clear();
                    std::string parentDirLink(pathdir);
                    buffer.append("<a id=\"parentDirLink\" class=\"icon up\" href=\"");
                    // std::cout << "_request.getroot() :" << _request.getroot() << std::endl;
                    // std::cout << "pathdir :" << pathdir << std::endl;
                    size_t start = (parentDirLink.compare("/") == 0) ? std::string::npos :parentDirLink.rfind("/");
                    (start != std::string::npos) ? parentDirLink.erase(start + 1, parentDirLink.length()) : parentDirLink;
                    // std::cout << "parentDirLink :" << parentDirLink << std::endl;
                    parentDirLink.erase(0, strlen(_request.getroot().c_str()));
                    buffer.append(parentDirLink);
                    buffer.append("\">      ");
                }
                if (buffer.find("<tbody id=") != std::string::npos) // start of the  table body
                {
                    // *  <tbody id="tbody">
                    line.append(buffer);
                    line.append("\n\r");
                    break;
                }
                line.append(buffer);
                line.append("\n\r");
            }

            inFile.close();

            // std::cout << "not foud error" << std::endl;
            // inFile.open("/Users/mmasstou/Desktop/webserV/var/srcs/autoIndex.html", std::ifstream::in)
            // {

            // }
            // std::cout << GREEN << " AutoIndex On" << END_CLR << std::endl;
            dirp = opendir(pathdir.c_str());
            if (dirp == NULL){throw _Exception(INTERNAL_SERVER_ERROR);}
            // std::cout << "Index of : " << pathdir << std::endl;
            // std::cout << "parent directory : " << pathdir << std::endl;
            // adding table fields :
            std::string request_URITmp;

            request_URITmp.append(_request.getrequest_URI());
            (request_URITmp.back() == '/')
                ? request_URITmp.erase(request_URITmp.length() - 1, request_URITmp.length())
                : request_URITmp;
            while ((dp = readdir(dirp)) != NULL)
            {
                // ! item_name
                // ! item_absPATH
                // ! item_URL
                // ! item_size
                // ! item_createDATE
                /*
                    <tr>
                        <td data-value="${item_name}"><a class="icon file" draggable="true" href="${item_path}">${item_name}</a></td>
                        <td class="detailsColumn" data-value="${item_size}">${item_size} B</td>
                        <td class="detailsColumn" data-value="1672423565">${item_createDate}</td>
                    </tr>
                **/

                std::cout << MAUVE  << "***** AOUTOINDEX *****" << std::endl;
                item_name.clear();
                item_name.append(dp->d_name, strlen(dp->d_name));
                std::cout << "item_name :" << item_name << std::endl;
                // ? ./var/




                item_absPATH.clear();
                // item_absPATH.append(_request.getroot());
                item_absPATH.append(_request.getrequest_URI());
                item_absPATH.append("/");
                item_absPATH.append(item_name);
                std::cout << "item_absPATH :" << item_absPATH << std::endl;

                // item_URL.append(item_absPATH, item_absPATH.length());
                // item_URL.erase(0, _request.getroot().length());




                item_URL.clear();

                if (_request.getcompare_URI().compare("/") != 0){
                    item_URL.append(_request.getcompare_URI());
                }
                item_URL.append(item_absPATH.substr( _request.getroot().length(), item_absPATH.length()));
                std::cout << "item_URL :" << item_URL << std::endl;
        

                struct stat STATFile;
                std::string filePATH;
                if (stat(item_absPATH.c_str(), &STATFile) != 0)
                {
                    std::cout << " |" << filePATH << "| file Not found \n";
                    // continue;
                }




                item_size.clear();
                item_size.append(std::to_string(STATFile.st_size));
                std::cout << "item_size :" << item_size << std::endl;




                item_createDATE.clear();
                item_createDATE.append(ctime(&STATFile.st_mtime));
                std::cout << "item_createDATE :" << item_createDATE << END_CLR <<std::endl;


                std::string d_nameTmp(dp->d_name);
                // std::cout << " Name :" << d_nameTmp << std::endl;
                if (d_nameTmp.compare(".") != 0 || d_nameTmp.compare("..") != 0)
                {
                    filePATH.clear();
                    filePATH.append(this->getRootPath());
                    filePATH.append(request_URITmp);
                    filePATH.append("/");
                    filePATH.append(dp->d_name);
                    std::cout << "filePATH :" << filePATH << std::endl;
                    // 
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
                    std::cout << "**filePATH :" << filePATH << std::endl;
                    // line.append("/");
                    // line.append(dp->d_name);
                    line.append("\">");
                    line.append(dp->d_name);
                    line.append("</a></td>");
                    line.append("<td class=\"detailsColumn\" >");
                    line.append(std::to_string(STATFile.st_size));
                    line.append("B</td>");
                    line.append("<td class=\"detailsColumn\" data-value=\"1672423565\"> ");
                    line.append(ctime(&STATFile.st_mtime));
                    line.append("</td></tr>\n\r");
                }
            }

            if (closedir(dirp) == -1)
            {
                perror("closedir");
                throw _Exception(INTERNAL_SERVER_ERROR);
                return 1;
            }
            dp = nullptr;
            line.append("</tbody></table></body></html>");
            // std::cout << "</Line >" << std::endl;
            this->setStatuscode(200);
            this->setreason_phrase("Ok");
           
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
    // std::cout << "safi salina ************* * * * * * * * * * \n";
    // std::cout << "Request Path :" << filename << std::endl;
    // read from server :
    // inFile.open(filename, std::ifstream::in);
    // if (!inFile.is_open())
    // {
    //     // std::cout << "Ana Hna\n";
    //     this->error(404, "Not Found");
    //     inFile.open("/Users/mmasstou/projects/WebServ/var/errors/40x.html", std::ifstream::in);
    //     // this->setStatuscode(404);
    //     // this->setreason_phrase("Not Found");
    // }
    // std::cout << "<Line URi='"<< this->getRequest_URI() <<"' root='"<< this->getRootPath()<<"' filename='"<<filename <<"'>" << std::endl;
    // while (std::getline(inFile, buffer))
    // {
    //     // std::cout << buffer << std::endl;
    //     line.append(buffer);
    // }
    // // std::cout << "</Line >" << std::endl;

    // this->setResponseBody(line);

    // std::cout << "\nBody : \n" << this->getResponseBody();
// 
    // std::cout <<"\n\n\n\nresponseBody :\n"<< this->getResponseBody() << std::endl;
    this->setResponseBody(line);

    // std::cout << "response : \n" << this->getResponseBody() << std::endl;
    // std::string tt( _request.getrequest_URI());
    std::string extension;
    size_t pos = _request.getrequest_URI().rfind(".");
    if (pos == 0)
        extension = "html";
    else 
        extension = _request.getrequest_URI().substr(pos + 1, _request.getrequest_URI().length());
    this->addHeader("Cache-Control", "no-cache");
    std::cout << "Assets::__getType(extension) :" << Assets::__getType(extension)  << " :>" << extension << std::endl;
    this->addHeader("Content-Type", Assets::__getType(extension));
    this->addHeader("Content-Length", std::to_string(this->getResponseBody().length()));
    return 1;
}