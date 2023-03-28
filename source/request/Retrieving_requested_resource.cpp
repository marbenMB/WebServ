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
    this->upload_store.clear();
    if (location_vars["upload_store"].size()){
        this->upload_store.append(this->root);
        this->upload_store.append(location_vars["upload_store"][0]);
    }
    else {this->upload_store.append(UPLOAD_STORE);}
    // fastcgi_pass
    if (location_vars["fastcgi_pass"].size()){
        this->fastcgi_pass = location_vars["fastcgi_pass"][0];
    }
    else
        this->fastcgi_pass = FAST_CGI_PASS;
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
        (_findHeader(REQUEST_METHOD).compare("DELETE") != 0))
            this->__method_status = NOT_IMPLIMENTED;
    else if (this->__post == NOT_ALLOWED)
        this->__method_status = NOT_ALLOWED;
}

void request::checkForIndex(std::vector<std::string> vect){
    if (vect.size()){
        std::vector<std::string>::iterator it = vect.begin();
        this->default_index.clear() ;
        if (!it[0].empty()){
            this->default_index.append("/");
            this->default_index.append(*it);
        }
        else this->default_index.append("/index.html");
        }
    // std::cout << "in Default Index : " << it[0] << std::endl;
    // std::cout << "out Default Index : " << this->default_index << std::endl;
}
void request::checkForErrorPage(std::vector<std::string> vect){
    struct stat STATFile;
    std::vector<std::string>::iterator errorspage_iterator = vect.begin();
    int NbeOfPages = 0;
    bool one = false, two = false, three = false, fore = false, five = false;
    while (errorspage_iterator != vect.end())
    {
        if (((*errorspage_iterator).find(".html") == std::string::npos)){
            if ((*errorspage_iterator)[0] == '1') one = true;
            if ((*errorspage_iterator)[0] == '2') two = true;
            if ((*errorspage_iterator)[0] == '3') three = true;
            if ((*errorspage_iterator)[0] == '4') fore = true;
            if ((*errorspage_iterator)[0] == '5') five = true;
            // std::cout << "type :"<< (*errorspage_iterator)[0]<< std::endl;
        }
        if ((*errorspage_iterator).find(".html") != std::string::npos){
            if (one) {

                this->default_10x = this->root + "/" + *errorspage_iterator;
                if (stat(this->default_10x.c_str(), &STATFile) != 0){
                   this->default_10x = ERROR_PATH;
                }
            }
            if (two){
                 this->default_20x = this->root+ "/" + *errorspage_iterator;
                if (stat(this->default_20x.c_str(), &STATFile) != 0){
                   this->default_20x = ERROR_PATH;
                }
            }
            if (three){
                 this->default_30x = this->root + "/" + *errorspage_iterator;
                if (stat(this->default_30x.c_str(), &STATFile) != 0){
                   this->default_30x = ERROR_PATH;
                }
            }
            if (fore){
                this->default_40x = this->root + "/" + *errorspage_iterator;
                if (stat(this->default_40x.c_str(), &STATFile) != 0){
                   this->default_40x = ERROR_PATH;
                }
            }
            if (five){
                this->default_50x = this->root + "/" + *errorspage_iterator;
                if (stat(this->default_50x.c_str(), &STATFile) != 0){
                   this->default_50x = ERROR_PATH;
                }
            }
            one = two = three = fore = five = false;
            NbeOfPages++;
        }
        ++errorspage_iterator;
    }  
}

int request::findLocation(std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location){
    char realPATH_dir[PATH_MAX];
    char realPATH_subdir[PATH_MAX];
    // char* _subdir;
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator locations_iterator = location.begin();
    int locationId = -1;
    int index = 0;
    std::string router;
    size_t pos = 0;
    
    if ((pos =  _findHeader(REQUEST_URI).rfind(".py")) != std::string::npos && (pos + 3) ==  _findHeader(REQUEST_URI).length())
    {
       this->is_cgi = true; 
        this->compare_URI.append("\%.py$");
    }
    else if ((pos =  _findHeader(REQUEST_URI).rfind(".go")) != std::string::npos && (pos + 3) ==  _findHeader(REQUEST_URI).length())
    {
        this->is_cgi = true;
        this->compare_URI.append("\%.go$");
    }
    else this->compare_URI.append(this->_findHeader(REQUEST_URI));
     //  * init string |> /srcs/dir001/dir0011/test.txt

    while (locations_iterator != location.end())
    {
        // ***> Create iterator for location Data
        std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_iterator = locations_iterator->begin();
        router.clear();
        router.append(location_iterator->first);
        if (router.compare(this->compare_URI) == 0){locationId = index;break;}
        ++locations_iterator;
        index++;
        if (locations_iterator == location.end() && locationId == -1){  // if '/srcs/dir001/dir0011/test.txt' not found ;
            size_t start = this->compare_URI.find_last_of("/"); // find the last '/'
            if (start != std::string::npos && this->compare_URI.compare("/") != 0){ // if the's  '/'
                locations_iterator = location.begin(); // init iterator to the begin
                index = 0; // init index count to the 0 for anther tour
                if (!start){start += 1;}// for the last one means request URI is  '/' ;becuse we should remove the last '/'
                this->compare_URI.erase(start, this->compare_URI.length() - 1); // now remove after ''/
            }
        }
    }
    if (!this->is_cgi) // remove similar string 
    {
        if (locationId != -1)
        {
            std::string __erraseTmp( _findHeader(REQUEST_URI)) ;
            std::string __URI(this->getroot());
            if (__erraseTmp.compare("/") != 0)
            {
                this->setcompare_URI(router);
                __erraseTmp.erase(0,router.length());
                __URI.append("/");
            }
            __URI.append(__erraseTmp);
            realpath("./", realPATH_subdir);
            realpath(__URI.c_str(), realPATH_dir);
            __erraseTmp.clear();
            __erraseTmp.append(realPATH_dir);
            __erraseTmp.erase(0, strlen(realPATH_subdir));
            __URI.clear();
            __URI.append(".");
            __URI.append(__erraseTmp);
            this->_requestHeaders[REQUEST_URI] = __URI;
        }
    }
    if (this->is_cgi && locationId == -1)
        throw  _Exception(NOT_IMPLEMENTED);
    return locationId;
}
