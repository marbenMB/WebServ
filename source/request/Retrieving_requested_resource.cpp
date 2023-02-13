#include "../../include/request.hpp"

void    request::Retrieving_requested_resource(Data *server)
{
    this->message.push_back(std::to_string(this->socketID));
    /* -------------------------------------------------------------------------- */
    /*                             // !read config file                           */
    /*                      // * Create instaence for server                      */
    /* -------------------------------------------------------------------------- */
    std::vector<ServerConf> serv = server->server_list;
    // *> create iterator for etch srever in config file :
    std::vector<ServerConf>::iterator server_iterator = server->server_list.begin();

    /* -------------------------------------------------------------------------- */
    /*      // ? Create instance for Serevr Data means header of config file      */
    /* -------------------------------------------------------------------------- */
    std::map<std::string, std::vector<std::string> > it = server_iterator->server_data;
    // ? root :
    std::vector<std::string>::iterator root_vect = it["root"].begin();
    if (!root_vect[0].empty() && it["root"].size() == 1) this->root = *root_vect;
    //  ? set a default index page 
    this->checkForIndex(it["index"]);

    // ? set client max body size :
    std::vector<std::string>::iterator client_max_body_size_vect = it["client_max_body_size"].begin();
    if (!client_max_body_size_vect[0].empty() && it["client_max_body_size"].size() == 1){
        if ((*client_max_body_size_vect).find("G") != std::string::npos)
        {
            // storege the value in the obj
            std::cout << "NOt in string\n";
        }
    }

     /* -------------------------------------------------------------------------- */
     /*                              // ? error pages                              */
     /* -------------------------------------------------------------------------- */
    this->checkForErrorPage(it["error_page"]);
    /* -------------------------------------------------------------------------- */
    /*           // ? Create instance for server Locations all location           */
    /* -------------------------------------------------------------------------- */
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location = server_iterator->locations;
    // **> Create iterator for etch loation :
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator locations_iterator = location.begin();
   
    //*                                    vars                                    
    /* -------------------------------------------------------------------------- */
    // bool allow = false;
    // bool deny = false;
    int j = 0;
    // int i = 0;
    // bool allow_methods = false;
    // create compare URI
    int locationIndex;
    //  *|> /srcs/dir001/dir0011/test.txt
    locationIndex = this->findLocation(location);
    if (locationIndex == -1)
    {
        this->setAutoIndex(AUTOINDEX_OFF);
        if (this->req_method.compare("GET") == 0) {this->__get = ALLOWED;}
        return;
    }
    std::cout << "locationIndex :" << locationIndex << std::endl;
    // int location_size = location.size();
    // while (locations_iterator != location.end())
    // {
    locations_iterator += locationIndex;
    j++;
    // ***> Create iterator for location Data
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_iterator = locations_iterator->begin();
    std::cout << "  +>" << location_iterator->first << std::endl;
    std::string str = location_iterator->first;
    // if (str.compare(this->request_URI) == 0)
    // {

    std::map<std::string, std::vector<std::string> > location_vars = location_iterator->second;
    std::vector<std::string>::iterator iitt = location_vars["allow"].begin();
    // std::cout << RED << "ALLOW :\n" << END_CLR ;
    std::cout << RED << "   +>Allow" << END_CLR << std::endl;
    if (location_vars["allow"].size()){
        while (iitt != location_vars["allow"].end())
        {
            // std::cout << "      +[allow]>" << *iitt << std::endl;
            // if ((*iitt).compare(this->req_method) == 0) allow = true;
            ++iitt;
        }
    } 
    // std::cout << RED << "DENY :\n" << END_CLR ;
    std::cout << RED << "   +>deny" << END_CLR << std::endl;

    iitt = location_vars["deny"].begin();
    if (location_vars["deny"].size()){
            while (iitt != location_vars["deny"].end())
        {
            // std::cout << "      +[deny]>" << *iitt << std::endl;
            // if ((*iitt).compare(this->req_method) == 0) deny = true;
            ++iitt;
        }
    } 
    //  std::cout << RED << "ROOT :\n" << END_CLR ;
    std::cout << RED << "   +>root" << END_CLR << std::endl;

    iitt = location_vars["root"].begin();
    if (location_vars["root"].size()) this->root = *iitt;
    std::cout << RED << "   +>index" << END_CLR << std::endl;

    checkForIndex(location_vars["index"]);
    if (this->getrequest_URI().compare("/") == 0){
        // std::cout << RED << this->default_index << END_CLR << std::endl;
        this->request_URI = "/" + this->default_index;
    }

    // autoindex
    iitt = location_vars["autoindex"].begin();
    if (location_vars["autoindex"].size() > 0 &&  location_vars["autoindex"][0].compare("on") == 0) {
        std::cout << "autoindex :"  << location_vars["autoindex"][0] <<"|"<< std::endl;
        this->autoindex = AUTOINDEX_ON;
        }
    std::cout << RED << "   +>redirection" << END_CLR << std::endl;

    // redirection :
    iitt = location_vars["return"].begin();
    // int kk = 0;
    int vect_size = location_vars["return"].size();
    // std::cout << "redirect :" << vect_size << std::endl;
    if (vect_size == 2){
        // std::cout << "  +>redirect :" << location_vars["return"][0] << std::endl;
        // std::cout << "  +>redirect :" << location_vars["return"][1] << std::endl;
        this->setRedirect_status(atoi(location_vars["return"][0].c_str()));
        this->setredirect_URL(location_vars["return"][1]);
    }
    else
    {
        this->setRedirect_status(-1);
        this->setredirect_URL("");
    }
    std::cout << RED << "   +>allow_methods" << END_CLR << std::endl;

    //    allow_methods
    iitt = location_vars["allow_methods"].begin();
    if (location_vars["allow_methods"].size()){
        while (iitt != location_vars["allow_methods"].end()){
            std::vector<std::string> vect_allow_methods = split(*iitt, ",");
            std::vector<std::string>::iterator iter_allow_methods = vect_allow_methods.begin();
            while(iter_allow_methods != vect_allow_methods.end()){
                if ((*iter_allow_methods).compare(this->req_method) == 0)
                {
                    if (this->req_method.compare("POST") == 0) {this->__post = ALLOWED;}
                    if (this->req_method.compare("GET") == 0) {this->__get = ALLOWED;}
                    if (this->req_method.compare("DELETE") == 0) {this->__delete = ALLOWED;}
                    // allow_methods = true;
                }
                // std::cout << "      +[allow_methods]["<< j <<"]>" << *iter_allow_methods << std::endl;
                ++iter_allow_methods;
            }
            ++iitt;
        }
    }
            // else {allow_methods = true;}
        // }
    //     i++;
    //     location_size--;
    //     ++locations_iterator;
       
    // }
}