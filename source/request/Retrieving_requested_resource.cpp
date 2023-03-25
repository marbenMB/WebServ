#include "../../include/request.hpp"

void    request::Retrieving_requested_resource(ServerConf *server)
{
  
    // ! SERVER DATA
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
    }
    this->checkForErrorPage(it["error_page"]);
    // ! LOCATION
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location = server->locations;
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator locations_iterator = location.begin();

    int locationIndex;
    //  *|> /srcs/dir001/dir0011/test.txt
    locationIndex = this->findLocation(location);
    // if the config file has no root location 
    if (locationIndex == -1)
    {
        this->setAutoIndex(AUTOINDEX_OFF);
        if (_findHeader(REQUEST_METHOD).compare("GET") == 0) {this->__get = ALLOWED;}
        return;
    }

    // get the location
    locations_iterator += locationIndex;
    // ***> Create iterator for location Data
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_iterator = locations_iterator->begin();
    std::string str = location_iterator->first;

    // get location Data from ConfigFile
    // create map instance 'location_vars'
    std::map<std::string, std::vector<std::string> > location_vars = location_iterator->second;
    std::vector<std::string>::iterator __iterator = location_vars["root"].begin();
    // root
    if (location_vars["root"].size()) this->root = *__iterator;
    
    // if is not CGI and requestURI is not in root dir 
    if (!this->is_cgi && !is__subDir(this->root, this->_findHeader(REQUEST_URI))){
        throw _Exception(BAD_REQUEST);
    }
    // check for index in Location
    checkForIndex(location_vars["index"]);
    // upload_store
    if (location_vars["upload_store"].size()){
        this->upload_store.clear();
        this->upload_store.append(this->root);
        this->upload_store.append(location_vars["upload_store"][0]);
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
    if (location_vars["autoindex"].size() > 0 &&  location_vars["autoindex"][0].compare("on") == 0) {
        this->autoindex = AUTOINDEX_ON;
    }
    // redirection :
    int vect_size = location_vars["return"].size();
    if (vect_size == 2){
        this->setRedirect_status(atoi(location_vars["return"][0].c_str()));
        this->setredirect_URL(location_vars["return"][1]);
    }
    // Allow_methods
    __iterator = location_vars["allow_methods"].begin();
    if (location_vars["allow_methods"].size()){
        while (__iterator != location_vars["allow_methods"].end()){
            if ((*__iterator).compare(_findHeader(REQUEST_METHOD)) == 0)
            {
                if (_findHeader(REQUEST_METHOD).compare("POST") == 0) {this->__post = ALLOWED;}
                else if (_findHeader(REQUEST_METHOD).compare("GET") == 0) {this->__get = ALLOWED;}
                else if (_findHeader(REQUEST_METHOD).compare("DELETE") == 0) {this->__delete = ALLOWED;}
                // allow_methods = true;
            }
            ++__iterator;
        }
    }
    if ((_findHeader(REQUEST_METHOD).compare("POST") != 0 ) &&
        (_findHeader(REQUEST_METHOD).compare("GET") != 0) &&
        (_findHeader(REQUEST_METHOD).compare("DELETE")))
            this->__noImplimented = ALLOWED;
}