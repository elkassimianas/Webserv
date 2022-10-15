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

    // if((pos = this->request.find("\r\n\")) == std::string::npos)
	// 	return (false);
    request.replace(request.end(), request.end(), 1, '$');
    //std::cout << "REQUEST START PARSING \n" << request << std::endl;;
    request_ln = strtok(&this->request[0], "\r\n");
    header_fiel = strtok(NULL, "$");
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
    int check = 0;
    std::string compare[4] = {"Content-Length", "Content-Type", "Host", "Connection"};

    if (header_fields == NULL)
    {
        std::cout << "RESPONSE 400" << std::endl;
    }
   // std::cout << "HEADER FIELDS \n" << header_fields << std::endl;
    while (header_fields && strcmp(header_fields, "\r\n") != 0)
    {
        token = strtok(header_fields, "\r\n");
        for(int i = 0; i < 4; i++)
        {
         //   std::cout << "Inside the for" << std::endl;
//std::cout << "TOKEN :: |" << token << "|"<< std::endl;
          //  std::cout << "compare:: |" << compare[i] << "|" << std::endl;

            if (std::string(token).find(compare[i]) != std::string::npos)
            {
                header_fields = strtok(NULL, "\r\n");
                strtok(token, ":");
                this->fields[i] = strtok(NULL, "\r\n");
              //  std::cout << "TOKEN :: " << this->fields[i] << std::endl;
               // std::cout << "I ::" << i << std::endl; 
                check  = 1;
            }
        }
        if (check == 0)
            header_fields = strtok(NULL, "\n");
        check = 0;
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