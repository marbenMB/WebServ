#include "../../include/method.hpp"

Post::Post(request rhs)
{
    this->setHost(rhs.gethost());
    this->setRequest_URI(rhs.getrequest_URI());
    this->setHttp_version(rhs.gethttp_version());
    this->setsocketID(rhs.getsocketID());
    this->setStatuscode(200);
    this->setRootPath("/Users/mmasstou/Desktop/webserv/data");
    this->setRequestBody(rhs.getRequestBody());
    this->setContent_Length(rhs.getContent_Length());
    this->setContent_Type(rhs.getContent_Type());
    this->setTransfer_Encoding(rhs.getTransfer_Encoding());
    this->setStatuscode(this->execute_method());
}
Post::~Post()
{
}
int Post::execute_method(void)
{
    std::string buffff;
    char  buff[this->getContent_Length()];
    std::vector<std::string> buff_Body;

    // while (endbyte <= 0)
    // {
    //     n = read(this->getContent_Length(), buff, this->getContent_Length());
    //     buffff.append(buff);
    //     endbyte -= n;
    // }
    // recv(this->getContent_Length(), buff, this->getContent_Length(), 0);
    printf("\n-----%d----%d----\nBODY:%s\n",this->getsocketID(), this->getContent_Length(), this->getRequestBody()[0].c_str());
    buff_Body.push_back(buff);
    this->setRequestBody(buff_Body);
    // check if the method Allowed .
    // if there's content Length :
    // std::cout << "this->getContent_Length() : " << this->getContent_Length() << std::endl;
    // std::cout << "this->getTransfer_Encoding() : " << this->getTransfer_Encoding() << std::endl;
    // if there's Transfer Encoding :
    if (this->getContent_Length() != -1)
    {
        this->parseBody_Content_Length();
        return true;
    }
    else if (!this->getTransfer_Encoding().empty())
    {
        this->parseBody_Transfer_Encoding();
        return true;
    }
    return false;
}

bool Post::parseBody_Content_Length()
{
    // std::cout << "parseBody_Content_Length\n";
    std::vector<std::string> reqbody;
    std::map<std::string, std::string> tmp = this->getContent_Type();

    reqbody = this->getRequestBody();
    // std::cout << "Body size :" <<reqbody.size() << std::endl;
    std::cout << "get +> <Content_Type size='" << tmp.size() << "' type='" << tmp["type"] << "'   boundary='" << tmp["boundary"] << "'>" << std::endl;
    std::cout << "<body type='Content_Length' size=" << reqbody.size() << "> :\n"
              << reqbody.begin()[0] << std::endl
              << "</body>\n\n\n\n";
    // std::vector<std::string>::iterator it = reqbody.begin()[0];
    // std::vector<std::string>  body = split((, "\r\n");
    // std::vector<std::string>::iterator it = body.begin();
    // while (it != reqbody.end())
    // {
    //     std::cout << *it << std::endl;
    //     ++it;
    // }

    /**
     *
     *
     *
     * --------------------------237479391124424959608132
     * ----------------------------237479391124424959608132
     */

    if (tmp["type"].compare("text/html") == 0)
    {
        std::ofstream outFile;

        std::string filename = this->getRootPath() + this->getRequest_URI();
        // std::cout << "getRequest_URI   +>" << this->getRequest_URI() << std::endl;
        // std::cout << "Request Path :" << filename << std::endl;
        outFile.open(filename, std::ifstream::out);
        if (!outFile.is_open())
        {
            this->setStatuscode(400);
        }
        outFile << reqbody.begin()[0];
        this->setResponseBody("");
    }
    else if (tmp["type"].compare("multipart/form-data") == 0)
    {
        /**
         *
           POST /test HTTP/1.1
            Host: foo.example
            Content-Type: multipart/form-data;boundary="boundary"

            --boundary
            Content-Disposition: form-data; name="field1"

            value1
            --boundary
            Content-Disposition: form-data; name="field2"; filename="example.txt"

            value2
            --boundary--
         *
         *
         *
         *
        */
    }
    return 1;
}
bool Post::parseBody_Transfer_Encoding()
{
    std::cout << "parseBody_Transfer_Encoding\n";
    return 1;
}

std::vector<std::string> const &Post::getRequestBody(void) const
{
    return (this->requestBody);
}

void Post::setRequestBody(std::vector<std::string> reqBody)
{
    this->requestBody = reqBody;
}

void method::setContent_Length(int Content_Length)
{
    this->Content_Length = Content_Length;
}
void method::setContent_Type(std::string Content_Type)
{
    std::vector<std::string> tmp = split(Content_Type, ";");
    // std::cout << "set +> <Content_Type size='" << tmp.size() << "' type='" << tmp[0] << "'   boundary='" << tmp[1] << "'>" << std::endl;
    this->Content_Type["type"] = (std::string)tmp[0];
    if (tmp.size() == 2)
        this->Content_Type["boundary"] = trimFront((std::string)tmp[1], "boundary=");
}
void method::setTransfer_Encoding(std::string Transfer_Encoding)
{
    this->Transfer_Encoding = Transfer_Encoding;
}