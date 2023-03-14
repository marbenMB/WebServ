#include "../../include/request.hpp"

void    request::Retrieving_requested_resource(ServerConf *server)
{
    this->message.push_back(std::to_string(this->socketID));
    /* -------------------------------------------------------------------------- */
    /*                             // !read config file                           */
    /*                      // * Create instaence for server                      */
    /* -------------------------------------------------------------------------- */
    // ! SERVER DATA
    // std::vector<ServerConf> serv = server->server_list;
    // *> create iterator for etch srever in config file :
    // std::vector<ServerConf>::iterator server_iterator = server->server_list.begin();
    std::map<std::string, std::vector<std::string> > it = server->server_data;
    // ? root :
    std::vector<std::string>::iterator root_vect = it["root"].begin();
    if (!root_vect[0].empty() && it["root"].size() == 1) this->root = *root_vect;
    
    //  ? set a default index page 
    this->checkForIndex(it["index"]);

    // ? set client max body size :
    std::vector<std::string>::iterator client_max_body_size_vect = it["client_max_body_size"].begin();
    if (!client_max_body_size_vect[0].empty() && it["client_max_body_size"].size() == 1){
       this->client_max_body_size = atoll(it["client_max_body_size"][0].c_str());
        // std::cout << "client_max_body_size :" << this->client_max_body_size << std::endl;
    }

     /* -------------------------------------------------------------------------- */
     /*                              // ? error pages                              */
     /* -------------------------------------------------------------------------- */
    this->checkForErrorPage(it["error_page"]);


    // ! LOCATION

    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location = server->locations;
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
    // if the config file has no root location 
    if (locationIndex == -1)
    {
        this->setAutoIndex(AUTOINDEX_OFF);
        if (this->req_method.compare("GET") == 0) {this->__get = ALLOWED;}
        return;
    }

    // get the location
    locations_iterator += locationIndex;
    j++;
    // ***> Create iterator for location Data
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_iterator = locations_iterator->begin();
    std::string str = location_iterator->first;

    // get location Data from ConfigFile
    // create map instance 'location_vars'
    std::map<std::string, std::vector<std::string> > location_vars = location_iterator->second;
    std::vector<std::string>::iterator iitt = location_vars["root"].begin();
    // root
    if (location_vars["root"].size()) this->root = *iitt;
    
    // if is not CGI and requestURI is not in root dir 
    if (!this->is_cgi && !is__subDir(this->root, this->getrequest_URI())){
        throw BadRequest();
    }
    // check for index in Location
    checkForIndex(location_vars["index"]);
    // upload_store
    if (location_vars["upload_store"].size()){
        this->upload_store.clear();
        this->upload_store.append(this->root);
        this->upload_store.append(location_vars["upload_store"][0]);
        // std::cout << "upload_store :" << this->upload_store << std::endl;
    }
    else{
        this->upload_store.clear();
        this->upload_store.append(UPLOAD_STORE);
        }
    // fastcgi_pass
    if (location_vars["fastcgi_pass"].size()){
        this->fastcgi_pass = location_vars["fastcgi_pass"][0];
    }

    // fastcgi_index
    if (location_vars["fastcgi_index"].size()){
        this->fastcgi_index = location_vars["fastcgi_index"][0];
    }
    // autoindex
    iitt = location_vars["autoindex"].begin();
    if (location_vars["autoindex"].size() > 0 &&  location_vars["autoindex"][0].compare("on") == 0) {
        // std::cout << "autoindex :"  << location_vars["autoindex"][0] <<"|"<< std::endl;
        this->autoindex = AUTOINDEX_ON;
        }
    // std::cout << RED << "   +>redirection" << END_CLR << std::endl;

    // redirection :
    iitt = location_vars["return"].begin();
    int vect_size = location_vars["return"].size();
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
    // std::cout << RED << "   +>allow_methods" << END_CLR << std::endl;

    // Allow_methods
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
}