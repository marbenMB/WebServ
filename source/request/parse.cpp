#include "../../include/request.hpp"

void request::print_vectINFO(std::vector<std::string> Data, std::string string)
{
    std::vector<std::string>::iterator it = Data.begin();
     while (it != Data.end())
    {
        /* code */
        std::cout << string <<" : |>" << it[0] << std::endl;
        ++it;
    }
}

void request::checkForIndex(std::vector<std::string> vect){

    std::vector<std::string>::iterator it = vect.begin();
    this->default_index = "" ;
    if (vect.size() && !it[0].empty()){
        if (it[0].find("/") != std::string::npos){
            this->default_index.append("/");
        }
        this->default_index.append(*it);
    }
    else{
        this->default_index.append("/index.html");
    }
}
void request::checkForErrorPage(std::vector<std::string> vect){
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
            if (one) this->default_10x = *errorspage_iterator;
            if (two) this->default_20x = *errorspage_iterator;
            if (three) this->default_30x = *errorspage_iterator;
            if (fore) this->default_40x = *errorspage_iterator;
            if (five) this->default_50x = *errorspage_iterator;
            one = two = three = fore = five = false;
            NbeOfPages++;
        }
        ++errorspage_iterator;
    }
}

std::vector<std::string> &request::execute(std::string body, Data *_confdata)
{

    method *reqmethod = nullptr;
    std::vector<std::string> rBody = split(body, "\r\n\r\n");
    this->message.push_back(std::to_string(this->socketID));
    /* -------------------------------------------------------------------------- */
    /*                             // !read config file                           */
    /*                      // * Create instaence for server                      */
    /* -------------------------------------------------------------------------- */
    std::vector<ServerConf> serv = _confdata->server_list;
    // *> create iterator for etch srever in config file :
    std::vector<ServerConf>::iterator server_iterator = _confdata->server_list.begin();

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
    // std::vector<std::string> error_page_vect = it["error_page"];
    this->checkForErrorPage(it["error_page"]);
    // std::cout << "error_page_vect : |>" << error_page_vect.size() << std::endl;
    // std::vector<std::string>::iterator errorspage_iterator = error_page_vect.begin();
    // int NbeOfPages = 0;
    // bool one = false, two = false, three = false, fore = false, five = false;
    // while (errorspage_iterator != error_page_vect.end())
    // {
    //     if (((*errorspage_iterator).find(".html") == std::string::npos)){
    //         if ((*errorspage_iterator)[0] == '1') one = true;
    //         if ((*errorspage_iterator)[0] == '2') two = true;
    //         if ((*errorspage_iterator)[0] == '3') three = true;
    //         if ((*errorspage_iterator)[0] == '4') fore = true;
    //         if ((*errorspage_iterator)[0] == '5') five = true;
    //         // std::cout << "type :"<< (*errorspage_iterator)[0]<< std::endl;
    //     }
    //     if ((*errorspage_iterator).find(".html") != std::string::npos){
    //         if (one) this->default_10x = *errorspage_iterator;
    //         if (two) this->default_20x = *errorspage_iterator;
    //         if (three) this->default_30x = *errorspage_iterator;
    //         if (fore) this->default_40x = *errorspage_iterator;
    //         if (five) this->default_50x = *errorspage_iterator;
    //         one = two = three = fore = five = false;
    //         NbeOfPages++;
    //     }
    //     ++errorspage_iterator;
    // }
    // std::cout << "Nbs of page :" << NbeOfPages << std::endl;
    // std::cout << "1ox of page :" << this->default_10x << std::endl;
    // std::cout << "20x of page :" << this->default_20x << std::endl;
    // std::cout << "30x of page :" << this->default_30x << std::endl;
    // std::cout << "40x of page :" << this->default_40x << std::endl;
    // std::cout << "50x of page :" << this->default_50x << std::endl;

    /* -------------------------------------------------------------------------- */
    /*           // ? Create instance for server Locations all location           */
    /* -------------------------------------------------------------------------- */
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > location = server_iterator->locations;
    // **> Create iterator for etch loation :
    std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator locations_iterator = location.begin();
   
    //*                                    vars                                    
    /* -------------------------------------------------------------------------- */
    bool allow_methods = false;
    bool allow = false;
    bool deny = false;


    while (locations_iterator != location.end())
    {
        // ***> Create iterator for location Data
        std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator location_iterator = locations_iterator->begin();
        // std::cout << "  +>" << location_iterator->first << std::endl;
        std::string str = location_iterator->first;
        if (str.compare("/") == 0 || str.compare(this->request_URI) == 0)
        {
            std::map<std::string, std::vector<std::string> > location_vars = location_iterator->second;
            std::vector<std::string>::iterator iitt = location_vars["allow"].begin();
            // std::cout << RED << "ALLOW :\n" << END_CLR ;
            if (location_vars["allow"].size()){
                while (iitt != location_vars["allow"].end())
                {
                    // std::cout << "      +[allow]>" << *iitt << std::endl;
                    if ((*iitt).compare(this->req_method) == 0) allow = true;
                    ++iitt;
                }
            } else {allow = true;}
            // std::cout << RED << "DENY :\n" << END_CLR ;
            iitt = location_vars["deny"].begin();
            if (location_vars["deny"].size()){
                 while (iitt != location_vars["deny"].end())
                {
                    // std::cout << "      +[deny]>" << *iitt << std::endl;
                    if ((*iitt).compare(this->req_method) == 0) deny = true;
                    ++iitt;
                }
            } else{ deny = false;}
            //  std::cout << RED << "ROOT :\n" << END_CLR ;
            iitt = location_vars["root"].begin();
            if (location_vars["root"].size()) this->root = *iitt;
            checkForIndex(location_vars["index"]);
            if (this->getrequest_URI().compare("/") == 0){
                // std::cout << RED << this->default_index << END_CLR << std::endl;
                this->request_URI = "/" + this->default_index;
            }
            iitt = location_vars["allow_methods"].begin();
            if (location_vars["allow_methods"].size()){
                while (iitt != location_vars["allow_methods"].end())
                {
                    std::vector<std::string> vect_allow_methods = split(*iitt, ",");
                    std::vector<std::string>::iterator iter_allow_methods = vect_allow_methods.begin();
                    while(iter_allow_methods != vect_allow_methods.end()){
                        if ((*iter_allow_methods).compare(this->req_method) == 0)
                            allow_methods = true;

                        // std::cout << "      +[allow_methods]>" << *iter_allow_methods << std::endl;
                        ++iter_allow_methods;
                    }
                    ++iitt;
                }
            }
            else {allow_methods = true;}

        }
        ++locations_iterator;
    }
    /* -------------------------------------------------------------------------- */
    /*                              // !execute method                            */
    /* -------------------------------------------------------------------------- */
    /* -------------------------------------------------------------------------- */
    /*                                // !init vars                               */
    /* -------------------------------------------------------------------------- */

    // std::cout << "  ROOT +>"<<this->root << std::endl;
    (void)allow;
    (void)deny;
    if (allow_methods == true){
        if (this->req_method.compare("GET") == 0)
            reqmethod = new get(*this);
        else if (this->req_method.compare("DELETE") == 0)
            reqmethod = new deleteMethod(*this);
        else if (this->req_method.compare("POST") == 0)
        {
            this->setRequestBody(rBody);
            reqmethod = new Post(*this);
        }
    } 
    else {
        std::cout << "the method not allowed" << std::endl;
        // exit(1);
    } // the request not allowed 


    /* -------------------------------------------------------------------------- */
    /*              // !retriving the request recource from the server            */
    /* -------------------------------------------------------------------------- */
    /* -------------------------------------------------------------------------- */
    /*                           // !create responce msg                          */
    /* -------------------------------------------------------------------------- */
    reqmethod->createresponse();
    this->message.push_back(reqmethod->getResponseBody());
    // * print status : HTTP/1.1 200 OK
    std::string color_status;
    if (reqmethod->getStatuscode() == 200 )
        color_status = GREEN;
    else
        color_status = RED;
    std::cout << color_status << "" << this->gethost() << " " << this->gethttp_version() << " " << reqmethod->getStatuscode() << " " << reqmethod->getreason_phrase() << " " << this->request_URI << END_CLR << std::endl;
    if (allow_methods == true)
        delete reqmethod;
    return (this->message);
}
