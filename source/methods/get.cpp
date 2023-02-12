#include "../../include/method.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

get::get(request rhs)
{

    this->setHost(rhs.gethost());
    this->setRequest_URI(rhs.getrequest_URI());
    this->setHttp_version(rhs.gethttp_version());
    this->setStatuscode(200);
    this->setreason_phrase("OK");
    this->setRootPath(rhs.getroot());
     this->setResponseBody("");
    this->setContent_Type(rhs.getContent_Type());
    this->execute_method(rhs);    
    this->setAutoIndex(rhs.getAutoIndex());
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
    // std::string responseBody;
    // check config file if the method is allowed:
    // if (this->getRequest_URI().compare("/") == 0){
    //     this->setRequest_URI("/index.html"); 
    // }
    std::string filename ;

    filename.append(this->getRootPath());
    filename.append(this->getRequest_URI());
    std::cout << "PATH : " << filename << std::endl;
    if (stat(filename.c_str(), &STATInfo) != 0){
        std::cout << "not foud error" << std::endl;
    }
    if ((STATInfo.st_mode & S_IFMT) == S_IFREG) // is file
    {

    }
    if (stat(filename.c_str(), &STATInfo) != 0 && (STATInfo.st_mode & S_IFMT) == S_IFDIR) // is dir
    {
        std::string pathdir;

        pathdir = filename;
        filename.append(_request.getdefaultIndex());
        inFile.open( filename, std::ifstream::in);
        if (!inFile.is_open() && this->getAutoIndex() == AUTOINDEX_ON){
            // std::cout << "not foud error" << std::endl;
            // inFile.open("/Users/mmasstou/Desktop/webserV/var/srcs/autoIndex.html", std::ifstream::in)
            // {

            // }
            std::cout << GREEN <<" AutoIndex On" << END_CLR << std::endl;
            dirp = opendir(pathdir.c_str());
            if (dirp == NULL) {
                perror("opendir");
                return 1;
            }

            while ((dp = readdir(dirp)) != NULL) {
                printf("%s\n", dp->d_name);
            }

            if (closedir(dirp) == -1) {
                perror("closedir");
                return 1;
            }

        }
    }
    // std::cout << "Request Path :" << filename << std::endl;
    // read from server :
    inFile.open( filename, std::ifstream::in);
    if (!inFile.is_open())
    {
        // std::cout << "Ana Hna\n";
        this->error(404, "Not Found");
         inFile.open( "/Users/mmasstou/projects/WebServ/var/errors/40x.html", std::ifstream::in);
        // this->setStatuscode(404);
        // this->setreason_phrase("Not Found");
    }
    // std::cout << "<Line URi='"<< this->getRequest_URI() <<"' root='"<< this->getRootPath()<<"' filename='"<<filename <<"'>" << std::endl;
    while (std::getline(inFile, buffer))
    {
        // std::cout << buffer << std::endl;
        line.append(buffer);
    }
    // std::cout << "</Line >" << std::endl;

    this->setResponseBody(line);

    // std::cout << "\nBody : \n" << this->getResponseBody();

    // std::cout <<"\n\n\n\nresponseBody :\n"<< this->getResponseBody() << std::endl;
    return 1;
    
}