#include "ParsingRequest.hpp"

ParsingRequest::ParsingRequest(void)
{
    return ;
}

ParsingRequest::~ParsingRequest(void)
{
    return ;
}

void    ParsingRequest::start_parsing(void)
{
    char *request_ln;
    char *header_fiel;
    size_t  index;

    //std::cout << "REQUEST START PARSING \n" << request << std::endl;;
    index = this->request.find("\r\n");
    request_ln = strtok(&this->request[0], "\r\n");
 //   std::cout << "request line :: \n" << request_ln << std::endl;
    header_fiel = strtok(&this->request[index + 2], "$");
  //  std::cout << "header fields :: \n" << header_fiel << std::endl;
    request_line(request_ln);
    header_fields(header_fiel);
}

void    ParsingRequest::end_header_fields(char **request)
{
    for(size_t i = 0; i < strlen(*request) - 1; i++)
    {
        if ((*request)[i] == '\n' && (*request)[i + 1] == '\n')
        {
            (*request)[i + 1] = '$';
        }
    }
}

void    ParsingRequest::request_line(char *request_line)
{
    int     check = 0;
    char *token;
    std::string methods[3] = {"GET", "POST", "DELETE"};
   
    token = strtok(request_line, " ");
    for (size_t i = 0; i < 3; i++)
    {
        if (token == methods[i])
        {
            this->method = methods[i];
            check = 1;
        }
    }
    if (check == 0)
    {
        std::cout << "RESPONSE 405" << std::endl;
    }
    token = strtok(NULL, " ");
    this->URI = token;

    return ;
}

void    ParsingRequest::header_fields(char *header_fields)
{
    char *token;
    std::string token_string;
    int check;
    std::string compare[9] = {"Content-Length", "Content-Type", "Host", "Connection", "Accept", "Cache-Control", "Postman-Token", "Accept-Encoding", "User-Agent"};

    check = 0;
    if (header_fields == NULL)
    {
        std::cout << "RESPONSE 400" << std::endl;
    }
    while (strcmp(header_fields, "\r\n") != 0)
    {
        
        //sstd::cout << "header_fields :: " << header_fields << std::endl << std::endl;
        if (check == 0)
        {
            token = strtok(header_fields, "\r\n");
          //  std::cout << "Token :: " << token << std::endl << std::endl;
            check = 1;
        }
        for(int index = 0; index < 9; index++)
        {
            if (std::string(token).find(compare[index]) != std::string::npos)
            {
                if (header_fields == NULL)
                {
                    std::cout << "RESPONSE 400" << std::endl;
                }
                token_string = std::string(token);
                token_string.erase(0, token_string.find(":") + 1);
                this->fields[index] = token_string;
            }
        }
        if (!(token = strtok(NULL, "\r\n")))
            break ;
    }
    return ;
}

void    ParsingRequest::set_errors(void)
{
    this->errors.insert(std::pair<int, std::string>(200, "OK"));
    this->errors.insert(std::pair<int, std::string>(201, "Created"));
    this->errors.insert(std::pair<int, std::string>(204, "No Content"));
    this->errors.insert(std::pair<int, std::string>(301, "Moved Permanently"));
    this->errors.insert(std::pair<int, std::string>(400, "Bad Request"));
    this->errors.insert(std::pair<int, std::string>(403, "Forbidden"));
    this->errors.insert(std::pair<int, std::string>(404, "Not Found"));
    this->errors.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
    this->errors.insert(std::pair<int, std::string>(409, "Conflict"));
    this->errors.insert(std::pair<int, std::string>(413, "Payload Too Large"));
    this->errors.insert(std::pair<int, std::string>(414, "URI Too Long"));
    this->errors.insert(std::pair<int, std::string>(500, "Internal Server Error"));
    this->errors.insert(std::pair<int, std::string>(501, "Not Implemented"));

    return ;
}

//getters

std::string ParsingRequest::get_fields(int field)
{
    return (this->fields[field]);
}

std::string ParsingRequest::get_request(void)
{
    return (this->request);
}

void        ParsingRequest::set_request(std::string request)
{
    this->request = request;
    return ;
}

// insert overloading

ParsingRequest& ParsingRequest::operator=(ParsingRequest const &obj)
{
    if (this != &obj)
    {
        this->method = obj.method;
        this->URI = obj.URI;
        this->errors.insert(obj.errors.begin(), obj.errors.end());
        this->fields.insert(obj.fields.begin(), obj.errors.end());
        this->request = obj.request;
    }
    return (*this);
}