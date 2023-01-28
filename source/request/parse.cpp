#include "../../include/request.hpp"

std::vector<std::string> &request::execute(std::string body, Data *_confdata)
{

    method *reqmethod = nullptr;
    std::vector<std::string> rBody = split(body, "\r\n\r\n");
    this->message.push_back(std::to_string(this->socketID));\

    // std::cout << "\nmethod : " << this->getmethod() << std::endl;

    std::vector<ServerConf> serv = _confdata->server_list;
    std::vector<ServerConf>::iterator serv__it = _confdata->server_list.begin();
    // std::vector<ServerConf>::iterator serv__it = _confdata->server_list.begin();
    std::map<std::string, std::vector<std::string> > it = _confdata->server_list.begin()->server_data;


    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location = serv__it->locations;
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator location_it = location.begin();


    while (location_it != location.end())
    {
        std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator locat_map_it = location_it->begin();
        std::cout << "  +>" << locat_map_it->first << std::endl;
        std::string str = locat_map_it->first;
        if (str.compare(this->request_URI) == 0)
        {
            std::map<std::string, std::vector<std::string> > loc_var = locat_map_it->second;
            std::vector<std::string>::iterator iitt = loc_var["allow"].begin();
            while(iitt != loc_var["allow"].end()){
            std::cout << "      +>" << *iitt << std::endl;
            ++iitt;
            }
            
            // while (loc_var != locat_map_it->second.end()) {
            //     std::cout << "      +>" << *loc_var << std::endl;
            //     loc_var++;
            // }
            // break;
        }
       ++location_it;
    }
    
    std::vector<std::string> root_vect = it["root"];
    std::vector<std::string>::iterator listen_vect = it["listen"].begin();
    std::vector<std::string> error_page_vect = it["error_page"];
    // std::cout << "root_vect : " << root_vect[0] << std::endl;

    while (listen_vect != it["listen"].end()){
        /* code */
        std::cout << "listen_vect : " << listen_vect[0] << std::endl;
        ++listen_vect;
    }
    std::vector<std::string>::iterator error_page_it = error_page_vect.begin();



    // ! check request Server :

    std::cout << "ROOT :" << this->root << std::endl;
    // std::cout << "HOST :" << this->host << std::endl;
    std::vector<std::string> splitHost = split(this->host, ":");
    std::cout << "/* -------------------------------------------------------------------------- */"<<std::endl;
    std::cout << "/*"<<BLUE<<"                                REQUEST INFO                                "<< END_CLR<<"*/"<<std::endl;
    std::cout << "/*                            HOST :"<<this->host<<"                            */"<<std::endl;
    std::cout << "/*                                 LISTEN:"<<splitHost[1]<<"                                */"<<std::endl;
    std::cout << "/*                              SERVER NAME:"<<splitHost[0]<<"                         */"<<std::endl;
    std::cout << "/* -------------------------------------------------------------------------- */"<<std::endl;
    // //  * listen :
    // std::cout << "LISTEN:" << splitHost[1] << std::endl;;
    // //  * servre_name:
    // std::cout << "SERVER NAME:" << splitHost[1] << std::endl;;
    // //  * host:
    // std::cout << "host:" << splitHost[0] << std::endl;;


    // while (error_page_it != error_page_vect.end())
    // {
    //     std::cout << "  +>"<< *error_page_it << std::endl;
    //     ++error_page_it;
    // }
    
    if (!root_vect[0].empty())
        this->root = root_vect[0];
    // std::cout << "ROOT :" << this->root << std::endl;
    
    // while (it != serv__it->server_data.end())
    // {
    //         ++it;
    // }

    if (this->req_method.compare("GET") == 0)
        reqmethod = new get(*this);
    else if (this->req_method.compare("DELETE") == 0)
        reqmethod = new deleteMethod(*this);
    else if (this->req_method.compare("POST") == 0)
    {
        this->setRequestBody(rBody);
        reqmethod = new Post(*this);
    }
    else
    {
    }
    if (reqmethod->getStatuscode() != 200)
    {
        // std::cout << "\e[1;31m+>Bad Request\n\e[0m";
        reqmethod->setRequest_URI("40x.html");
        std::ifstream inFile;
        std::string line;
        // std::string responseBody;
        std::string filename = "/Users/mmasstou/Desktop/webserv/data/errors/40x.html";
        // check config file if the method is allowed:
        // std::cout << "Request Path :" << filename << std::endl;
        // read from server :
        inFile.open(filename, std::ifstream::in);
        body = "";
        while (std::getline(inFile, line))
        {
            body.append(line);
        }
        this->requirements = false;
        reqmethod->setResponseBody(body);
    }
    reqmethod->createresponse();
    // std::cout << "\n    Body :\n"
    //   << reqmethod->getResponseBody() << std::endl;
    this->message.push_back(reqmethod->getResponseBody());

    // std::cout << "\n    ++> SocketId :" << this->message[0] << "\n    ++>Message :" << this->message[1] << std::endl;
    // std::cout <<"\n" << reqmethod->getHost()<< " " <<  reqmethod->getStatuscode() << " " ;
    // * print status : HTTP/1.1 200 OK
    std::cout << GREEN << "[" << this->gethost() << "] " << this->gethttp_version() << " " << reqmethod->getStatuscode() << " " << reqmethod->getreason_phrase() << " " << this->request_URI << END_CLR <<std::endl;
    delete reqmethod;
    return (this->message);
}
