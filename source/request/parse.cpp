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