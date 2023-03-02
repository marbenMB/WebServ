#include "../../include/method.hpp"
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>

/*

 TODO
  * 1 - AutoIndex √
  * 2 - allowed method
  ! 3 - deny
  ! 4 - allow
  * 5 - redirect

**/

get::get(request rhs)
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

get::~get()
{
}
/* -------------------------------------------------------------------------- */
/*                            execute_method                            */
/* -------------------------------------------------------------------------- */

int get::execute_method(request _request)
{
    std::ifstream inFile;
    std::string line = "";
    std::string buffer;
    struct stat STATInfo;
    DIR *dirp;
    struct dirent *dp;
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
        this->setStatuscode(_request.getRedirect_status());
        this->setreason_phrase("Moved Permanently");
        if (Is_cgi(_request.getredirect_URL())){ throw request::CGI();}
        filename.clear();
        filename.append(_request.getroot());
        filename.append(_request.getredirect_URL());

        _request.setrequest_URI(filename);
        // set Header : 
        this->addHeader("Location", _request.getredirect_URL());
        inFile.open(filename, std::ifstream::in);
        while (std::getline(inFile, buffer))
        {
            // std::cout << buffer << std::endl;
            line.append(buffer);
        }
        // std::cout << "</Line >" << std::endl;
        inFile.close();
    }
    else if (stat(filename.c_str(), &STATInfo) != 0)
    { // not exist
        throw request::NotFound();
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFREG) { // is file   S_ISREG(fileStat.st_mode)
        this->setStatuscode(200);
        this->setreason_phrase("Ok");
        inFile.open(filename, std::ifstream::in);
        while (std::getline(inFile, buffer))
        {
            // std::cout << buffer << std::endl;
            line.append(buffer);
        }
        inFile.close();
    }
    else if ((STATInfo.st_mode & S_IFMT) == S_IFDIR) { // is dir
        // std::cout << "stat dir : " << filename << std::endl;
        filename.append("/");
        filename.append(_request.getdefaultIndex());
        // std::cout << "filename [" << _request.getAutoIndex() << "]: " << filename << std::endl;
        // if (_request.getAutoIndex() == AUTOINDEX_ON)
        //     std::cout << "AUTOINDEX_ON\n";
        // else
        //     std::cout << "AUTOINDEX_OFF\n";

        // if (stat(filename.c_str(), &STATInfo) != 0)
        // {
        //     std::cout << "not foud error" << std::endl;
        // }

        inFile.open(filename, std::ifstream::in);
        if (Is_cgi(_request.getdefaultIndex())){ throw request::CGI();}
        else if (!inFile.is_open() && _request.getAutoIndex() == AUTOINDEX_ON) // run AutoIndex 
        {
            std::string pathdir(_request.getrequest_URI());
            // if (pathdir.compare("/") != 0){
            //     pathdir.append("/..");
            // }
            std::cout << "AUTOINDEX :" << _request.getrequest_URI() << std::endl;
            line.clear();
            inFile.open("./var/srcs/autoIndex.html", std::ifstream::in);
            if (!inFile.is_open())
                throw request::InternalServerError();
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
                    std::cout << "_request.getroot() :" << _request.getroot() << std::endl;
                    std::cout << "pathdir :" << pathdir << std::endl;
                    size_t start = (parentDirLink.compare("/") == 0) ? std::string::npos :parentDirLink.rfind("/");
                    (start != std::string::npos) ? parentDirLink.erase(start + 1, parentDirLink.length()) : parentDirLink;
                    std::cout << "parentDirLink :" << parentDirLink << std::endl;
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
            if (dirp == NULL)
            {
                throw request::InternalServerError();
                return 1;
            }
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
                /*
                    <tr>
                        <td data-value="runtime.txt"><a class="icon file" draggable="true" href="/Users/mmasstou/Desktop/my-Store/runtime.txt">runtime.txt</a></td>
                        <td class="detailsColumn" data-value="14">14 B</td>
                        <td class="detailsColumn" data-value="1672423565">12/30/22, 7:06:05 PM</td>
                    </tr>
                    ctime(&file_stat.st_mtime)
                    .st_size
                **/
                std::string d_nameTmp(dp->d_name);
                // std::cout << " Name :" << d_nameTmp << std::endl;
                if (d_nameTmp.compare(".") != 0 || d_nameTmp.compare("..") != 0)
                {
                    struct stat STATFile;
                    std::string filePATH;
                    filePATH.clear();
                    filePATH.append(this->getRootPath());
                    filePATH.append(request_URITmp);
                    filePATH.append("/");
                    filePATH.append(dp->d_name);
                    // std::cout << "FILE PATH :" << filePATH << std::endl;
                    if (stat(filePATH.c_str(), &STATFile) != 0)
                    {
                        std::cout << " |" << filePATH << "| file Not found \n";
                        continue;
                    }
                    line.append("<tr> <td data-value=\"");
                    line.append(dp->d_name);
                    line.append("\">\n\r<a class=\"icon file\" draggable=\"true\" href=\"");
                    filePATH.erase(0, strlen(_request.getroot().c_str()));
                    line.append(filePATH);
                    // line.append("/");
                    // line.append(dp->d_name);
                    line.append("\">");
                    line.append(dp->d_name);
                    line.append("</a>\n\r</td>");
                    line.append("<td class=\"detailsColumn\" >");
                    line.append(std::to_string(STATFile.st_size));
                    line.append("B</td>\n\r");
                    line.append("<td class=\"detailsColumn\" data-value=\"1672423565\"> ");
                    line.append(ctime(&STATFile.st_mtime));
                    line.append("</td>\n\r</tr>\n\r");
                }
            }

            if (closedir(dirp) == -1)
            {
                perror("closedir");
                throw request::InternalServerError();
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
        else throw request::Forbiden();
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

    // std::cout <<"\n\n\n\nresponseBody :\n"<< this->getResponseBody() << std::endl;
    this->setResponseBody(line);
    this->addHeader("Content-Type", "text/html");
    this->addHeader("Content-Length", std::to_string(strlen(this->getResponseBody().c_str())));
    return 1;
}